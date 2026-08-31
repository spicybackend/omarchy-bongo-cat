#include "../key-state.h"

#include <assert.h>

int main(void) {
    struct bongo_cat_key_state first_keyboard = {0};
    struct bongo_cat_key_state second_keyboard = {0};
    unsigned int side_counts[2] = {0, 0};

    assert(key_is_left(KEY_5));
    assert(key_is_left(KEY_T));
    assert(key_is_left(KEY_G));
    assert(key_is_left(KEY_B));
    assert(key_is_left(KEY_LEFTSHIFT));
    assert(key_is_left(KEY_102ND));
    assert(key_is_left(KEY_F6));
    assert(!key_is_left(KEY_6));
    assert(!key_is_left(KEY_Y));
    assert(!key_is_left(KEY_H));
    assert(!key_is_left(KEY_N));
    assert(!key_is_left(KEY_BACKSPACE));
    assert(!key_is_left(KEY_SPACE));
    assert(!key_is_left(KEY_RIGHTALT));
    assert(!key_is_left(KEY_F7));

    assert(key_counts_toward_wpm(KEY_A));
    assert(key_counts_toward_wpm(KEY_SPACE));
    assert(key_counts_toward_wpm(KEY_KP7));
    assert(!key_counts_toward_wpm(KEY_LEFTCTRL));
    assert(!key_counts_toward_wpm(KEY_LEFT));
    assert(!key_counts_toward_wpm(KEY_BACKSPACE));

    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_A, 1)
        == BONGO_CAT_EVENT_LEFT_DOWN);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_S, 1)
        == BONGO_CAT_EVENT_NONE);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_Y, 1)
        == BONGO_CAT_EVENT_RIGHT_DOWN);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_A, 2)
        == BONGO_CAT_EVENT_NONE);
    assert(bongo_cat_key_event(&second_keyboard, side_counts, KEY_A, 1)
        == BONGO_CAT_EVENT_NONE);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_A, 0)
        == BONGO_CAT_EVENT_NONE);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_S, 0)
        == BONGO_CAT_EVENT_NONE);
    assert(bongo_cat_key_event(&second_keyboard, side_counts, KEY_A, 0)
        == BONGO_CAT_EVENT_LEFT_UP);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_Y, 0)
        == BONGO_CAT_EVENT_RIGHT_UP);
    assert(bongo_cat_key_event(&first_keyboard, side_counts, KEY_N, 0)
        == BONGO_CAT_EVENT_NONE);
    return 0;
}
