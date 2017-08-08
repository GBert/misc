
#include "ringb.h"
void ringb_init(struct ringb *ring, uint8_t * buf, int len) {
    ring->buf_len = len;
    ring->buf = buf;
    ring->idx_r = 0;
    ring->idx_w = 0;
}

int ringb_get(struct ringb *ring) {
#if 1
    int rval;
    if (ring->idx_r != ring->idx_w) {
	rval = ring->buf[ring->idx_r];
	ring->idx_r = (ring->idx_r + 1) % ring->buf_len;
	return rval;
    }
    return -1;
#else
    if (((ring->idx_w - ring->idx_r) % ring->buf_len) >= 0) {
	int rval = ring->buf[ring->idx_r];
	ring->idx_r = (ring->idx_r + 1) % ring->buf_len;
	return rval;
    } else {
	return -1;
    }
#endif
}

bool ringb_put(struct ringb * ring, uint8_t c) {
#if 1
    unsigned int next = (ring->idx_w + 1) % ring->buf_len;
    if (next != ring->idx_r) {
	ring->buf[ring->idx_w] = c;
	ring->idx_w = next;
	return true;
    }
    return false;
#else
    if (((ring->idx_w - ring->idx_r) % ring->buf_len) == 0) {
	return false;
    }
    ring->buf[ring->idx_w] = c;
    ring->idx_w = (ring->idx_w + 1) % ring->buf_len;
    return true;
#endif

}

void ringb_flush(struct ringb *ring) {
    ring->idx_r = 0;
    ring->idx_w = 0;
}

int ringb_depth(struct ringb *ring) {
    return ((unsigned int)(ring->idx_w - ring->idx_r) % ring->buf_len);
}
