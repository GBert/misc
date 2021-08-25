/* cvs: $Id: netservice.c 1735 2016-04-17 09:40:50Z gscholz $             */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <sys/socket.h>

#include "netservice.h"
#include "clientservice.h"
#include "srcp-server.h"
#include "syslogmessage.h"


typedef struct _THREADS {
    unsigned short int port;
    int socket;
} net_thread_t;

static pthread_t netservice_tid;


void change_privileges()
{
    struct group *group;
    struct passwd *passwd;
    char *grp = ((SERVER_DATA *) buses[0].driverdata)->groupname;
    char *uid = ((SERVER_DATA *) buses[0].driverdata)->username;


    if (grp != NULL) {
        if ((group = getgrnam(grp)) != NULL ||
            (group = getgrgid((gid_t) atoi(grp))) != NULL) {
            if (setegid(group->gr_gid) != 0) {
                syslog_bus(0, DBG_WARN, "Could not change to group %s: %s",
                           group->gr_name, strerror(errno));
            }
            else {
                syslog_bus(0, DBG_INFO, "Changed to group %s",
                           group->gr_name);
            }
        }
        else {
            syslog_bus(0, DBG_WARN, "Could not change to group %s", grp);
        }
    }

    if (uid != NULL) {
        if ((passwd = getpwnam(uid)) != NULL ||
            (passwd = getpwuid((uid_t) atoi(uid))) != NULL) {
            if (seteuid(passwd->pw_uid) != 0) {
                syslog_bus(0, DBG_INFO, "Could not change to user %s: %s",
                           passwd->pw_name, strerror(errno));
            }
            else {
                syslog_bus(0, DBG_INFO, "Changed to user %s",
                           passwd->pw_name);
            }
        }
        else {
            syslog_bus(0, DBG_INFO, "Could not change to user %s", uid);
        }
    }
}

/*runtime check for ipv6 support */
int ipv6_supported()
{
#ifdef ENABLE_IPV6
    int s = socket(AF_INET6, SOCK_STREAM, 0);
    if (s != -1) {
        close(s);
        return 1;
    }
#endif
    return 0;
}

/*cleanup routine for network syn request thread*/
void end_netrequest_thread(net_thread_t * ntd)
{
    if (ntd->socket != -1) {
        close(ntd->socket);
    }
    free(ntd);
    free(buses[0].driverdata);
}

/*handle incoming network syn requests*/
void *thr_handlePort(void *v)
{
    int last_cancel_state, last_cancel_type;
    pthread_t ttid;
    int result;

    net_thread_t *ntd = (net_thread_t *) malloc(sizeof(net_thread_t));
    if (ntd == NULL)
        pthread_exit((void *) 1);

    ntd->port = (unsigned long int) v;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &last_cancel_type);

    /*register cleanup routine */
    pthread_cleanup_push((void *) end_netrequest_thread, (void *) ntd);

#ifdef ENABLE_IPV6
    struct sockaddr_in6 sin6;
    struct sockaddr_in6 fsin6;
#endif
    struct sockaddr_in sin;
    struct sockaddr_in fsin;
    struct sockaddr *saddr, *fsaddr;
    socklen_t socklen;
    int sock_opt;
    bool has_ipv6_support = ipv6_supported();

#ifdef ENABLE_IPV6
    if (has_ipv6_support) {
        memset(&sin6, 0, sizeof(sin6));
        sin6.sin6_family = AF_INET6;

        sin6.sin6_port = htons(ntd->port);
        sin6.sin6_addr = in6addr_any;

        /* create a socket for listening */
        ntd->socket = socket(AF_INET6, SOCK_STREAM, 0);
        if (ntd->socket == -1) {
            syslog_bus(0, DBG_ERROR,
                       "Socket creation failed: %s (errno = %d). "
                       "Terminating...\n", strerror(errno), errno);
            exit(EXIT_FAILURE);
        }
        saddr = (struct sockaddr *) &sin6;
        fsaddr = (struct sockaddr *) &fsin6;
        socklen = sizeof(sin6);
    }
    else
