/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _SUBSCRIBER_H
#define _SUBSCRIBER_H

#include <time.h>
#include <netinet/in.h>
#include "uthash.h"

#define	DEFAULT_BROADCAST_FLAGS	0x00000001

struct subscriber_t {
    uint32_t ip;
    uint32_t broadcast_flags;
    struct sockaddr_in client_addr;
    int client_socket;
    int active;
    struct timeval last_seen;
    UT_hash_handle hh;
};

int add_z21c_ip(uint32_t ip, int verbose);
int del_z21c_ip(uint32_t ip);
int set_z21c_bcf(uint32_t ip, uint32_t broadcast_flags);


#endif /* _SUBSCRIBER_H */
