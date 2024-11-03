/*
 *------------------------------------------------------------
 *                                  ___ ___ _   
 *  ___ ___ ___ ___ ___       _____|  _| . | |_ 
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_| 
 *                     |_____|           devices
 * ------------------------------------------------------------
 * Copyright (c)2024 The rosco_m68k OSP
 * See top-level LICENSE.md for licence information.
 *
 * rosco_m68k Keyboard command processor
 * ------------------------------------------------------------
 */

/*
 * rosco_m68k keyboard microcontroller code (command.cpp)
 *
 * Copyright (c)2023-2024 The Really Old-School Company Limited
 *
 * Released to open source: 2024-10-20; MIT License
 */

#include <stdint.h>
#include <stdbool.h>

#define __COMPILE_COMMAND_C

#include "command.h"

static uint8_t current_command;

uint8_t cmd_key_mode;
bool cmd_i2c_mode;
bool cmd_enable_mouse_reports;
bool cmd_uart_caps_led_on;
bool cmd_have_mouse;
uint16_t cmd_repeat_delay;
uint8_t cmd_repeat_rate_limit;

int putchar(int c);

static inline void led_set(const uint8_t led_mask, uint8_t brightness) {
    // TODO
}

static inline void pow_set(const uint8_t led_mask, uint8_t brightness) {
    // TODO
}

void init_state() {
    cmd_key_mode = IDENT_MODE_ASCII;
    cmd_i2c_mode = false;
    cmd_enable_mouse_reports = false;
    cmd_uart_caps_led_on = false;
    cmd_have_mouse = false;
    cmd_repeat_delay = 500;
    cmd_repeat_rate_limit = 100;
}

