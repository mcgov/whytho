#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "memhammer.h"


/*
    memhammer.c
    pointlessly allocate and touch memory to run with `time`

    author: mcgov
        Matthew G McGovern
*/


int main(int argc, char **argv)
{


    long gibibytes;
    if (argc == 1)
        gibibytes = 1;
    else
        gibibytes = atol(argv[1]);

    // put an arbitrary (high) limit on amount of gibibytes we will allocate.
    assert(gibibytes > 0 && gibibytes <= 0x4000);
    // give a short estimation of the time to run.

    printf("memhammer.c: %s...\n", get_time_estimation_string(gibibytes));

    size_t *allocation = malloc(gibibytes * GIBIBYTE);
    if (!allocation)
    {
        fprintf(stderr, "Could not allocate %ld gibibytes! %s\n", gibibytes, strerror(errno));
        return errno;
    }

    // do a bunch of nonsense
    size_t random_sizet = hammer_memory(allocation, gibibytes);
    printf("Random u64 from results: %lx\n", random_sizet);
    free(allocation);
    return 0;
}