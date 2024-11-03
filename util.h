#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdio.h>
#if 0
#define DEBUG_OUT(...) printf(__VA_ARGS__);
#else
#define DEBUG_OUT(...) (void)0;
#endif

void initClock();
void initWatchdog(uint8_t initial);
void delayUs(unsigned short n);
void delay(unsigned short n);
void initUART0(unsigned long baud, int alt);
unsigned char UART0Receive();
void UART0Send(unsigned char b);

// The following routines require timer 0 to be initialized...
// Timer resolution is 10ms...
void initTimer0(void);

// Get current system ticks
uint32_t getSystemTicks(void);

// Calculate elapsed milliseconds since start tick
uint32_t getElapsedMs(uint32_t startTick);

#define PIN_MODE_INPUT 0
#define PIN_MODE_INPUT_PULLUP 1
#define PIN_MODE_OUTPUT 2
#define PIN_MODE_OUTPUT_OPEN_DRAIN 3
#define PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK 4
#define PIN_MODE_INPUT_OUTPUT_PULLUP 5
#define PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK 6
void pinMode(unsigned char port, unsigned char pin, unsigned char mode);

typedef void(* __data FunctionReference)();
extern FunctionReference runBootloader;

#endif