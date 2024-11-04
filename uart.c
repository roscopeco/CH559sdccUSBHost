
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "uart.h"
#include "command.h"
#include "usbinput.h"
// uint8_t __xdata uartRxBuff[64];
// uint8_t __xdata rxPos = 0;

void processUart(void){
    while(RI){
		RI = 0;
		process_command(SBUF);
    }
}

void sendProtocolMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer){
    unsigned short i;
    putchar(0xFE);	
	putchar(length);
	putchar((unsigned char)(length>>8));
	putchar(msgtype);
	putchar(type);
	putchar(device);
	putchar(endpoint);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	for (i = 0; i < length; i++)
	{
		putchar(msgbuffer[i]);
	}
	putchar('\n');
}

void sendHidPollMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
	unsigned short i;

	switch (cmd_key_mode) {
	case IDENT_MODE_SCAN:
	case IDENT_MODE_ASCII:
		hid_handler(type, length, msgbuffer);
		break;
	case IDENT_MODE_USB:
		putchar(0xFE);	
		putchar(length);
		putchar((unsigned char)(length>>8));
		putchar(msgtype);
		putchar(type);
		putchar(device);
		putchar(endpoint);
		putchar(idVendorL);
		putchar(idVendorH);
		putchar(idProductL);
		putchar(idProductH);
		for (i = 0; i < length; i++)
		{
			putchar(msgbuffer[i]);
		}
		putchar('\n');
		break;
	}
}