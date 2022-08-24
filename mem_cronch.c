#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/*
    mem_cronch.c
    pointlessly allocate and memcpy to run with `time`

    author: mcgov
        Matthew G McGovern
*/

#define GIBIBYTE (0x1000<<12)


int main(int argc, char** argv) {

    int result;
    if (argc < 2) {
        fprintf(stderr,"Usage: cruncher <size in GB to allocate>\n");
        return -1;
    }

    int allocated_gb = atoi(argv[1]);
    if (allocated_gb < 1){
        fprintf(stderr, "Argument must be nonnegative integer < 0x8000_0000\n");
        return -1;
    }

    int fd_random = open("/dev/random", O_RDONLY);
    if (fd_random == -1) {
        fprintf(stderr, "Error opening /dev/random: %s\n", strerror(errno));
        return errno;
    }

    // table to hold our pointers
    void** allocations = calloc(allocated_gb, sizeof(void*));
    if (allocations == NULL) {
        fprintf(stderr, "Could not allocate pointer array for allocations: %s\n", strerror(errno));
        return errno;
    }

    // allocate and fill the allocations
    for (int i = 0; i < allocated_gb; i++) {
        allocations[i] = malloc(GIBIBYTE);
        if (allocations[i] == NULL) {
            fprintf(stderr, "Could not allocate the %dth allocation: %s\n", i, strerror(errno));
            goto __TEARDOWN;
        } else
            printf("Chronch allocated 1GiB at %p\n", allocations[i]);
        int bytes_read = 0, last_read = 0;
        while (bytes_read < GIBIBYTE){
            last_read += read(fd_random, allocations[i]+bytes_read, GIBIBYTE-bytes_read);
            if (last_read == -1){
                fprintf(stderr, "Read random error: %s\n", strerror(errno));
                goto __TEARDOWN;
            } else {
                printf("Cronch read 0x%x bytes from /dev/random...\n", last_read);
            }
            bytes_read += last_read;
        }
    }

    close(fd_random);

    // do some pointless memcpys
    for(int i = 0, j=allocated_gb-1; i < allocated_gb, j >= 0; i++, j--) {
        if (i != j) {
            memcpy(allocations[i], allocations[j], GIBIBYTE);
        }
        if (i > j && memcmp(allocations[i],allocations[j], GIBIBYTE) != 0) {
            fprintf(stderr, "Memory for allocations %d and %d differed after memcpy\n", i, j);
        } else {
            printf("Cronch memcpy'd allocation[%d] into allocation[%d]...\n", i ,j);
        }
    }


    printf("Cronch used some memory and did some memcpys. Tearing down...\n");

__TEARDOWN:
    result = errno;
    for (int i = 0; i < allocated_gb; i++){
        if (allocations[i]) free(allocations[i]);
        else break;
    }
    free(allocations);
    return 0;

}