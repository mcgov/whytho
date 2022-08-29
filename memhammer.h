#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <emmintrin.h>

#define GIBIBYTE (0x1000 << 12)

#define ROL8(value) ((((size_t)value) << 8) | (((size_t)value) >> (sizeof(size_t) * (sizeof(size_t) - 1))))
#define ITERATIONS 0x200


size_t hammer_memory(size_t *allocation)
{
    size_t timer_samples = 0;
    size_t value = 0xDEADBEEF;
    time_t epoch_start = time(0);
    size_t array_elements = GIBIBYTE / sizeof(size_t);
    uint64_t loop_start, loop_end, iter_start;
    uint64_t iteration_timers[ITERATIONS] = {};
    loop_start = __rdtsc();
    for (int repeat = 0; repeat < ITERATIONS; repeat++)
    {
        iter_start = __rdtsc();
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
            allocation[(i^iter_start)%array_elements] = ROL8(allocation[i]) ^ value * 0xB4DD1EB00;
        }
        // get weird
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around a bit
            value = ROL8(value);
            allocation[(i * iter_start) % (array_elements)] = ROL8(allocation[i]) ^ value * 0xFEDBE75;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around some more
            value = ROL8(value);
            allocation[(i * iter_start * 2) % (array_elements)] = ROL8(allocation[i]) ^ 0xB4ff1ed * value;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around using epoch mod as an offset
            value = ROL8(value);
            allocation[(i * epoch_start) % (array_elements)] = ROL8(allocation[i]) ^ 0xB4ff1ed * value;
        }
        iteration_timers[repeat] = __rdtsc() - iter_start;
    }
    loop_end = __rdtsc();

    for (int i = 0; i < ITERATIONS; i++){
        printf("Access time for iteration %d: %ld (total ticks) %ld (average per element)\n", i, iteration_timers[i], iteration_timers[i]/(5*array_elements));
    }
    printf(
        "Iterated in %ld (total ticks), average ticks/element in %ld ticks\n",
         loop_end-loop_start,
         (loop_end-loop_start)/(ITERATIONS*5*array_elements)
        );

    return allocation[allocation[epoch_start * loop_start % (array_elements)] % (array_elements)];
}
