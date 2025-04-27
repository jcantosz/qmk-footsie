// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┐
     * │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │
     * └───┴───┴───┴───┴───┴───┘
     * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
     * https://johnmu.com/basic-qmk-keyboard/
     * https://docs.qmk.fm/porting_your_keyboard_to_qmk#direct-pin-matrix
     */
    [0] = LAYOUT_ortho_4x4(KC_LEFT_CTRL, KC_LEFT_ALT, KC_LEFT_GUI, KC_LEFT_SHIFT, KC_LEFT, KC_RIGHT)};
