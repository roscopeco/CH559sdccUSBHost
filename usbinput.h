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
 * USB HID Input Driver
 * ------------------------------------------------------------
 */

#ifndef __ROSCO_USB_INPUT_H
#define __ROSCO_USB_INPUT_H

#include <stdint.h>

void hid_handler(uint8_t type, uint16_t payload_len, uint8_t *payload);

#endif