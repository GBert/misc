#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 256

FILE * fp;
char line[MAXLINE];

extern int errno ;

/*
 * return string with CRLF removed from end
 */
char * rmcrlf(char *s, int slen) {
        int l;

        s[slen - 1] = '\0';

        l = strlen(s) - 1;

        while (l >= 0 && (s[l]=='\r' || s[l]=='\n'))
                s[l--]= '\0';

        return s;
}

int main(int argc, char ** argv) {
    int count = 0;
    int id = 0;
    int seite = 0;
    char gs_name[MAXLINE];
 
    if (argc != 2)
    {
       printf("usage: %s <config>\n", argv[0]);
       exit(1);
    }

    if((fp = fopen(argv[1],"r")) != NULL){
        while(fgets(line,MAXLINE,fp)!=NULL){
            count +=1;
            if (strstr(line, "seite") == line ) {
                seite=1;
            }
            else if (strstr(line, " .id=") == line ) {
                id=strtoul(&line[5], NULL,0);
            }
            else if (strstr(line, " .name=") == line ) {
                rmcrlf(line,MAXLINE);
                strncpy(gs_name, &line[7], MAXLINE);
                if (seite)
                    printf("%02d %s.cs2\n", id, gs_name);
            }
        }
        /* fgets returned null */
        if(errno != 0){
            perror(argv[0]);    /* argv[0]?  Why that? */
            exit(1);
        }
        exit(0);                /* EOF found, normal exit */
    } else {                    /* there was an error on open */
        perror(argv[0]);
        exit(1);
    }
}
