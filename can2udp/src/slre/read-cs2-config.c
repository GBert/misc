#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "slre.h"

#define MAXLINE 100

FILE *fp;
char bufr[MAXLINE];

extern int errno;

int main(int argc, char **argv) {
    int count = 0;
    int id = 0;
    struct slre_cap caps[10];

    if (argc != 2) {
	printf("usage: %s <config>\n", argv[0]);
	exit(1);
    }

    if ((fp = fopen(argv[1], "r")) != NULL) {
	/* then file opened successfully. */
	while (fgets(bufr, MAXLINE, fp) != NULL) {
	    /* then no read error */
	    count += 1;
	    /* printf("%d: >%s<\n", count, bufr); */
	    if (slre_match("^seite$", bufr, 5, NULL, 0, 0) > 0) {
		printf("%d: %s", count, bufr);
	    }
	    else if (slre_match("^ .id=(\\d+)", bufr, strlen(bufr), caps, 0, 0) > 0) {
		id = atoi(caps[0].ptr);
		printf("%d\n", id);
	    }
	    else if (slre_match("^ .name=(\\S+)", bufr, strlen(bufr), caps, 0, 0) > 0) {
		printf("%s %d\n", caps[0].ptr, id);
	    }
	}
	/* fgets returned null */
	if (errno != 0) {
	    perror(argv[0]);	/* argv[0]?  Why that? */
	    exit(1);
	}
	exit(0);		/* EOF found, normal exit */
    } else {			/* there was an error on open */
	perror(argv[0]);
	exit(1);
    }
}
