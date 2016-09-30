#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

int main(int argc, char **argv) {

    int size = sizeof(size_t); 
    int byte_size = 32/sizeof(size_t);

    size_t byteset[32/sizeof(size_t)];

    printf("size of size_t %d   byte_size = %d\n", size, byte_size);

    printf("size of byteset %d\n", sizeof byteset);

    memset(byteset, 0, sizeof byteset);
     
    
    return 0;
}
