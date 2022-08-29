#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdalign.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <assert.h>
#include <math.h>

/*
    mnuma.c
    let's poke caches and look at the physical page info

    author: Matthew G. McGovern matthew@mcgov.dev

*/

#define CACHE_LEVELS 3

#pragma GCC push_options
#pragma GCC optimize("O0")

size_t next_highest_pow2(size_t n)
{
    return 1 << ((sizeof(void *) * 8) - __builtin_clzl(n - 1));
}
struct cache_list
{
    struct cache_list *next;
    size_t data_sz;
    size_t data;
};

/* create n number of linked list elements per cache sized area, across a span of multiple cached size areas */
struct cache_list *create_cache_list(size_t cache_size, size_t per_cache, size_t span_caches)
{
    size_t element_size = cache_size / per_cache;
    size_t elements = per_cache * span_caches;
    assert(elements);
    // aligned alloc requires a power of 2, so check the cache size is one
    // and use the next highest size for the allocator if it's not.
    size_t pow2_check = next_highest_pow2(cache_size);
    if (pow2_check != cache_size)
    {
        printf("NOTE: cache size reported was not a power of 2: (%lx). "
               "Will use next highest pow2 for allocation size: (%lx)\n",
               cache_size, pow2_check);
        cache_size = pow2_check;
    }

    struct cache_list *first = (struct cache_list *)aligned_alloc(cache_size, span_caches*cache_size);
    struct cache_list *next = first;
    memset(first, '\0', cache_size);
    while (--elements > 0)
    { // classic
        next->data_sz = element_size;
        next->next = (struct cache_list *)(((uint8_t *)next) + element_size);
        next = next->next;
    }
    return first;
}

uint64_t time_accesses(struct cache_list *allocation, size_t element_count)
{
    struct cache_list *list_iter = allocation;
    size_t start, end;
    list_iter = allocation;
    // iterate list, time it, and show the time / elements iterated at the end.
    start = __rdtsc();
    while (list_iter)
    {
        // train cache
        list_iter = list_iter->next;
    }
    end = __rdtsc();
    printf("List iterated in %lu clock ticks\n", (end - start) / element_count);
}

void train_and_access(size_t *cache_sizes, size_t numer_of_caches, struct cache_list **allocation_out)
{
    size_t cache_size, allocation_size, start_address, end_address;
    uint64_t start, end;
    struct cache_list *allocation;
    time_t time_ = time(0);
    size_t elements_per_cache = 0x100, cache_count = 0x100;
    for (int cache_level = 0; cache_level < numer_of_caches; cache_level++)
    {

        // get cache size
        cache_size = cache_sizes[cache_level];
        // determine size of allocation
        allocation_size = cache_sizes[cache_level] * 3;

        // create cache list including n elements across multiple cache sized allocations
        allocation = create_cache_list(cache_size, elements_per_cache, cache_count);

        printf("Training and accessing cache L%d... ", cache_level + 1);
        // amount of list elements remains the same, while the jumps between them are variable
        // based on the cache size.
        // we walk each list and divide the time by elements walked.
        time_accesses(allocation, elements_per_cache * cache_count);

        allocation_out[cache_level] = allocation; // save off the allocation
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
    printf("            --- Welcome to memnuma!  <('-'<) --- ");
    printf("Found cache sizes l1d %lx l1i %lx l2 %lx l3 %lx \n", l1d, l1i, l2, l3);

    // run the training and accessing (not guaranteed to work as expected yet)
    size_t cache_sizes[] = {l1d, l2, l3};
    struct cache_list *allocations[CACHE_LEVELS] = {};
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

        uint8_t *start_address = (uint8_t *)allocations[cache_level];
        uint8_t *end_address = start_address + (cache_sizes[cache_level]);

        uint8_t *addresses[] = {start_address, end_address};
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