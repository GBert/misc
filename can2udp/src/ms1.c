#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct id_node {
    uint32_t id;
    uint32_t master_handle;
    struct id_node *next;
};

struct id_node *ms1_root_handle=NULL;
 
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

int main() {
    ms1_add_id(ms1_root_handle, 0x01020304, 0x000882);
    ms1_add_id(ms1_root_handle, 0x04050607, 0x000892);
    ms1_add_id(ms1_root_handle, 0x04050607, 0x000892);
    ms1_add_id(ms1_root_handle, 0x08090A0B, 0x0008A2);
    ms1_add_id(ms1_root_handle, 0x01020304, 0x000882);
    ms1_add_id(ms1_root_handle, 0x01020304, 0x000882);
    ms1_print_handles(ms1_root_handle);

    return 0;
}
