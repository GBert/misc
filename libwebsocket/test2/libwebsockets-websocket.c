#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    return 0;
}

static int callback_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:	// just log message that someone is connecting
	printf("connection established\n");
	break;
    case LWS_CALLBACK_RECEIVE:{// the funny part
	    // create a buffer to hold our response
	    // it has to have some pre and post padding. You don't need to care
	    // what comes there, lwss will do everything for you. For more info see
	    // http://git.warmcat.com/cgi-bin/cgit/lwss/tree/lib/lwss.h#n597
	    unsigned char *buf = (unsigned char *)malloc(LWS_SEND_BUFFER_PRE_PADDING + len +
							 LWS_SEND_BUFFER_POST_PADDING);

	    int i;

	    // pointer to `void *in` holds the incomming request
	    // we're just going to put it in reverse order and put it in `buf` with
	    // correct offset. `len` holds length of the request.
	    for (i = 0; i < len; i++) {
		buf[LWS_SEND_BUFFER_PRE_PADDING + (len - 1) - i] = ((char *)in)[i];
	    }

	    // log what we recieved and what we're going to send as a response.
	    // that disco syntax `%.*s` is used to print just a part of our buffer
	    // http://stackoverflow.com/questions/5189071/print-part-of-char-array
	    printf("received data: %s, replying: %.*s\n", (char *)in, (int)len, buf + LWS_SEND_BUFFER_PRE_PADDING);

	    // send response
	    // just notice that we have to tell where exactly our response starts. That's
	    // why there's `buf[LWS_SEND_BUFFER_PRE_PADDING]` and how long it is.
	    // we know that our response has the same length as request because
	    // it's the same message in reverse order.
	    lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_TEXT);

	    // release memory back into the wild
	    free(buf);
	    break;
	}
    default:
	break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
     "http-only",		// name
     callback_http,		// callback
     0				// per_session_data_size
     },
    {
     "dumb-increment-protocol",	// protocol name - very important!
     callback_dumb_increment,	// callback
     0				// we don't use any per session data
     },
    {
     NULL, NULL, 0		/* End of list */
     }
};

int main(void) {
    // server url will be http://localhost:9000
    int port = 9000;
    struct lws_context *context;
    struct lws_context_creation_info context_info = {
	.port = port,.iface = NULL,.protocols = protocols,.extensions = NULL,
	.ssl_cert_filepath = NULL,.ssl_private_key_filepath = NULL,.ssl_ca_filepath = NULL,
	.gid = -1,.uid = -1,.options = 0, NULL,.ka_time = 0,.ka_probes = 0,.ka_interval = 0
    };
    // create lws context representing this server
    context = lws_create_context(&context_info);

    if (context == NULL) {
	fprintf(stderr, "lws init failed\n");
	return -1;
    }

    printf("starting server...\n");

    // infinite loop, to end this server send SIGTERM. (CTRL+C)
    while (1) {
	lws_service(context, 50);
	// lws_service will process all waiting events with their
	// callback functions and then wait 50 ms.
	// (this is a single threaded webserver and this will keep our server
	// from generating load while there are not requests to process)
    }

    lws_context_destroy(context);

    return 0;
}
