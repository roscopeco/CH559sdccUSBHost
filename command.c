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

#include "CH559.h"
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

void init_state(void) {
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
            cmd_have_mouse = true;
            putchar(CMD_ACK);
            break;
        case CMD_MOUSE_STRM_ON:
            cmd_have_mouse = true;
            cmd_enable_mouse_reports = true;
            putchar(CMD_ACK);
            break;
        case CMD_MOUSE_STRM_OFF:
                cmd_enable_mouse_reports = false;
                putchar(CMD_ACK);
            break;

        // TODO not supported
        case CMD_MOUSE_REPORT:
        case CMD_SPI_ENABLE:
        case CMD_SPI_DISABLE:
            putchar(CMD_ACK);
            break;

        case CMD_RESET:
            putchar(CMD_ACK);
            WDOG_COUNT = 0xff;      // Reset on next watchdog tick...
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
            putchar(CAPABILITIES);
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
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_POWGRN:
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_POWBLU:
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_CAPS:
            cmd_uart_caps_led_on = byte > 0;
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_DISK:
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_EXTRED:
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_EXTGRN:
            putchar(CMD_ACK);
            current_command = 0;
            break;
        case CMD_LED_EXTBLU:
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
