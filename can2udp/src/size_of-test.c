#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>

int main(int argc, char **argv) {

    int size = sizeof(size_t); 
    int byte_size = 32/sizeof(size_t);

    printf("size of size_t %d   byte_size = %d\n", size, byte_size);
    
    return 0;
}
