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

// WPM is based on printable key-downs. Navigation, modifiers, and editing
// commands are intentionally excluded; one word is the conventional five keys.
static inline bool key_counts_toward_wpm(unsigned short code) {
    switch (code) {
    case KEY_GRAVE:
    case KEY_1: case KEY_2: case KEY_3: case KEY_4: case KEY_5:
    case KEY_6: case KEY_7: case KEY_8: case KEY_9: case KEY_0:
    case KEY_MINUS: case KEY_EQUAL:
    case KEY_Q: case KEY_W: case KEY_E: case KEY_R: case KEY_T:
    case KEY_Y: case KEY_U: case KEY_I: case KEY_O: case KEY_P:
    case KEY_LEFTBRACE: case KEY_RIGHTBRACE: case KEY_BACKSLASH:
    case KEY_A: case KEY_S: case KEY_D: case KEY_F: case KEY_G:
    case KEY_H: case KEY_J: case KEY_K: case KEY_L:
    case KEY_SEMICOLON: case KEY_APOSTROPHE:
    case KEY_102ND:
    case KEY_Z: case KEY_X: case KEY_C: case KEY_V: case KEY_B:
    case KEY_N: case KEY_M: case KEY_COMMA: case KEY_DOT: case KEY_SLASH:
    case KEY_SPACE:
    case KEY_KP0: case KEY_KP1: case KEY_KP2: case KEY_KP3: case KEY_KP4:
    case KEY_KP5: case KEY_KP6: case KEY_KP7: case KEY_KP8: case KEY_KP9:
    case KEY_KPDOT:
        return true;
    default:
        return false;
    }
}

#endif
