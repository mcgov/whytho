/* too-simple tool to check latency measurements from mlc */

#include <stdio.h>

#define THRESHOLD 250.0

int main(int argc, char** argv) {
    float sample;
    unsigned count_above_threshold = 0, could_not_parse = 0;

    // skip argument 0 (program name)
    for (int i = 1; i < argc; i++) {
        if (sscanf(argv[i], "%f", &sample) == 1){
            if (sample > THRESHOLD) {
                count_above_threshold++;
            }
            else if (sample < 0.0) {
                printf("Error processing sample: %s\n", argv[i]);
                could_not_parse++;
            }
            else{
                printf("success processing sample < %f: %f\n", THRESHOLD, sample);
            }
        }
        else {
            fprintf(stderr, "ERROR: could not match argument: %s\n", argv[i]);
            could_not_parse++;
        }
    }

    return count_above_threshold < 2 && could_not_parse < 2 ? 0 : 0xFF;
}