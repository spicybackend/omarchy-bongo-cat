#ifndef BONGO_CAT_KEY_STATE_H
#define BONGO_CAT_KEY_STATE_H

#include "key-side.h"

#include <stdbool.h>

// A press is tracked per input device. Aggregate side counts prevent releasing
// one held key from lifting a paw while another key on that side remains held.
enum bongo_cat_event {
    BONGO_CAT_EVENT_NONE,
    BONGO_CAT_EVENT_LEFT_DOWN,
    BONGO_CAT_EVENT_LEFT_UP,
    BONGO_CAT_EVENT_RIGHT_DOWN,
    BONGO_CAT_EVENT_RIGHT_UP,
};

struct bongo_cat_key_state {
    bool pressed[KEY_MAX + 1];
};

static inline enum bongo_cat_event bongo_cat_key_event(
    struct bongo_cat_key_state *state,
    unsigned int side_counts[2],
    unsigned short code,
    int value
) {
    if (code > KEY_MAX || (value != 0 && value != 1)) return BONGO_CAT_EVENT_NONE;

    const unsigned int side = key_is_left(code) ? 0 : 1;
    if (value == 1) {
        if (state->pressed[code]) return BONGO_CAT_EVENT_NONE;
        state->pressed[code] = true;
        if (side_counts[side]++ != 0) return BONGO_CAT_EVENT_NONE;
        return side == 0 ? BONGO_CAT_EVENT_LEFT_DOWN : BONGO_CAT_EVENT_RIGHT_DOWN;
    }

    if (!state->pressed[code]) return BONGO_CAT_EVENT_NONE;
    state->pressed[code] = false;
    if (--side_counts[side] != 0) return BONGO_CAT_EVENT_NONE;
    return side == 0 ? BONGO_CAT_EVENT_LEFT_UP : BONGO_CAT_EVENT_RIGHT_UP;
}

#endif
