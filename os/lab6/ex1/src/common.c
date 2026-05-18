#define _POSIX_C_SOURCE 200809L

#include "common.h"

#include <errno.h>
#include <limits.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

enum { PATH_BUFFER_SIZE = 4096 };

typedef struct {
    CameraFrame frames[CAMERA_QUEUE_CAPACITY];
    size_t head;
    size_t tail;
    size_t count;
    pthread_mutex_t mutex;
} CameraBuffer;

typedef struct {
    StereoPair value;
    unsigned long sequence;
    bool has_value;
    pthread_mutex_t mutex;
} LatestStereoPair;

typedef struct {
    RobotState value;
    unsigned long sequence;
    bool has_value;
    pthread_mutex_t mutex;
} LatestRobotState;

typedef struct {
    const SimulationConfig *config;
    atomic_bool running;
    pthread_t left_camera_thread;
    pthread_t right_camera_thread;
    pthread_t sync_thread;
    pthread_t writer_thread;
    pthread_t robot_thread;
    pthread_t logger_thread;
    sem_t camera_events;
    sem_t stereo_events;
    sem_t state_events;
    CameraBuffer left_camera;
    CameraBuffer right_camera;
    LatestStereoPair stereo;
    LatestRobotState robot_state;
} SimulationContext;

static long long timespec_to_ns(struct timespec ts) {
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
}

static struct timespec ns_to_timespec(long long ns) {
    struct timespec result;
    result.tv_sec = (time_t)(ns / 1000000000LL);
    result.tv_nsec = (long)(ns % 1000000000LL);
    if (result.tv_nsec < 0) {
        result.tv_nsec += 1000000000L;
        result.tv_sec -= 1;
    }
    return result;
}

static struct timespec timespec_add_ns(struct timespec value, long long ns) {
    return ns_to_timespec(timespec_to_ns(value) + ns);
}

static long long timespec_diff_ns(struct timespec a, struct timespec b) {
    long long diff = timespec_to_ns(a) - timespec_to_ns(b);
    return diff < 0 ? -diff : diff;
}

static int sleep_until(struct timespec deadline) {
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL) == -1) {
        if (errno != EINTR) {
            return -1;
        }
    }

    return 0;
}

