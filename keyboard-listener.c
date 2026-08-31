#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <linux/input.h>
#include <poll.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "key-state.h"

#define BITS_PER_LONG (sizeof(unsigned long) * 8)
#define BIT_ARRAY_SIZE(bits) (((bits) + BITS_PER_LONG - 1) / BITS_PER_LONG)
#define TEST_BIT(bits, bit) ((bits[(bit) / BITS_PER_LONG] >> ((bit) % BITS_PER_LONG)) & 1UL)

struct device {
    int fd;
    char *path;
    struct bongo_cat_key_state key_state;
};

static volatile sig_atomic_t keep_running = 1;

static void stop(int signal_number) {
    (void)signal_number;
    keep_running = 0;
}

static const char *event_message(enum bongo_cat_event event) {
    switch (event) {
    case BONGO_CAT_EVENT_LEFT_DOWN: return "left down";
    case BONGO_CAT_EVENT_LEFT_UP: return "left up";
    case BONGO_CAT_EVENT_RIGHT_DOWN: return "right down";
    case BONGO_CAT_EVENT_RIGHT_UP: return "right up";
    case BONGO_CAT_EVENT_NONE: return NULL;
    }

    return NULL;
}

static bool is_keyboard(int fd) {
    unsigned long event_bits[BIT_ARRAY_SIZE(EV_MAX + 1)] = {0};
    unsigned long key_bits[BIT_ARRAY_SIZE(KEY_MAX + 1)] = {0};

    if (ioctl(fd, EVIOCGBIT(0, sizeof(event_bits)), event_bits) < 0
        || !TEST_BIT(event_bits, EV_KEY)
        || ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits) < 0) {
        return false;
    }

    // KEY_A separates actual keyboards from power buttons and mouse devices.
    return TEST_BIT(key_bits, KEY_A);
}

static bool add_device(struct device **devices, size_t *count, size_t *capacity, const char *path) {
    int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) return false;

    if (!is_keyboard(fd)) {
        close(fd);
        return false;
    }

    if (*count == *capacity) {
        size_t next_capacity = *capacity == 0 ? 4 : *capacity * 2;
        struct device *next = realloc(*devices, next_capacity * sizeof(**devices));
        if (next == NULL) {
            close(fd);
            return false;
        }
        *devices = next;
        *capacity = next_capacity;
    }

    struct device *device = &(*devices)[*count];
    memset(device, 0, sizeof(*device));
    device->fd = fd;
    device->path = strdup(path);
    if (device->path == NULL) {
        close(fd);
        return false;
    }

    ++*count;
    return true;
}

static void close_devices(struct device *devices, size_t count) {
    for (size_t index = 0; index < count; ++index) {
        close(devices[index].fd);
        free(devices[index].path);
    }
    free(devices);
}

int main(int argc, char **argv) {
    const char *requested_device = argc == 2 ? argv[1] : NULL;
    if (argc > 2) {
        fprintf(stderr, "usage: %s [/dev/input/eventN]\n", argv[0]);
        return 64;
    }

    struct device *devices = NULL;
    size_t device_count = 0;
    size_t device_capacity = 0;
    unsigned int side_counts[2] = {0, 0};

    if (requested_device != NULL) {
        add_device(&devices, &device_count, &device_capacity, requested_device);
    } else {
        glob_t matches = {0};
        if (glob("/dev/input/event*", 0, NULL, &matches) == 0) {
            for (size_t index = 0; index < matches.gl_pathc; ++index) {
                add_device(&devices, &device_count, &device_capacity, matches.gl_pathv[index]);
            }
        }
        globfree(&matches);
    }

    if (device_count == 0) {
        fprintf(stderr, "No readable keyboard event device. Add this user to the input group, then sign out and back in.\n");
        return 77;
    }

    struct pollfd *poll_fds = calloc(device_count, sizeof(*poll_fds));
    if (poll_fds == NULL) {
        fprintf(stderr, "Could not allocate keyboard listener state.\n");
        close_devices(devices, device_count);
        return 70;
    }

    for (size_t index = 0; index < device_count; ++index) {
        poll_fds[index].fd = devices[index].fd;
        poll_fds[index].events = POLLIN;
    }

    signal(SIGINT, stop);
    signal(SIGTERM, stop);
    puts("ready");
    fflush(stdout);

    while (keep_running) {
        int result = poll(poll_fds, device_count, -1);
        if (result < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "Keyboard listener poll failed: %s\n", strerror(errno));
            break;
        }

        for (size_t index = 0; index < device_count; ++index) {
            if ((poll_fds[index].revents & POLLIN) == 0) continue;

            struct input_event event;
            while (read(poll_fds[index].fd, &event, sizeof(event)) == sizeof(event)) {
                if (event.type != EV_KEY) continue;

                const bool is_new_key_down = event.value == 1 && event.code <= KEY_MAX
                    && !devices[index].key_state.pressed[event.code];
                enum bongo_cat_event key_event = bongo_cat_key_event(
                    &devices[index].key_state, side_counts, event.code, event.value
                );
                if (is_new_key_down && key_counts_toward_wpm(event.code)) {
                    puts("typed");
                    fflush(stdout);
                }

                const char *message = event_message(key_event);
                if (message != NULL) {
                    puts(message);
                    fflush(stdout);
                }
            }
        }
    }

    free(poll_fds);
    close_devices(devices, device_count);
    return 0;
}
