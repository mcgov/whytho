#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

/*
    memhammer.c
    pointlessly allocate and touch memory to run with `time`

    author: mcgov
        Matthew G McGovern
*/

#define GIBIBYTE (0x1000 << 12)

#define ROL8(value) ((((size_t)value) << 8) | (((size_t)value)>>(sizeof(size_t)*(sizeof(size_t)-1))))

int main(int argc, char **argv)
{

    size_t* allocation = malloc(GIBIBYTE);
    size_t  value = 0xDEADBEEF;

    if (!allocation){
        fprintf(stderr, "Could not allocate a gibibyte! %s\n", strerror(errno));
        return errno;
    }

    // do a bunch on nonsense
    for (int repeat = 0; repeat < 0x2000; repeat++){
        for (size_t i = 0; i < GIBIBYTE/sizeof(size_t); i++){
            value = ROL8(value);
            allocation[i] ^= ROL8(value) ^ 0xC4FEF00D;
        }
        for (size_t i = 0; i < GIBIBYTE/sizeof(size_t); i++){
            allocation[i] = ROL8(allocation[i]) ^ 0xB4DD1EB00;
        }
        // get weird
        for (size_t i = 0; i < GIBIBYTE/sizeof(size_t); i++){
            value = ROL8(value);
            allocation[(i*2)%(GIBIBYTE/sizeof(size_t))] = ROL8(allocation[i]) ^ ROL8(value);

        }
        for (size_t i = 0; i < GIBIBYTE/sizeof(size_t); i++){
            value = ROL8(value);
            allocation[(i*2)%(GIBIBYTE/sizeof(size_t))] = ROL8(allocation[i]) ^ 0xB4ff1ed * value;
        }
    }

    printf("Random u64 from results: %lx\n", allocation[(time()*time())%(GIBIBYTE/8)]);

    free(allocation);
    return 0;
}