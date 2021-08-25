/*
 * Various functions used by or useful for more than 1 module.
 * 
 */

#include "toolbox.h"

/**
 * cmdTime:
 * compare two time stamps. 
 * Input: 2 time stamps in the struct timeval (from sys/time.h)
 * Output: 1 if timestamp 1 is newer than 2
 *         0 if same or older
 */

int cmpTime(struct timeval *t1, struct timeval *t2)
{
    int result;

    result = 0;
    if (t2->tv_sec > t1->tv_sec) {
        result = 1;
    }
    else {
        if (t2->tv_sec == t1->tv_sec) {
            if (t2->tv_usec > t1->tv_usec) {
                result = 1;
            }
        }
    }
    return result;
}
