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
#include <zlib.h>

/* CHUNK is the size of the memory chunk used by the zlib routines. */

#define CHUNK 0x8000
#define windowBits 15
#define GZIP_ENCODING 16

/* The following macro calls a zlib routine and checks the return
   value. If the return value ("status") is not OK, it prints an error
   message and exits the program. Zlib's error statuses are all less
   than zero. */

#define CALL_ZLIB(x) {                                                  \
        int status;                                                     \
        status = x;                                                     \
        if (status < 0) {                                               \
            fprintf (stderr,                                            \
                     "%s:%d: %s returned a bad status of %d.\n",        \
                     __FILE__, __LINE__, #x, status);                   \
            exit (EXIT_FAILURE);                                        \
        }                                                               \
    }

uint8_t * read_config_file(char *filename, uint32_t *nbytes) {
    int rc;
    struct stat st;
    FILE *fp;
    /* char *s; */
    uint8_t *config;

    rc = stat(filename, &st);
    if (rc < 0) {
        printf("%s: error stat failed for file %s\n", __func__, filename);
        return NULL;
    }
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("%s: error fopen failed for file %s\n", __func__, filename);
        return NULL;
    }
    *nbytes = st.st_size;
    config = (uint8_t *)calloc(*nbytes, sizeof(uint8_t));
    if (config == NULL) {
        printf("%s: error calloc failed creatig config buffer for %s\n", __func__, filename);
        return NULL;
    }
    rc = fread((void *)config, 1, *nbytes, fp);
    if ((rc != *nbytes)) {
        printf("%s: error fread failed reading %s\n", __func__, filename);
        return NULL;
    }
    return config;
}

static void strm_init (z_stream * strm) {
    strm->zalloc = Z_NULL;
    strm->zfree  = Z_NULL;
    strm->opaque = Z_NULL;
    CALL_ZLIB (deflateInit2 (strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                             windowBits , 8,
                             Z_DEFAULT_STRATEGY));
}

int send_tcp_config_data(char *filename, int tcp_socket, int flags) {
    /* uint16_t crc; */
    uint32_t *nbytes = NULL;
    uint8_t *config;
    uint8_t *out;
    z_stream strm;
    int inflated_size;

    config=read_config_file(filename, nbytes);
    if (config)  {
        printf("%s read config file %s\n", __func__, filename);
    } else {
        printf("%s: error reading config %s\n", __func__, filename);
        return -1;
    }

    if (flags && COMPRESSED) {
        /* we need some more bytes to prepare send data (includes inflated file size and padding)    */
        /* assuming that out[CHUNK] is large enough to compress the whole file, otherwise don't send */
        out = (uint8_t *)calloc(CHUNK + 12, sizeof(uint8_t));
        if (out == NULL) {
            printf("%s: error calloc failed creating deflation buffer\n", __func__);
            return -1;
        }
        strm_init (& strm);
        strm.next_in = config;
        strm.avail_in = *nbytes;
        strm.avail_out = CHUNK;
        strm.next_out = &out[4];
        CALL_ZLIB (deflate (& strm, Z_FINISH));
        if (strm.avail_out != 0) {
            printf("%s: compressed file to large\n", __func__);
            free(config);
            free(out);
            return -1;
        }
        deflateEnd (& strm);
        /* now prepare the senind buffer */
        inflated_size = htonl(*nbytes);
        memcpy(out, &inflated_size, 4);
    }
    free(config);
    free(out);
    return 0;
}
