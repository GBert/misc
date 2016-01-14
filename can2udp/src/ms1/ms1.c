/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "can2lan.h"

#define MS1_BUFFER_SIZE 8
#define MS1_BUFFER_MASK (MS1_BUFFER_SIZE-1)

extern struct MS1_Node_Buffer ms1_node_buffer;

extern struct id_node *ms1_root_handle;
extern int verbose;

void ms1_node_buffer_init(void) {
    memset(&ms1_node_buffer, 0, sizeof(struct MS1_Node_Buffer));
}

int ms1_node_buffer_in(uint8_t node) {
    uint8_t next = (ms1_node_buffer.write + 1) & MS1_BUFFER_MASK;
    if (ms1_node_buffer.read == next)
	return -1;
    ms1_node_buffer.data[ms1_node_buffer.write & MS1_BUFFER_MASK] = node;
    ms1_node_buffer.write = next;
    return 1;
}

int ms1_node_buffer_out(uint8_t * node) {
    if (ms1_node_buffer.read == ms1_node_buffer.write)
	return -1;
    *node = ms1_node_buffer.data[ms1_node_buffer.read];
    ms1_node_buffer.read = (ms1_node_buffer.read + 1) & MS1_BUFFER_MASK;
    return 1;
}

int ms1_print_handles(struct id_node *node) {
    struct id_node *my_node = node;
    while (my_node != NULL) {
	printf("MS1 ID: 0x%08X Master handle: 0x%02X\n", my_node->id, my_node->slave_node);
	my_node = my_node->next;
    }
    return 0;
}

struct id_node *ms1_search_for_id(struct id_node *node, uint32_t id) {
    struct id_node *my_node = node;
    while (my_node != NULL) {
	if (my_node->id == id)
	    return my_node;
	my_node = my_node->next;
    }
    return NULL;
}

struct id_node *ms1_search_for_slave(struct id_node *node, uint8_t slave_node) {
    struct id_node *my_node = node;
    while (my_node != NULL) {
	if (my_node->slave_node == slave_node)
	    return my_node;
	my_node = my_node->next;
    }
    return NULL;
}

int ms1_add_id(struct id_node *root_node, uint32_t id, uint8_t slave_node) {
    struct id_node *new_node;
    if ((ms1_search_for_id(root_node, id)) == NULL) {
	if ((new_node = (struct id_node *)malloc(sizeof(struct id_node))) != NULL) {
	    new_node->id = id;
	    new_node->slave_node = slave_node;
	    new_node->next = NULL;
	    /* check if this is the first entry */
	    if (ms1_root_handle == NULL) {
		ms1_root_handle = new_node;
	    } else {
		/* insert new enty at the beginning */
		new_node->next = ms1_root_handle->next;
		ms1_root_handle->next = new_node;
	    }
	} else {
	    fprintf(stderr, "can't add MS1 handle - malloc failed\n");
	    return -1;
	}
    }
    return 0;
}
