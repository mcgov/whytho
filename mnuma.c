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
/*
    mnuma.c
    let's poke caches and look at the physical page info

    author: Matthew G. McGovern matthew@mcgov.dev

*/
uint64_t time_access_index(uint8_t* allocation, size_t size, size_t index){
    int time_ = time(0);
    // train cache
    struct timespec start, end;
    for (int i = 0; i < 0x1000; i++) {
        for (size_t j = 0; j < size; j++) {
            allocation[j] = (j*i*time_)%0xFF;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint8_t picked = allocation[index];
    clock_gettime(CLOCK_MONOTONIC,&end);
    printf("picked element: %lu in %lu s %lu ns\n", index, end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    picked = allocation[size+(size/2)];
    clock_gettime(CLOCK_MONOTONIC,&end);
    printf("picked element: %lu in %lu s %lu ns\n", size+(size/2), end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);
}

int main(int argc, char** argv) {

    size_t l1d = 0, l1i = 0, l2 = 0, l3 = 0;
    if (argc < 5)
    {
        printf("usage: mnuma `lscpu -C -B | tail -4 | awk ' { print $2 } ' | tr \"\n\" " " `");
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

    printf("Found cache sizes l1d %lu l1i %lu l2 %lu l3 %lu\n", l1d, l1i, l2 ,l3);

    size_t cache_sizes[] = {l1d, l2, l3};

    for (int cache_level = 0; cache_level < sizeof(cache_sizes)/sizeof(size_t); cache_level++) {
        size_t cache_size = cache_sizes[cache_level];
        size_t allocation_size = cache_sizes[cache_level] * 2;
        uint8_t* allocation = aligned_alloc(cache_size, allocation_size);
        for(int i = 0; i < cache_size * 2; i++) {
            allocation[i] = i & 0xFF;
        }
        uint64_t start_address = (uint64_t) allocation;
        uint64_t end_address = (uint64_t) allocation + allocation_size;
        time_access_index(allocation, cache_size-1, cache_size/2);
    }

/*
    char filename[0x100];
    snprintf(filename, sizeof(filename), "/proc/%d/pagemap", getpid());

    int fd = open(filename, O_RDONLY);
    if(fd < 0) {
        perror("open");
        return 1;
    }

    uint64_t start_address = allocation;
    uint64_t end_address = allocation

    for(uint64_t i = start_address; i < end_address; i += 0x1000) {
        uint64_t data;
        uint64_t index = (i / PAGE_SIZE) * sizeof(data);
        if(pread(fd, &data, sizeof(data), index) != sizeof(data)) {
            perror("pread");
            break;
        }
*/
}