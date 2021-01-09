#include "simpletools.h"                      // Include simple tools

#include "fdserial.h"
#define MSG_TYPE_CONNECTED      0x01
#define MSG_TYPE_DISCONNECTED   0x02
#define MSG_TYPE_ERROR          0x03
#define MSG_TYPE_DEVICE_POLL    0x04
#define MSG_TYPE_DEVICE_STRING  0x05
#define MSG_TYPE_DEVICE_INFO    0x06
#define MSG_TYPE_HID_INFO       0x07
#define MSG_TYPE_STARTUP        0x08


unsigned char  uartRxBuff[1024];
int  rxPos = 0;
int  cmdLength = 0;
unsigned char  cmdType = 0;


fdserial *usbhost;

const char *deviceType[] = {"UNKNOWN", "POINTER", "MOUSE", "RESERVED", "JOYSTICK", "GAMEPAD", "KEYBOARD", "KEYPAD", "MULTI_AXIS", "SYSTEM"};


void filterCommand(int buffLength, unsigned char *msgbuffer);

int main()
{
  pause(1500);
  print("Ready.\r");

  usbhost = fdserial_open(17, -1, 0b0000, 400000);

  while(1) {
    while (1) {
      uartRxBuff[rxPos] = fdserial_rxChar(usbhost);
      if (uartRxBuff[rxPos] != -1) {  
        if (rxPos == 0 && uartRxBuff[rxPos] == 0xFE) {
          cmdType = 1;
        } else if (rxPos == 1 && cmdType == 1) {
          cmdLength = uartRxBuff[rxPos];
        } else if (rxPos == 2 && cmdType == 1) {
          cmdLength += (uartRxBuff[rxPos] << 8);
        } else if (cmdType == 0 && uartRxBuff[rxPos] == '\n') {
          rxPos = 0;
          cmdType = 0;
          break;
        }
        if (rxPos > 0 && rxPos == cmdLength + 11 && cmdType || rxPos > 1024) {
          filterCommand(cmdLength, uartRxBuff);
          rxPos = 0;
          cmdType = 0;
          break;
        } else {
          rxPos++;
        }
      }               
    }
    rxPos = 0;
  
  }
}




void filterCommand(int buffLength, unsigned char *msgbuffer) {
  int cmdLength = buffLength;
  unsigned char msgType = msgbuffer[3];
  unsigned char devType = msgbuffer[4];
  unsigned char device = msgbuffer[5];
  unsigned char endpoint = msgbuffer[6];
  unsigned char idVendorL = msgbuffer[7];
  unsigned char idVendorH = msgbuffer[8];
  unsigned char idProductL = msgbuffer[9];
  unsigned char idProductH = msgbuffer[10];
  switch (msgType) {
    case MSG_TYPE_CONNECTED:
      print("Connection port %d\r", device);
      break;
    case MSG_TYPE_DISCONNECTED:
      print("Disconnection port %d\r", device);
      break;
    case MSG_TYPE_ERROR:
      print("%s error port %d\r", deviceType[devType], device);
      break;
    case MSG_TYPE_DEVICE_POLL:
      print("%s Data port: %d, %d bytes, ID: ", deviceType[devType], device, cmdLength);
        for (int j = 0; j < 4; j++) {
          print("%02x", msgbuffer[j + 7]);
        }
        print(", ");
        for (int j = 0; j < cmdLength; j++) {
          print("%02x", msgbuffer[j + 11]);
        }
        print("\r");
      break;
    case MSG_TYPE_DEVICE_STRING:
      print("String from port %d: ", devType);
      for (int j = 0; j < cmdLength; j++) {
        print("%c", msgbuffer[j + 11]);
      }
      print("\r");
      break;
    case MSG_TYPE_DEVICE_INFO:
      print("Info from port %d: ", device);
      for (int j = 0; j < cmdLength; j++) {
        print("0x%02x ", msgbuffer[j + 11]);
      }
      print("\r");
      break;
    case MSG_TYPE_HID_INFO:
      print("HID info from port %d: ", device);
      for (int j = 0; j < cmdLength; j++) {
        print("0x%02x ", msgbuffer[j + 11]);
      }
      print("\r");
      break;
    case MSG_TYPE_STARTUP:
      print("USB host ready\r");
      break;
  }
}
