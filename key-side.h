#ifndef BONGO_CAT_KEY_SIDE_H
#define BONGO_CAT_KEY_SIDE_H

#include <linux/input.h>
#include <stdbool.h>

// Linux input codes describe physical key positions, so this stays correct for
// every layout (including non-QWERTY layouts) using a standard keyboard shape.
static inline bool key_is_left(unsigned short code) {
    switch (code) {
    case KEY_ESC:
    case KEY_GRAVE:
    case KEY_1:
    case KEY_2:
    case KEY_3:
    case KEY_4:
    case KEY_5:
    case KEY_TAB:
    case KEY_Q:
    case KEY_W:
    case KEY_E:
    case KEY_R:
    case KEY_T:
    case KEY_CAPSLOCK:
    case KEY_A:
    case KEY_S:
    case KEY_D:
    case KEY_F:
    case KEY_G:
    case KEY_LEFTSHIFT:
    case KEY_102ND:
    case KEY_Z:
    case KEY_X:
    case KEY_C:
    case KEY_V:
    case KEY_B:
    case KEY_LEFTCTRL:
    case KEY_LEFTMETA:
    case KEY_LEFTALT:
    case KEY_F1:
    case KEY_F2:
    case KEY_F3:
    case KEY_F4:
    case KEY_F5:
    case KEY_F6:
        return true;
    default:
        // Center and unrecognized keys deliberately follow the right-arm rule.
        return false;
    }
}

#endif
