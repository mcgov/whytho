#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

#define GIBIBYTE (0x1000 << 12)

#define ROL8(value) ((((size_t)value) << 8) | (((size_t)value) >> (sizeof(size_t) * (sizeof(size_t) - 1))))

struct timespec assign_minimum(struct timespec current, struct timespec next)
{
    if (current.tv_sec || current.tv_nsec)
    {
        if (current.tv_sec < next.tv_sec ||
            (current.tv_sec == next.tv_sec &&
             current.tv_nsec < next.tv_nsec))
            return current;
        else
            return next;
    }
    return next;
}

struct timespec assign_maximum(struct timespec current, struct timespec next)
{
    if (current.tv_sec || current.tv_nsec)
    {
        if (current.tv_sec < next.tv_sec ||
            (current.tv_sec == next.tv_sec &&
            current.tv_nsec < next.tv_nsec))
            return next;
        else
            return current;
    }
    return next;
}
size_t hammer_memory(size_t *allocation, size_t iterations)
{
    size_t timer_samples = 0;
    size_t value = 0xDEADBEEF;
    time_t epoch_start = time(0);
    size_t array_elements = GIBIBYTE / sizeof(size_t);
    struct timespec timer_avg = {0, 0}, timer_min = {0, 0}, timer_max = {0, 0};
    struct timespec timer_start, timer_end;
    for (int repeat = 0; repeat < iterations; repeat++)
    {
        bool sample_time = false;
        if (clock_gettime(CLOCK_MONOTONIC, &timer_start) != 0)
            printf("couldn't get timer for iteration %d: %s\n", repeat, strerror(errno));
        else
            sample_time = true;

        for (size_t i = 0; i < array_elements; i++)
        {
            // uninitialized memory use
            value = ROL8(value);
            allocation[i] += epoch_start * value ^ 0xC4FEF00D;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // smash some bits in sequence
            value = ROL8(value);
            allocation[i] = ROL8(allocation[i]) ^ value * 0xB4DD1EB00;
        }
        // get weird
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around a bit
            value = ROL8(value);
            allocation[(i * 2) % (array_elements)] = ROL8(allocation[i]) ^ value * 0xFEDBE75;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around some more
            value = ROL8(value);
            allocation[(i * 2) % (array_elements)] = ROL8(allocation[i]) ^ 0xB4ff1ed * value;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around using epoch mod as an offset
            value = ROL8(value);
            allocation[(i * epoch_start) % (array_elements)] = ROL8(allocation[i]) ^ 0xB4ff1ed * value;
        }
        if (sample_time && clock_gettime(CLOCK_MONOTONIC, &timer_end) == 0)
        {
            timer_end.tv_nsec -= timer_start.tv_nsec;
            timer_end.tv_sec -= timer_start.tv_sec;
            timer_max = assign_maximum(timer_max, timer_end);
            timer_min = assign_minimum(timer_min, timer_end);
            printf("Iter %d: access in %lu s %lu ns\n", repeat, timer_end.tv_sec, timer_end.tv_nsec);
        }
        else
            printf("couldn't get timer for iteration %d\n", repeat);
    }

    printf(
        "Accesses:\n"
        "Maximum: %lu s %lu ns\n"
        "Minimum:%lu s %luns\n",
        timer_max.tv_sec, timer_max.tv_nsec,
        timer_min.tv_sec, timer_min.tv_nsec);

    return allocation[allocation[epoch_start * timer_end.tv_nsec % (array_elements)] % (array_elements)];
}