#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdalign.h>
#include <threads.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <x86intrin.h>

/*
    mnuma.c
    let's poke caches and look at the physical page info

    author: Matthew G. McGovern matthew@mcgov.dev

*/

#define CACHE_LEVELS 3

#pragma GCC push_options
#pragma GCC optimize("O0")

uint64_t time_first_access(uint8_t *allocation, size_t size, uint8_t *access_point)
{

    // train cache
    uint64_t start = 0, end = 0;
    uint8_t picked;
    time_t time_ = time(0);

    for (int i = 0; i < 0x1000; i++)
    {
        for (size_t j = size; j > 0; j--)
        {
            allocation[j] = (j * i * time_ ) % 0xFF;
        }
    }

    start = __rdtsc();
    picked = allocation[0];
    end = __rdtsc();
    printf("picked first element (%hhx) in %lu clock ticks\n", picked, end - start);
}

void train_and_access(size_t *cache_sizes, size_t numer_of_caches, uint8_t **allocation_out)
{
    size_t cache_size, allocation_size, start_address, end_address;
    uint64_t start, end;
    uint8_t *allocation, *cache_access_point, picked;
    for (int cache_level = 0; cache_level < numer_of_caches; cache_level++)
    {
        printf("Training and accessing cache L%d-----------\n", cache_level + 1);
        // get cache size
        cache_size = cache_sizes[cache_level];
        // determine size of allocation
        allocation_size = cache_sizes[cache_level] * 3;
        // allocation 3x the size of the cache (aligned alloc)
        allocation = aligned_alloc(cache_size, allocation_size);

        // fill it in with some stuff
        for (int i = allocation_size; i >= 0; i--)
        {
            allocation[i] = i & 0xFF;
        }

        cache_access_point = allocation + allocation_size -1;
        time_first_access(allocation, cache_size, cache_access_point);

        // time second access post-return
        start = __rdtsc();
        picked = *cache_access_point; // access far beyond the first cache sized array
        end = __rdtsc();
        printf("picked last element (%hhx): in %lu clock ticks\n", picked, end - start);

        allocation_out[cache_level] = allocation; //save off the allocation
    }
}
#pragma GCC pop_options

int main(int argc, char **argv)
{

    size_t l1d = 0, l1i = 0, l2 = 0, l3 = 0;
    if (argc < 5)
    {
        printf("usage: mnuma `lscpu -C -B | tail -4 | awk ' { print $2 } ' | tr \"\n\" "
               " `");
        return -1;
    }
    if (sscanf(argv[1], "%lu", &l1d) <= 0)
        return -1;
    if (sscanf(argv[2], "%lu", &l1i) <= 0)
        return -1;
    if (sscanf(argv[3], "%lu", &l2) <= 0)
        return -1;
    if (sscanf(argv[4], "%lu", &l3) == EOF)
        return -1;

    printf("Found cache sizes l1d %lu l1i %lu l2 %lu l3 %lu\n", l1d, l1i, l2, l3);

    // run the training and accessing (not guaranteed to work as expected yet)
    size_t cache_sizes[] = {l1d, l2, l3};
    uint8_t *allocations[CACHE_LEVELS] = {};
    train_and_access(cache_sizes, CACHE_LEVELS, allocations);

    // get pagemap filename
    char filename[0x100];
    snprintf(filename, sizeof(filename), "/proc/%d/pagemap", getpid());

    // open the pagemap
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror(filename);
        goto FREE_ALLOCATIONS;
    }
    printf("Opened %s\n", filename);

    // get the physical address of the pages we used
    for (int cache_level = 0; cache_level < CACHE_LEVELS; cache_level++)
    {

        uint8_t * start_address = allocations[cache_level];
        uint8_t * end_address = allocations[cache_level] + (cache_sizes[cache_level] * 3);

        uint8_t * addresses[] = {start_address, end_address};
        for (uint64_t i = 0; i < 2; i++)
        {
            uint64_t data;
            uint64_t index = (((uint64_t)addresses[i]) / 0x1000) * sizeof(data);
            if (pread(fd, &data, sizeof(data), index) != sizeof(data))
            {
                perror("pread");
                goto CLOSE_FD;
            }
            else
            {
                printf("Cache L%d allocation was at:"
                       " %-16lx (virtual)  %-16lx (physical)\n",
                       cache_level,
                       (uint64_t)addresses[i],
                       (data & 0x7fffffffffffff) * 0x1000);
            }
        }
    }

CLOSE_FD:
    close(fd);
FREE_ALLOCATIONS:
    for (int i = 0; i < CACHE_LEVELS; i++)
    {
        free(allocations[i]);
    }

    return 0;
}