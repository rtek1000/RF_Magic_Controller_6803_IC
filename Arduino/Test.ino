/* 
 *  This sample code does the counting in hexadecimal
 *  
 *  Created by RTEK1000
 *  Date: 2022-07-16
 *  Repo.: https://github.com/rtek1000/RF_Magic_Controller_6803_IC/blob/main/Arduino/Teste.ino
 *  
 *  Here's how to program:
 *  https://circuitdigest.com/microcontroller-projects/programming-stm8s-microcontrollers-using-arduino-ide
 *  
 *  (Tested using the STM8S103F3 model selected in the Arduino IDE)
 *  
 */

#include"stm8s.h"

char c[5] = {36, 164, 164, 164, 0};

const char char_H = 17;
const char char_P = 25;
const char char_Off = 62;

// Characters found:
// 17: H
// 18: seg e
// 19: seg b, c, d
// 20: seg g
// 21: seg d, e, f "L"
// 23: seg a, b, c, e, f "N"
// 25: P
// 28: seg a, e
// 29: seg a, b, d, e, f
// 30: seg d, g
// 31: seg c, d, e, g "o"
// 34: seg a, g
// 36: seg a
// 37: seg a, b
// 38: seg a, b, c
// 39: seg a, b, c, d
// 40: seg a, b, c, d, e
// 41: seg a, b, c, d, e, f
// 42: seg a, b, c, d, e, f, g
// 62: all off
// 64: 0
// 65: 1
// 66: 2
// 67: 3
// 68: 4
// 69: 5
// 70: 6
// 71: 7
// 72: 8
// 73: 9
// 74: A
// 75: b
// 76: C
// 77: d
// 78: E
// 79: F
// 93: seg a, b, d, e, f "C'"

int cnt1 = 0;

char val_0_F(int x);
void print_4_dig_dec(int x);
void print_4_dig_hex(int x);
void send_string(char *str);
void setup (void);
void loop (void);

char val_0_F(int x) {
  return x + 64;
}

void print_4_dig_dec(int x) {
  int x1000 = x / 1000;
  x -= x1000 * 1000;
  int x100 = x / 100;
  x -= x100 * 100;
  int x10 = x / 10;
  x -= x10 * 10;
  int x1 = x;

  c[0] = val_0_F(x1000);
  c[1] = val_0_F(x100);
  c[2] = val_0_F(x10);
  c[3] = val_0_F(x1);

  send_string(c);
}

void print_4_dig_hex(int x) {
  int x1000 = x / 0x1000;
  x -= x1000 * 0x1000;
  int x100 = x / 0x100;
  x -= x100 * 0x100;
  int x10 = x / 0x10;
  x -= x10 * 0x10;
  int x1 = x;

  c[0] = val_0_F(x1000);
  c[1] = val_0_F(x100);
  c[2] = val_0_F(x10);
  c[3] = val_0_F(x1);

  send_string(c);
}

/*
 * 
 * Routine send_string source: https://github.com/tenbaht/sduino/blob/development/examples/uart-spl/uart_spl.c
 * 
 */ 

void send_string(char *str){
  char c;

  if (!str) return;

  while ( c = *str++ ) { // assignment intented
    while (!UART1_GetFlagStatus(UART1_FLAG_TXE));
    UART1_SendData8(c);
  }
}

void setup (void) {
  UART1_DeInit();
  UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  while (1) {
    send_string(c);

    if (c[0] < 42) {
      c[0]++;
      c[1]++;
      c[2]++;
      c[3]++;
    } else {
      break;
    }

    delay(100);
  }
}

void loop (void) {
  print_4_dig_hex(cnt1);

  if (cnt1 < 0x9999) {
    cnt1++;
  } else {
    cnt1 = 0;
  }

  delay(100);
}
