#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char **argv) {

    unsigned int baud, temp, div0, div1;
    double real_baud;
    
    if (argc == 2) {
	baud = strtoul(argv[1], (char **)NULL, 10);
        
	temp = 12000000 * 32 / baud;
        div1 = 0;
        while (temp >= 256) {
	    temp >>= 1;
	    div1 += 1;
	    printf("temp:%d  div1:%d \n", temp, div1);
        }
        div0 = temp;
	printf("baud: %d  div0:%d  div1:%d \n", baud, div0, div1);
        real_baud = 12000000 * 32 / pow(2, div1); 
        real_baud = real_baud / (double) div0;
	printf("baud: %d realbaud:%f \n", baud, real_baud);
	
    }
    return 0;
}
