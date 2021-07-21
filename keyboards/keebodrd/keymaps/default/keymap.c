#include "kb.h"

#define _BL 0
#define _XL 1
#define _FL 2
#define _NL 3
#define _SL 4

enum my_keymaps {
  KC_SHLK = SAFE_RANGE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BL] = LAYOUT( /* Std Layer */
    KC_GRV,              KC_1,      KC_2,    KC_3,    KC_4, KC_5,    KC_6,    KC_7,    KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
    KC_TAB,              KC_Q,      KC_W,    KC_E,    KC_R, KC_T,    KC_Y,    KC_U,    KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
    MT(MOD_LCTL,KC_ESC), KC_A,      KC_S,    KC_D,    KC_F, KC_G,    KC_H,    KC_J,    KC_K, KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
    KC_LSFT,             KC_NUBS,   KC_Z,    KC_X,    KC_C, KC_V,    KC_B,    KC_N,    KC_M, KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_DEL,
    KC_LCTL,             MO(_FL),   KC_LGUI, KC_LALT,       KC_SPC,  KC_SPC,  MO(_NL),                KC_RALT, KC_RCTL, KC_PGUP, KC_UP,   KC_PGDN,
		                                            KC_BTN1, KC_BTN3, KC_BTN2,                                  KC_LEFT, KC_DOWN, KC_RGHT),

  [_XL] = LAYOUT( /* Special Functions */
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______,          _______, _______, _______,                   _______, _______, _______, _______, _______,
                                                 XXXXXXX, XXXXXXX, XXXXXXX,                                     _______, _______, _______),

  [_FL] = LAYOUT( /* Function Layer */
    KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,     KC_F10,  KC_F11,  KC_F12,  _______,
    _______, _______, _______, _______, _______, _______, _______, KC_PAUS, KC_HOME, KC_SYSREQ, KC_SLCK, KC_MUTE, KC_VOLD, KC_VOLU,
    KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_BRID, KC_BRIU, _______,   _______, _______, _______, KC_INS,
    _______, _______, _______, _______,          _______, _______, MO(_SL),                     KC_RGUI, KC_APP,  KC_PSCR, _______, KC_END,
                                                 _______, _______, _______,                                       KC_MPRV, KC_MPLY, KC_MNXT),

  [_NL] = LAYOUT( /* Numpad Layer */
    KC_NLCK, _______, _______, _______, _______, _______, _______, KC_P7,   KC_P8,  KC_P9,   KC_PSLS, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_P4,   KC_P5,  KC_P6,   KC_PAST, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_P1,   KC_P2,  KC_P3,   KC_PMNS, _______, KC_PENT,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_P0,  KC_PPLS, KC_PDOT, _______, _______, _______,
    _______, MO(_SL), _______, _______,          _______, _______, _______,                  _______, _______, _______, _______, _______,
                                                 _______, _______, _______,                                    _______, _______, _______),

  [_SL] = LAYOUT( /* Special Functions */
    KC_PWR,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    KC_SHLK, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______,          _______, _______, _______,                   _______, _______, _______, _______, _______,
                                                 _______, TG(_XL), _______,                                     _______, _______, _______)

};

static led_t leds;
static uint16_t key_timer = 0;
static bool blink_enable = false;
static bool first_blink_enable = false;
static bool second_blink_enable = false;
static bool third_blink_enable = false;
uint16_t cur_time = 0;
void triple_led_blink(uint16_t cur_time);
void led_routines(void);
void deactivateD6(void);
void activateD6(void);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch(keycode) {
		case KC_SHLK:
			if(record->event.pressed) { // notice the order
				leds = host_keyboard_led_state();
				if(leds.scroll_lock) // BUG: scroll lock false
					third_blink_enable = true;
			        if(leds.caps_lock)
					second_blink_enable = true;
				if(leds.num_lock)
					first_blink_enable = true;
				blink_enable = true;
				deactivateD6();
				key_timer = timer_read();
			} else {
				// release action
			}
			return false;
			break;
		case KC_NLCK:
		case KC_SLCK:
		case KC_CAPS:
			if(record->event.pressed) { // notice the order
				third_blink_enable = true;
				second_blink_enable = true;
				first_blink_enable = true;
				blink_enable = true;
				deactivateD6();
				key_timer = timer_read();
			} else {
				// release action
			}
			return true;
			break;
		default:
			if(record->event.pressed) {
				writePinHigh(D6);
			} else { // when releasing
				writePinLow(D6);
			}
			return true;
	}
}

void deactivateD6(void) {
	if(readPin(D6)) writePinLow(D6);
}

void activateD6(void) {
	if(!readPin(D6)) writePinHigh(D6);
}

void triple_led_blink(uint16_t cur_time) {
	if(first_blink_enable) {
		switch(cur_time) {
			case 1 ... 150:
				activateD6();
				break;
			case 151 ... 300:
				first_blink_enable = false;
				deactivateD6();
				break;
		}
	}
	if(second_blink_enable) {
		switch(cur_time) {
			case 301 ... 450:
				activateD6();
				break;
			case 451 ... 600:
				second_blink_enable = false;
				deactivateD6();
				break;
		}
	}
	if(third_blink_enable) {
		switch(cur_time) {
			case 601 ... 750:
				activateD6();
				break;
			case 751 ... 900:
				third_blink_enable = false;
				deactivateD6();
				break;
		}
	}
	// reaches end of control only when over 900ms
	if(!first_blink_enable &&
			!second_blink_enable &&
			!third_blink_enable) {
		switch(cur_time) {
			case 901 ... 1050:
				deactivateD6();
				key_timer = 0;
				blink_enable = false;
				break;
		}
	}
}

void led_routines(void) {
	cur_time = timer_elapsed(key_timer);
	triple_led_blink(cur_time);
}

void matrix_scan_user(void) {
	if(blink_enable) led_routines();
}
