#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <emmintrin.h>

#define GIBIBYTE (0x1000 << 12)
#define ROL8(value) ((((size_t)value) << 8) | (((size_t)value) >> (sizeof(size_t) * (sizeof(size_t) - 1))))
#define ITERATIONS 0x200

size_t hammer_memory(size_t *allocation, size_t size)
{
    size_t timer_samples = 0;
    size_t value = 0xDEADBEEF;
    time_t epoch_start = time(0);
    size_t array_elements = (size * GIBIBYTE) / sizeof(size_t);
    uint64_t loop_start, loop_end, iter_start, max = 0, min = UINT64_MAX, loop_work_time = 0;
    size_t element_accesses = (5 * array_elements * 2);

    uint64_t iteration_timers[ITERATIONS] = {};
    loop_start = __rdtsc();
    for (int repeat = 0; repeat < ITERATIONS; repeat++)
    {
        value = ROL8(value);
        iter_start = __rdtsc();

        // multiple pointless allocation accesses
        // assignments/accesses hold no special purpose other than
        // being sort of random
        for (size_t i = 0; i < array_elements; i++)
        {
            allocation[i] += allocation[(iter_start + i) % array_elements];
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            allocation[(i ^ iter_start) % array_elements] = allocation[i];
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around a bit
            allocation[(i ^ iter_start) % (array_elements)] = allocation[i] ^ value ^ 0xFEDBE75;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around some more
            allocation[(i ^ iter_start ^ allocation[i]) % (array_elements)] = allocation[i] ^ 0xB4ff1ed ^ value;
        }
        for (size_t i = 0; i < array_elements; i++)
        {
            // jump around using epoch mod as an offset
            allocation[(i ^ epoch_start ^ iter_start ^ allocation[i]) % (array_elements)] = allocation[i] ^ 0xB4ff1ed ^ value;
        }
        iteration_timers[repeat] = __rdtsc() - iter_start;
        if (iteration_timers[repeat] > max)
            max = iteration_timers[repeat];
        if (iteration_timers[repeat] < min)
            min = iteration_timers[repeat];
    }
    loop_end = __rdtsc();
    for (int i = 0; i < ITERATIONS; i++)
    {
        printf("Access time for iteration %d: %ld (total ticks) %ld (average per element)\n",
               i, iteration_timers[i], iteration_timers[i] / element_accesses );
        loop_work_time += iteration_timers[i];
    }

    printf(
        "Iterated in %ld (total) %ld (work), average ticks/element in %ld ticks, max: %ld min: %ld\n",
        loop_end - loop_start,
        loop_work_time,
        loop_work_time / (ITERATIONS * element_accesses),
        max / element_accesses,
        min / element_accesses);

    return allocation[allocation[epoch_start * loop_start % (array_elements)] % (array_elements)];
}

static const char* time_estimate_messages[] = {
    "This will go very quickly",
    "This will go pretty quickly",
    "This might go sort of quicly"
    "This might take a while",
    "This will take a little while",
    "This will take a long time",
    "You've selected over 1TiB of memory, this might take a very long time.",
    "If you don't have 4TiB of RAM this will take a verrrry long time",
    "You have 17TiB of ram?? What year is it?"
};
const char* get_time_estimation_string(long gibibytes){
    for (int i = 0; i < sizeof(time_estimate_messages)/sizeof(const char*); i++){
        if (1<<(i*2) >= gibibytes) {
            return time_estimate_messages[i];
        }
    }
    assert("Error: out-of-bounds gibibyte value should have been asserted in memhammer.c\n");
}