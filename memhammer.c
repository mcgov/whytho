#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "memhammer.h"


/*
    memhammer.c
    pointlessly allocate and touch memory to run with `time`

    author: mcgov
        Matthew G McGovern
*/

int main(int argc, char **argv)
{


    printf("memhammer.c: This shouldn't take long...\n");
    size_t *allocation = malloc(GIBIBYTE);
    if (!allocation)
    {
        fprintf(stderr, "Could not allocate a gibibyte! %s\n", strerror(errno));
        return errno;
    }

    // do a bunch of nonsense
    size_t random_sizet = hammer_memory(allocation);
    printf("Random u64 from results: %lx\n", random_sizet);

    free(allocation);
    return 0;
}