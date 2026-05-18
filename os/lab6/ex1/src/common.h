#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

enum {
    CAMERA_QUEUE_CAPACITY = 256,
    CAMERA_PERIOD_NS = 40000000LL,
    ROBOT_PERIOD_NS = 10000000LL,
    OUTPUT_PERIOD_NS = 100000000LL,
    MAX_TIMESTAMP_DIFF_NS = 20000000LL,
};

typedef struct {
    int frame_number;
    struct timespec timestamp;
} CameraFrame;

typedef struct {
    CameraFrame left;
    CameraFrame right;
    unsigned long sequence;
    struct timespec timestamp;
} StereoPair;

typedef struct {
    unsigned long sample_number;
    double x;
    double y;
    double orientation;
    struct timespec timestamp;
} RobotState;

typedef struct {
    const char *label;
    const char *output_dir;
    int duration_seconds;
} SimulationConfig;

int run_simulation(const SimulationConfig *config);

#endif