static int ensure_directory_exists(const char *path) {
    char buffer[PATH_BUFFER_SIZE];
    size_t length = strlen(path);

    if (length >= sizeof(buffer)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    memcpy(buffer, path, length + 1U);

    for (char *cursor = buffer + 1; *cursor != '\0'; ++cursor) {
        if (*cursor == '/') {
            *cursor = '\0';
            if (mkdir(buffer, 0777) == -1 && errno != EEXIST) {
                return -1;
            }
            *cursor = '/';
        }
    }

    if (mkdir(buffer, 0777) == -1 && errno != EEXIST) {
        return -1;
    }

    return 0;
}

static void camera_buffer_init(CameraBuffer *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->count = 0;
    pthread_mutex_init(&buffer->mutex, NULL);
}

static void latest_stereo_init(LatestStereoPair *stereo) {
    stereo->sequence = 0;
    stereo->has_value = false;
    pthread_mutex_init(&stereo->mutex, NULL);
}

static void latest_robot_init(LatestRobotState *robot_state) {
    robot_state->sequence = 0;
    robot_state->has_value = false;
    pthread_mutex_init(&robot_state->mutex, NULL);
}

static void camera_buffer_push(CameraBuffer *buffer, CameraFrame frame) {
    if (buffer->count == CAMERA_QUEUE_CAPACITY) {
        buffer->head = (buffer->head + 1U) % CAMERA_QUEUE_CAPACITY;
        buffer->count -= 1U;
    }

    buffer->frames[buffer->tail] = frame;
    buffer->tail = (buffer->tail + 1U) % CAMERA_QUEUE_CAPACITY;
    buffer->count += 1U;
}

static CameraFrame camera_buffer_peek(const CameraBuffer *buffer) {
    return buffer->frames[buffer->head];
}

static CameraFrame camera_buffer_pop(CameraBuffer *buffer) {
    CameraFrame frame = buffer->frames[buffer->head];
    buffer->head = (buffer->head + 1U) % CAMERA_QUEUE_CAPACITY;
    buffer->count -= 1U;
    return frame;
}

static int write_stereo_files(const SimulationContext *context, const StereoPair *pair) {
    char left_path[PATH_BUFFER_SIZE];
    char right_path[PATH_BUFFER_SIZE];
    FILE *left_file = NULL;
    FILE *right_file = NULL;

    if (snprintf(left_path, sizeof(left_path), "%s/left_%04lu.jpg", context->config->output_dir, pair->sequence) >= (int)sizeof(left_path)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    if (snprintf(right_path, sizeof(right_path), "%s/right_%04lu.jpg", context->config->output_dir, pair->sequence) >= (int)sizeof(right_path)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    left_file = fopen(left_path, "w");
    if (left_file == NULL) {
        return -1;
    }

    right_file = fopen(right_path, "w");
    if (right_file == NULL) {
        fclose(left_file);
        return -1;
    }

    fprintf(left_file,
            "program=%s\n"
            "pair=%lu\n"
            "camera=left\n"
            "frame=%d\n"
            "timestamp=%lld.%09ld\n"
            "paired_with=%d\n",
            context->config->label,
            pair->sequence,
            pair->left.frame_number,
            (long long)pair->left.timestamp.tv_sec,
            pair->left.timestamp.tv_nsec,
            pair->right.frame_number);

    fprintf(right_file,
            "program=%s\n"
            "pair=%lu\n"
            "camera=right\n"
            "frame=%d\n"
            "timestamp=%lld.%09ld\n"
            "paired_with=%d\n",
            context->config->label,
            pair->sequence,
            pair->right.frame_number,
            (long long)pair->right.timestamp.tv_sec,
            pair->right.timestamp.tv_nsec,
            pair->left.frame_number);

    fclose(left_file);
    fclose(right_file);
    return 0;
}

static int append_robot_state(FILE *log_file, const SimulationConfig *config, const RobotState *state) {
    return fprintf(log_file,
                   "%s sample=%lu timestamp=%lld.%09ld x=%.3f y=%.3f orientation=%.3f\n",
                   config->label,
                   state->sample_number,
                   (long long)state->timestamp.tv_sec,
                   state->timestamp.tv_nsec,
                   state->x,
                   state->y,
                   state->orientation) < 0 ? -1 : 0;
}

static void *left_camera_thread_main(void *user_data) {
    SimulationContext *context = user_data;
    int frame_number = 0;
    struct timespec next_tick;

    clock_gettime(CLOCK_MONOTONIC, &next_tick);
    next_tick = timespec_add_ns(next_tick, CAMERA_PERIOD_NS);

    while (atomic_load(&context->running)) {
        CameraFrame frame;

        if (sleep_until(next_tick) != 0) {
            break;
        }

        frame_number += 1;
        clock_gettime(CLOCK_MONOTONIC, &frame.timestamp);
        frame.frame_number = frame_number;

        pthread_mutex_lock(&context->left_camera.mutex);
        camera_buffer_push(&context->left_camera, frame);
        pthread_mutex_unlock(&context->left_camera.mutex);

        sem_post(&context->camera_events);
        next_tick = timespec_add_ns(next_tick, CAMERA_PERIOD_NS);
    }

    return NULL;
}

static void *right_camera_thread_main(void *user_data) {
    SimulationContext *context = user_data;
    int frame_number = 0;
    struct timespec next_tick;

    clock_gettime(CLOCK_MONOTONIC, &next_tick);
    next_tick = timespec_add_ns(next_tick, CAMERA_PERIOD_NS);

    while (atomic_load(&context->running)) {
        CameraFrame frame;

        if (sleep_until(next_tick) != 0) {
            break;
        }

        frame_number += 1;
        clock_gettime(CLOCK_MONOTONIC, &frame.timestamp);
        frame.frame_number = frame_number;

        pthread_mutex_lock(&context->right_camera.mutex);
        camera_buffer_push(&context->right_camera, frame);
        pthread_mutex_unlock(&context->right_camera.mutex);

        sem_post(&context->camera_events);
        next_tick = timespec_add_ns(next_tick, CAMERA_PERIOD_NS);
    }

    return NULL;
}

static void *sync_thread_main(void *user_data) {
    SimulationContext *context = user_data;
    unsigned long pair_sequence = 0;

    while (atomic_load(&context->running) || context->left_camera.count > 0 || context->right_camera.count > 0) {
        if (sem_wait(&context->camera_events) == -1) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }

        while (sem_trywait(&context->camera_events) == 0) {
        }

        pthread_mutex_lock(&context->left_camera.mutex);
        pthread_mutex_lock(&context->right_camera.mutex);

        while (context->left_camera.count > 0 && context->right_camera.count > 0) {
            CameraFrame left = camera_buffer_peek(&context->left_camera);
            CameraFrame right = camera_buffer_peek(&context->right_camera);
            long long difference = timespec_diff_ns(left.timestamp, right.timestamp);

            if (difference <= MAX_TIMESTAMP_DIFF_NS) {
                StereoPair stereo_pair;

                stereo_pair.left = camera_buffer_pop(&context->left_camera);
                stereo_pair.right = camera_buffer_pop(&context->right_camera);
                pair_sequence += 1;
                stereo_pair.sequence = pair_sequence;
                stereo_pair.timestamp = stereo_pair.left.timestamp;
                if (timespec_to_ns(stereo_pair.right.timestamp) < timespec_to_ns(stereo_pair.timestamp)) {
                    stereo_pair.timestamp = stereo_pair.right.timestamp;
                }

                pthread_mutex_lock(&context->stereo.mutex);
                context->stereo.value = stereo_pair;
                context->stereo.sequence = pair_sequence;
                context->stereo.has_value = true;
                pthread_mutex_unlock(&context->stereo.mutex);

                sem_post(&context->stereo_events);
                continue;
            }

            if (timespec_to_ns(left.timestamp) < timespec_to_ns(right.timestamp)) {
                (void)camera_buffer_pop(&context->left_camera);
            } else {
                (void)camera_buffer_pop(&context->right_camera);
            }
        }

        pthread_mutex_unlock(&context->right_camera.mutex);
        pthread_mutex_unlock(&context->left_camera.mutex);
    }

    return NULL;
}

static void *robot_thread_main(void *user_data) {
    SimulationContext *context = user_data;
    unsigned long sample_number = 0;
    struct timespec next_tick;

    clock_gettime(CLOCK_MONOTONIC, &next_tick);
    next_tick = timespec_add_ns(next_tick, ROBOT_PERIOD_NS);

    for (;;) {
        RobotState state;

        if (!atomic_load(&context->running)) {
            break;
        }

        if (sleep_until(next_tick) != 0) {
            break;
        }

        sample_number += 1;
        clock_gettime(CLOCK_MONOTONIC, &state.timestamp);
        state.sample_number = sample_number;
        state.x = 0.02 * (double)sample_number;
        state.y = 0.01 * (double)sample_number;
        state.orientation = 0.005 * (double)sample_number;

        pthread_mutex_lock(&context->robot_state.mutex);
        context->robot_state.value = state;
        context->robot_state.sequence = sample_number;
        context->robot_state.has_value = true;
        pthread_mutex_unlock(&context->robot_state.mutex);

        sem_post(&context->state_events);
        next_tick = timespec_add_ns(next_tick, ROBOT_PERIOD_NS);
    }

    return NULL;
}

static void *writer_thread_main(void *user_data) {
    SimulationContext *context = user_data;
    unsigned long last_written_sequence = 0;
    unsigned long write_index = 0;
    struct timespec next_tick;

    clock_gettime(CLOCK_MONOTONIC, &next_tick);
    next_tick = timespec_add_ns(next_tick, OUTPUT_PERIOD_NS);

    while (atomic_load(&context->running)) {
        StereoPair current_pair;
        bool has_new_pair = false;

        if (sleep_until(next_tick) != 0) {
            break;
        }

        while (sem_trywait(&context->stereo_events) == 0) {
        }

        pthread_mutex_lock(&context->stereo.mutex);
        if (context->stereo.has_value && context->stereo.sequence != last_written_sequence) {
            current_pair = context->stereo.value;
            has_new_pair = true;
            last_written_sequence = context->stereo.sequence;
        }
        pthread_mutex_unlock(&context->stereo.mutex);

        if (has_new_pair) {
            write_index += 1;
            current_pair.sequence = write_index;
            if (write_stereo_files(context, &current_pair) != 0) {
                perror("write_stereo_files");
                break;
            }
        }

        next_tick = timespec_add_ns(next_tick, OUTPUT_PERIOD_NS);
    }

    return NULL;
}

static void *logger_thread_main(void *user_data) {
    SimulationContext *context = user_data;
    unsigned long last_written_sequence = 0;
    struct timespec next_tick;
    char log_path[PATH_BUFFER_SIZE];
    FILE *log_file = NULL;

    if (snprintf(log_path, sizeof(log_path), "%s/robot_state.txt", context->config->output_dir) >= (int)sizeof(log_path)) {
        errno = ENAMETOOLONG;
        perror("snprintf");
        return NULL;
    }

    log_file = fopen(log_path, "w");
    if (log_file == NULL) {
        perror("fopen");
        return NULL;
    }

    clock_gettime(CLOCK_MONOTONIC, &next_tick);
    next_tick = timespec_add_ns(next_tick, OUTPUT_PERIOD_NS);

    while (atomic_load(&context->running)) {
        RobotState current_state;
        bool has_new_state = false;

        if (sleep_until(next_tick) != 0) {
            break;
        }

        while (sem_trywait(&context->state_events) == 0) {
        }

        pthread_mutex_lock(&context->robot_state.mutex);
        if (context->robot_state.has_value && context->robot_state.sequence != last_written_sequence) {
            current_state = context->robot_state.value;
            has_new_state = true;
            last_written_sequence = context->robot_state.sequence;
        }
        pthread_mutex_unlock(&context->robot_state.mutex);

        if (has_new_state) {
            if (append_robot_state(log_file, context->config, &current_state) != 0) {
                perror("append_robot_state");
                break;
            }
            fflush(log_file);
        }

        next_tick = timespec_add_ns(next_tick, OUTPUT_PERIOD_NS);
    }

    fclose(log_file);
    return NULL;
}

static int initialize_context(SimulationContext *context, const SimulationConfig *config) {
    memset(context, 0, sizeof(*context));
    context->config = config;
    atomic_init(&context->running, true);

    if (ensure_directory_exists(config->output_dir) != 0) {
        return -1;
    }

    if (sem_init(&context->camera_events, 0, 0) != 0) {
        return -1;
    }

    if (sem_init(&context->stereo_events, 0, 0) != 0) {
        sem_destroy(&context->camera_events);
        return -1;
    }

    if (sem_init(&context->state_events, 0, 0) != 0) {
        sem_destroy(&context->camera_events);
        sem_destroy(&context->stereo_events);
        return -1;
    }

    camera_buffer_init(&context->left_camera);
    camera_buffer_init(&context->right_camera);
    latest_stereo_init(&context->stereo);
    latest_robot_init(&context->robot_state);

    return 0;
}

static void destroy_context(SimulationContext *context) {
    sem_destroy(&context->camera_events);
    sem_destroy(&context->stereo_events);
    sem_destroy(&context->state_events);
    pthread_mutex_destroy(&context->left_camera.mutex);
    pthread_mutex_destroy(&context->right_camera.mutex);
    pthread_mutex_destroy(&context->stereo.mutex);
    pthread_mutex_destroy(&context->robot_state.mutex);
}

static int set_high_priority(pthread_t thread) {
    struct sched_param param;

    param.sched_priority = 10;
    if (pthread_setschedparam(thread, SCHED_FIFO, &param) != 0) {
        return -1;
    }

    return 0;
}

int run_simulation(const SimulationConfig *config) {
    SimulationContext context;
    struct timespec start_time;
    struct timespec end_time;
    int status = 0;

    if (config == NULL || config->label == NULL || config->output_dir == NULL || config->duration_seconds <= 0) {
        errno = EINVAL;
        return -1;
    }

    if (initialize_context(&context, config) != 0) {
        return -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    end_time = timespec_add_ns(start_time, (long long)config->duration_seconds * 1000000000LL);

    if (pthread_create(&context.left_camera_thread, NULL, left_camera_thread_main, &context) != 0 ||
        pthread_create(&context.right_camera_thread, NULL, right_camera_thread_main, &context) != 0 ||
        pthread_create(&context.sync_thread, NULL, sync_thread_main, &context) != 0 ||
        pthread_create(&context.writer_thread, NULL, writer_thread_main, &context) != 0 ||
        pthread_create(&context.robot_thread, NULL, robot_thread_main, &context) != 0 ||
        pthread_create(&context.logger_thread, NULL, logger_thread_main, &context) != 0) {
        atomic_store(&context.running, false);
        sem_post(&context.camera_events);
        status = -1;
        goto join_threads;
    }

    (void)set_high_priority(context.robot_thread);

    if (sleep_until(end_time) != 0) {
        status = -1;
    }

    atomic_store(&context.running, false);
    sem_post(&context.camera_events);

join_threads:
    pthread_join(context.left_camera_thread, NULL);
    pthread_join(context.right_camera_thread, NULL);
    pthread_join(context.sync_thread, NULL);
    pthread_join(context.writer_thread, NULL);
    pthread_join(context.robot_thread, NULL);
    pthread_join(context.logger_thread, NULL);

    destroy_context(&context);
    return status;
}