void process_command(int byte) {
    if (byte < 0) {
        return;
    }

    if (current_command == 0) {
        switch (byte) {
        // starting a command
        case CMD_LED_POWRED:
        case CMD_LED_POWGRN:
        case CMD_LED_POWBLU:
        case CMD_LED_CAPS:
        case CMD_LED_DISK:
        case CMD_LED_EXTRED:
        case CMD_LED_EXTGRN:
        case CMD_LED_EXTBLU:
        case CMD_MODE_SET:
        case CMD_RPT_DELAY_SET:
        case CMD_RPT_RATE_SET:
        case CMD_MOUSE_SET_RATE:
        case CMD_MOUSE_SET_RES:
        case CMD_MOUSE_SET_SCALE:
            current_command = byte;
            putchar(CMD_ACK);
            break;
        case CMD_MOUSE_DETECT:
            // if (!i2c_mode) {
            //     if (mouse.initialise() == 0) {  // 0 == ok, else timeout
            //         have_mouse = true;
            //         putchar(CMD_ACK);
            //     } else {
            //         putchar(CMD_NAK);
            //     }
            // } else {
            //     putchar(CMD_NAK);
            // }
            putchar(CMD_NAK);
            break;
        case CMD_MOUSE_STRM_ON:
            // if (!i2c_mode && !uart_mode) {
            //     if (mouse.initialise() == 0) {  // 0 == ok, else timeout
            //         have_mouse = true;
            //         enable_mouse_reports = true;
            //         putchar(CMD_ACK);
            //     } else {
            //         // In case we're reinitializing after unplug and haven't noticed yet...
            //         have_mouse = false;
            //         enable_mouse_reports = false;
            //         putchar(CMD_NAK);
            //     }
            // } else {
            //     putchar(CMD_NAK);
            // }
            putchar(CMD_NAK);
            break;
        case CMD_MOUSE_STRM_OFF:
                cmd_enable_mouse_reports = false;
                putchar(CMD_ACK);
            break;
        case CMD_MOUSE_REPORT:
            // if (i2c_mode) {
            //     putchar(CMD_NAK);
            // } else {
            //     if (!have_mouse) {
            //         if (mouse.initialise() != 0) {  // 0 == ok, else timeout
            //             putchar(CMD_NAK);
            //             break;
            //         }

            //         have_mouse = true;
            //     }

            //     if (service_mouse()) {
            //         putchar(CMD_ACK);
            //     } else {
            //         putchar(CMD_NAK);
            //     }
            // }
            putchar(CMD_NAK);

            break;
#ifdef REVISION_2
        case CMD_SPI_ENABLE:
            if (uart_mode) {
                putchar(CMD_NAK);
            } else {
                enable_spi_reports = true;
                putchar(CMD_ACK);
            }
            break;
        case CMD_SPI_DISABLE:
            enable_spi_reports = false;
            putchar(CMD_ACK);
            break;
#endif
        case CMD_RESET:
            putchar(CMD_ACK);
            // wdt_enable(WDTO_15MS);
            while (1);
        case CMD_IDENT:
            putchar('r');
            putchar('o');
            putchar('s');
            putchar('c');
            putchar('o');
            putchar('_');
            putchar('k');
            putchar('b');
            putchar('d');
            putchar(cmd_key_mode);            
            putchar(KEY_COUNT);
            putchar(LED_COUNT);

            if (cmd_i2c_mode) {
                putchar(CAPABILITIES | CAP_I2C);
            } else {
                putchar(CAPABILITIES | CAP_PS2);
            }

            putchar((uint8_t)0);
            putchar((uint8_t)0);
            putchar(CMD_ACK);
            break;
        default:
            putchar(CMD_NAK);
        }
    } else {
        // operand
        switch (current_command) {
        case CMD_LED_POWRED:
            pow_set(POW_RED, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_POWGRN:
            pow_set(POW_GRN, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_POWBLU:
            pow_set(POW_BLU, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_CAPS:
            led_set(LED_CAPS, byte);
            cmd_uart_caps_led_on = byte > 0;
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_DISK:
            led_set(LED_DISK, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_EXTRED:
            led_set(LED_EXTRED, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_EXTGRN:
            led_set(LED_EXTGRN, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_EXTBLU:
            led_set(LED_EXTBLU, byte);
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_MODE_SET:
            switch (byte) {
            case CMD_MODE_SCAN:
                cmd_key_mode = IDENT_MODE_SCAN;
                putchar(CMD_ACK);
                break;
            case CMD_MODE_ASCII:
                cmd_key_mode = IDENT_MODE_ASCII;
                putchar(CMD_ACK);
                break;
            case CMD_MODE_USB:
                cmd_key_mode = IDENT_MODE_USB;
                putchar(CMD_ACK);
                break;
            default:
                putchar(CMD_NAK);
                break;
            }
            current_command = 0;
            break;
        case CMD_RPT_DELAY_SET:
            cmd_repeat_delay = byte * 10;
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_RPT_RATE_SET:
            if (byte == 0) {
                putchar(CMD_NAK);
            } else {
                cmd_repeat_rate_limit = (uint8_t)(256 - byte);
                putchar(CMD_ACK);
            }
            current_command = 0;
            break;            
        case CMD_MOUSE_SET_RATE:
            if (cmd_have_mouse) {
                switch (byte) {
                case 10:
                case 20:
                case 40:
                case 60:
                case 80:
                case 100:
                case 200:
                    // TODO wat do?
                    putchar(CMD_ACK);
                    break;
                default:
                    putchar(CMD_NAK);
                }
            } else {
                putchar(CMD_NAK);
            }
            current_command = 0;
            break;
        case CMD_MOUSE_SET_RES:
            if (cmd_have_mouse) {
                if (byte >= 0 && byte < 5) {
                    // TODO wat do?
                    putchar(CMD_ACK);
                } else {
                    putchar(CMD_NAK);
                }
            } else {
                putchar(CMD_NAK);
            }
            current_command = 0;
            break;
        case CMD_MOUSE_SET_SCALE:
            if (cmd_have_mouse) {
                switch (byte) {
                case CMD_MOUSE_SCL_11:
                    // TODO wat do?
                    putchar(CMD_ACK);
                    break;
                case CMD_MOUSE_SCL_21:
                    // TODO wat do?
                    putchar(CMD_ACK);
                    break;
                default:
                    putchar(CMD_NAK);
                }
            } else {
                putchar(CMD_NAK);
            }
            current_command = 0;
            break;
        default:
            putchar(CMD_NAK);
            current_command = 0;
        }
    }
}