#endif
    {
        /* Here would be the original IPv4 code as usual */
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;       /* IPv4 address family */
        sin.sin_port = htons(ntd->port);
        sin.sin_addr.s_addr = INADDR_ANY;

        /* Create the socket */
        ntd->socket = socket(AF_INET, SOCK_STREAM, 0);
        if (ntd->socket == -1) {
            syslog_bus(0, DBG_ERROR,
                       "Socket creation failed: %s (errno = %d). "
                       "Terminating...\n", strerror(errno), errno);
            exit(EXIT_FAILURE);
        }
        saddr = (struct sockaddr *) &sin;
        fsaddr = (struct sockaddr *) &fsin;
        socklen = sizeof(sin);
    }
    if (getuid() == 0) {
        change_privileges();
    }

    sock_opt = 1;
    if (setsockopt(ntd->socket, SOL_SOCKET, SO_REUSEADDR, &sock_opt,
                   sizeof(sock_opt)) == -1) {
        syslog_bus(0, DBG_ERROR, "setsockopt() failed: %s (errno = %d). "
                   "Terminating...\n", strerror(errno), errno);
        close(ntd->socket);
        exit(EXIT_FAILURE);
    }

    /* saddr=(sockaddr_in) if ntd.socket is of type AF_INET else its (sockaddr_in6) */
    if (bind(ntd->socket, (struct sockaddr *) saddr, socklen) == -1) {
        syslog_bus(0, DBG_ERROR, "bind() failed: %s (errno = %d). "
                   "Terminating...\n", strerror(errno), errno);
        close(ntd->socket);
        exit(EXIT_FAILURE);
    }

    if (listen(ntd->socket, 1) == -1) {
        syslog_bus(0, DBG_ERROR, "Listen failed: %s (errno = %d). "
                   "Terminating...\n", strerror(errno), errno);
        close(ntd->socket);
        exit(EXIT_FAILURE);
    }

    /* Wait for connection requests */
    for (;;) {
        pthread_testcancel();
        int clientsocket = accept(ntd->socket, (struct sockaddr *) fsaddr,
                &socklen);

        if (clientsocket == -1) {
            /* Possibly the connection got aborted */
            syslog_bus(0, DBG_WARN, "accept() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
            continue;
        }

        syslog_bus(0, DBG_INFO, "New connection received.\n");
        /* Now process the connection as per the protocol */
#ifdef ENABLE_IPV6
        if (has_ipv6_support) {
            /* This casting must work as we have taken care of the
             * appropriate data structures */
            struct sockaddr_in6 *sin6_ptr = (struct sockaddr_in6 *) fsaddr;
            char addrbuf[INET6_ADDRSTRLEN];

            if (IN6_IS_ADDR_V4MAPPED(&(sin6_ptr->sin6_addr))) {
                syslog_bus(0, DBG_INFO,
                           "Connection from an IPv4 client\n");
            }

            syslog_bus(0, DBG_INFO, "Connection from %s/%d\n",
                       inet_ntop(AF_INET6, (void *) &(sin6_ptr->sin6_addr),
                                 addrbuf, sizeof(addrbuf)),
                       ntohs(sin6_ptr->sin6_port));
        }
        else
#endif
        {
            struct sockaddr_in *sin_ptr = (struct sockaddr_in *) fsaddr;
            syslog_bus(0, DBG_INFO, "Connection from %s/%d\n",
                       inet_ntoa(sin_ptr->sin_addr),
                       ntohs(sin_ptr->sin_port));
        }
        sock_opt = 1;
        if (setsockopt(clientsocket, SOL_SOCKET, SO_KEEPALIVE, &sock_opt,
                       sizeof(sock_opt)) == -1) {
            syslog_bus(0, DBG_ERROR,
                       "Setsockopt failed: %s (errno = %d)\n",
                       strerror(errno), errno);
            close(clientsocket);
            continue;
        }

        /* create an anonymous session with a valid socket */
        session_node_t *asn = create_anonymous_session(clientsocket);
        if (asn == NULL) {
            close(clientsocket);
            syslog_bus(0, DBG_ERROR, "Session create failed!");
            continue;
        }

        /* hand over client service to "thr_doClient()" from clientservice.c */
        result = pthread_create(&ttid, NULL, thr_doClient, asn);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR, "Create thread for network client "
                       "failed: %s (errno = %d). Terminating...\n",
                       strerror(result), result);
            close(clientsocket);
            destroy_anonymous_session(asn);
            continue;
        }
    }

    /*run the cleanup routine */
    pthread_cleanup_pop(1);
    return NULL;
}

/* create network connection thread */
void create_netservice_thread()
{
    int result;
    unsigned short int port;

    port = ((SERVER_DATA *) buses[0].driverdata)->TCPPORT;

    /*TODO: search for other solution than doubled type cast */
    result = pthread_create(&netservice_tid, NULL, thr_handlePort,
                            (void *) (unsigned long int) port);

    if (result != 0) {
        syslog_bus(0, DBG_ERROR, "Create netservice thread failed: %s "
                   "(errno = %d). Terminating...\n", strerror(result),
                   result);
        exit(EXIT_FAILURE);
    }

    syslog_bus(0, DBG_INFO, "Netservice thread for port %d created.",
               port);
}

/* cancel network connection thread */
void cancel_netservice_thread()
{
    int result;
    void *thr_result;

    result = pthread_cancel(netservice_tid);
    if (result != 0)
        syslog_bus(0, DBG_ERROR,
                   "Netservice thread cancel failed: %s (errno = %d).",
                   strerror(result), result);

    /*wait for termination */
    result = pthread_join(netservice_tid, &thr_result);
    if (result != 0)
        syslog_bus(0, DBG_ERROR,
                   "Netservice thread join failed: %s (errno = %d).",
                   strerror(result), result);

    syslog_bus(0, DBG_INFO, "Netservice thread terminated.");
}
