/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "subscriber.h"
#include "uthash.h"
#include "z21.h"

struct subscriber_t *subscriber = NULL;

int add_z21c_ip(uint32_t ip) {
    struct subscriber_t *sub;

    HASH_FIND_INT(subscriber, &ip, sub);
    if (sub) {
    } else {
	sub = (struct subscriber_t *)calloc(1, sizeof(struct subscriber_t));
	if (!sub) {
	    fprintf(stderr, "%s: can't calloc subscriber IP: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	sub->client_addr.sin_addr.s_addr = ip;
	sub->client_addr.sin_family = AF_INET;
	sub->ip = ip;
	/* TODO */
	sub->client_addr.sin_port = htons(21105);
	sub->client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (sub->client_socket < 0) {
	    fprintf(stderr, "primary sending UDP socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	gettimeofday(&(sub->last_seen), NULL);

	printf("Client IP added %s\n", inet_ntoa(sub->client_addr.sin_addr));
	HASH_ADD(hh, subscriber, ip, sizeof(uint32_t), sub);
    }

    return EXIT_SUCCESS;
}

int set_z21c_bcf(uint32_t ip, uint32_t broadcast_flags) {
    struct subscriber_t *sub;

    HASH_FIND_INT(subscriber, &ip, sub);
    if (sub) {
	sub->broadcast_flags = broadcast_flags;
	return EXIT_SUCCESS;
    }
    return(EXIT_FAILURE);
}

int del_z21c_ip(uint32_t ip) {
    struct subscriber_t *sub;

    HASH_FIND_INT(subscriber, &ip, sub);
    if (sub) {
	HASH_DEL(subscriber, sub);
	return EXIT_SUCCESS;
    }
    return(EXIT_FAILURE);
}

