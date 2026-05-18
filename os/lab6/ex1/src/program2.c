#include "common.h"

#include <stdlib.h>

int main(int argc, char *argv[]) {
    SimulationConfig config = {
        .label = "program2",
        .output_dir = "output/program2",
        .duration_seconds = 20,
    };

    if (argc > 1) {
        config.duration_seconds = atoi(argv[1]);
    }

    return run_simulation(&config) == 0 ? 0 : 1;
}