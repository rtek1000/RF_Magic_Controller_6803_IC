/*
    This sample code does the counting in hexadecimal

    Created by RTEK1000
    Date: 2022-07-16
    Repo.: https://github.com/rtek1000/RF_Magic_Controller_6803_IC/blob/main/Arduino/Test_keyboard.ino

    Here's how to program:
    https://circuitdigest.com/microcontroller-projects/programming-stm8s-microcontrollers-using-arduino-ide

    (Tested using the STM8S103F3 model selected in the Arduino IDE)

*/

#include"stm8s.h"

char c[5] = {36, 164, 164, 164, 0};

const char char_H = 17;
const char char_P = 25;
const char char_Off = 62;

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

/*
   Not Connected:
   PD1 SWIM (ST-Link)
   PB5 D3
   PD4 D13
*/

/*
   RF input:
   PB4 D4

*/

/*
   Output:
   PD3 D12
   PD2 D11

*/

/*
   Keyboard pinout
   GND
   PA3: D2
   PA2: D1
   PA1: D0
   PC3: D5
   PC4: D6
   PC5: D7
   PC6: D8
   PC7: D9

*/

#define key_0 2
#define key_1 1
#define key_2 0
#define key_3 5
#define key_4 6
#define key_5 7
#define key_6 8
#define key_7 9

bool key_0_old = true;
bool key_1_old = true;
bool key_2_old = true;
bool key_3_old = true;
bool key_4_old = true;
bool key_5_old = true;
bool key_6_old = true;
bool key_7_old = true;

unsigned int cnt1 = 0;

char val_0_F(int x);
void print_4_dig_dec(uint32_t x);
void print_4_dig_hex(uint32_t x);
void send_string(char *str);
void setup (void);
void loop (void);

char val_0_F(int x) {
  return x + 64;
}

void print_4_dig_dec(uint32_t x) {
  uint8_t x1000 = x / 1000;
  x -= x1000 * 1000;
  uint8_t x100 = x / 100;
  x -= x100 * 100;
  uint8_t x10 = x / 10;
  x -= x10 * 10;
  uint8_t x1 = x;

  c[0] = val_0_F(x1000);
  c[1] = val_0_F(x100);
  c[2] = val_0_F(x10);
  c[3] = val_0_F(x1);

  send_string(c);
}

void print_4_dig_hex(uint32_t x) {
  uint8_t x1000 = x / 0x1000;
  x -= x1000 * 0x1000;
  uint8_t x100 = x / 0x100;
  x -= x100 * 0x100;
  uint8_t x10 = x / 0x10;
  x -= x10 * 0x10;
  uint8_t x1 = x;

  c[0] = val_0_F(x1000);
  c[1] = val_0_F(x100);
  c[2] = val_0_F(x10);
  c[3] = val_0_F(x1);

  send_string(c);
}

/*

   Routine send_string source: https://github.com/tenbaht/sduino/blob/development/examples/uart-spl/uart_spl.c

*/

void send_string(char *str) {
  char c;

  if (!str) return;

  while ( c = *str++ ) { // assignment intented
    while (!UART1_GetFlagStatus(UART1_FLAG_TXE));
    UART1_SendData8(c);
  }
}

void setup (void) {
  pinMode(key_0, INPUT_PULLUP);
  pinMode(key_1, INPUT_PULLUP);
  pinMode(key_2, INPUT_PULLUP);
  pinMode(key_3, INPUT_PULLUP);
  pinMode(key_4, INPUT_PULLUP);
  pinMode(key_5, INPUT_PULLUP);
  pinMode(key_6, INPUT_PULLUP);
  pinMode(key_7, INPUT_PULLUP);

  //  pinMode(14, OUTPUT); // Serial TX
  //
  //  digitalWrite(14, HIGH);
  //
  //  delay(5000);

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

  //  if (cnt1 < 0x9999) {
  //    cnt1++;
  //  } else {
  //    cnt1 = 0;
  //  }

  if ((digitalRead(key_0) == LOW) && (key_0_old == true)) { // Key Length +
    key_0_old = false;
    cnt1 += 0x1;
  } else if (digitalRead(key_0) == HIGH) {
    key_0_old = true;
  }
  
  if ((digitalRead(key_1) == LOW) && (key_1_old == true)) { // Key Length -
    key_1_old = false;
    cnt1 -= 0x1;
  } else if (digitalRead(key_1) == HIGH) {
    key_1_old = true;
  }
  
  if ((digitalRead(key_2) == LOW) && (key_2_old == true)) { // Key Quick
    key_2_old = false;
    cnt1 += 0x10;
  } else if (digitalRead(key_2) == HIGH) {
    key_2_old = true;
  }
  
  if ((digitalRead(key_3) == LOW) && (key_3_old == true)) { // Key Program -
    key_3_old = false;
    cnt1 -= 0x10;
  } else if (digitalRead(key_3) == HIGH) {
    key_3_old = true;
  }
  
  if ((digitalRead(key_4) == LOW) && (key_4_old == true)) { // Key Program +
    key_4_old = false;
    cnt1 += 0x100;
  } else if (digitalRead(key_4) == HIGH) {
    key_4_old = true;
  }
  
  if ((digitalRead(key_5) == LOW) && (key_5_old == true)) { // Key Slow
    key_5_old = false;
    cnt1 -= 0x100;
  } else if (digitalRead(key_5) == HIGH) {
    key_5_old = true;
  }
  
  if ((digitalRead(key_6) == LOW) && (key_6_old == true)) { // Key Power
    key_6_old = false;
    cnt1 += 0x1000;
  } else if (digitalRead(key_6) == HIGH) {
    key_6_old = true;
  }
  
  if ((digitalRead(key_7) == LOW) && (key_7_old == true)) { // Key Pause
    key_7_old = false;
    cnt1 -= 0x1000;
  } else if (digitalRead(key_7) == HIGH) {
    key_7_old = true;
  }

  if (cnt1 >= 0x10000) {
    cnt1 = 0;
  }

  delay(100);
}
