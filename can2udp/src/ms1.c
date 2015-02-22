#include "can2lan.h"

extern struct id_node *ms1_root_handle;

int ms1_print_handles(struct id_node *node) {
    struct id_node *my_node = node;
    while (my_node != NULL) {
	printf("MS1 ID: 0x%08X Master handle: 0x%08X\n", my_node->id, my_node->master_handle);
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

int ms1_add_id(struct id_node *root_node, uint32_t id, uint32_t master_handle) {
    struct id_node *new_node;
    if ((ms1_search_for_id(root_node, id)) == NULL) {
	if ((new_node = (struct id_node *) malloc(sizeof(struct id_node))) != NULL) {
	    new_node->id = id;
	    new_node->master_handle = master_handle;
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
