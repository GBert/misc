#ifndef COMPARE_H
#define COMPARE_H

#define GREATER(x) ((x)>0)
#define EQUAL(x)   ((x)==0)
#define SMALLER(x) ((x)<0)

typedef int (*CmpFkt)(void *d1, void *d2);

#endif
