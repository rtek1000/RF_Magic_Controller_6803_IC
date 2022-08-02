/*
    This sample code does the counting in hexadecimal

    Created by RTEK1000
    Date: 2022-07-16
    Repo.: https://github.com/rtek1000/RF_Magic_Controller_6803_IC/blob/main/Arduino/Test_keyboard.ino

    Here's how to program:
    https://circuitdigest.com/microcontroller-projects/programming-stm8s-microcontrollers-using-arduino-ide

    (Tested using the STM8S103F3 model selected in the Arduino IDE)

    [Solved] BUG: Only the ON/OFF button worked well, the other buttons do not have the same performance.
    - 2022-07-17: Added timer counter
    - 2022-07-17: Added external interrupt
    - 2022-07-17: Added return (exit) for resync

    Other modifications:
    - 2022-07-17: Added interruption for keyboard

    - 2022-07-17: Relocated RF signal input from remote control, from PB4 to PD4.
    Note: The RF signal input from the remote control is originally connected to the PB4 pin,
          but on this pin there is the I2C hardware module. To expand the capacity of available pins,
          without needing much hardware change, I chose to use a PCF8574 port expander.
          For this I modified (cut) the track of the RF receiver of the remote control and
          connected it to the PD4 pin of the uC STM8.

    - 2022-07-17: Added display initialization routine, to improve (a little) code reading
    - 2022-07-17: Added keyboard initialization routine, to improve (a little) code reading
    - 2022-07-17: Added rf remote control initialization routine, to improve (a little) code reading
*/

#include"stm8s.h"

// ref.: https://www.stm32duino.com/viewtopic.php?t=719
#define digitalPinToInterrupt(p)  ((p)==(p)) // workaround-fix, empty macro is missing in header files

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

#define Serial_TX 14 // PD5

/*
   Not Connected:
   PD1 SWIM (ST-Link)
   PB5 D3
   PD4 D13
*/

/*
   RF input:
   PB4 D4

   Changed to PD4 D13

*/

//#define RF_in 4
#define RF_in 13

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

volatile long cnt1 = 0;

volatile unsigned long millis1 = 0;

volatile bool RF_in_new = false;
volatile bool RF_in_old = false;

volatile int _cnt_bits_rf = 0;

volatile unsigned long _data_bits_rf = 0;
volatile unsigned long _data_bits_rf2 = 0;

volatile bool flag = false;

volatile unsigned long _const_data_addr_rf = 0;
volatile unsigned int _data_rf = 0;

volatile unsigned long _width_us = 0;
volatile unsigned long _interval_width_us = 0;
volatile unsigned long _interval_base_us = 0;

// Remote control patterns:
//         0000000000111111 111122222
//         0123456789012345 678901234
// ON/OFF: 0101010101010101 110000000
// PAUSE:  0101010101010101 001100000
// B+:     0101010101010101 001111000
// B-:     0101010101010101 110011000
// S+:     0101010101010101 000011000
// S-:     0101010101010101 111100000
// M+:     0101010101010101 000000110
// M-:     0101010101010101 111111000
// T: 1.570ms
// 0: 0.3985ms
// 1: 1.185ms
// Interval: 12.29ms

const unsigned long _const_data_addr = 0B0101010101010101;
const unsigned int _const_data_on_off =  0B110000000;
const unsigned int _const_data_pause =   0B001100000;
const unsigned int _const_data_b_plus =  0B001111000;
const unsigned int _const_data_b_minus = 0B110011000;
const unsigned int _const_data_s_plus =  0B000011000;
const unsigned int _const_data_s_minus = 0B111100000;
const unsigned int _const_data_m_plus =  0B000000110;
const unsigned int _const_data_m_minus = 0B111111000;

char val_0_F(int x);
void print_4_dig_dec(uint32_t x);
void print_4_dig_hex(uint32_t x);
void send_string(char *str);
void key_interr (void);
void display_init(void);
void keyboard_init(void);
void rf_ctrl_init(void);
void setup (void);
void loop (void);

char val_0_F_dig0(int x) {
  return x + 64;
}

char val_0_F(int x) {
  return x + 64; //0x80;
}

void print_4_dig_dec(uint32_t x) {
  uint8_t x1000 = x / 1000;
  x -= x1000 * 1000;
  uint8_t x100 = x / 100;
  x -= x100 * 100;
  uint8_t x10 = x / 10;
  x -= x10 * 10;
  uint8_t x1 = x;

  if (x1000 == 0) x1000 = 62;
  c[0] = x1000;
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

  if (x1000 == 0) x1000 = 62;
  c[0] = x1000;
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

void RF_interr(void) {
  _width_us = TIM1_GetCounter(); // step: 0.5us
  TIM1_SetCounter(0);

  if (digitalRead(RF_in) == HIGH) {
    if (_cnt_bits_rf != 25) {
      _cnt_bits_rf++;

      _interval_width_us = micros() - _interval_base_us;

      if (_interval_width_us > 5000) { // Interval (resync)
        _data_bits_rf = 0;
        _cnt_bits_rf = 0;
        return;
      }
    }
  } else { // if (digitalRead(RF_in) == LOW) {
    _interval_base_us = micros();

    // 1185us * 2 = 2370us
    // 2370us + 150us = 2520us
    // 2370us - 150us = 2220us
    // 398us * 2 = 796us
    // 796us + 150us = 946us
    // 796us - 150us = 646us

    if (_width_us > 2220 ) {
      if (_width_us < 2520) {
        _data_bits_rf = (_data_bits_rf << 1) + 1;
      } else {
        _data_bits_rf = 0; // (resync)
        _cnt_bits_rf = 0;
        return;
      }
    } else if (_width_us < 946) {
      if (_width_us > 646) {
        _data_bits_rf = _data_bits_rf << 1;
      } else {
        _data_bits_rf = 0; // (resync)
        _cnt_bits_rf = 0;
        return;
      }
    } else {
      _data_bits_rf = 0; // (resync)
      _cnt_bits_rf = 0;
      return;
    }

    _data_bits_rf2 = _data_bits_rf;

    _const_data_addr_rf = _data_bits_rf2 >> 9;

    if (_const_data_addr_rf == _const_data_addr) {
      _data_rf = _data_bits_rf2 & 0x1FF;

      if (_data_rf == _const_data_on_off) { // Key Power
        cnt1 += 0x1000;
      } else if (_data_rf == _const_data_pause) { // Key Pause
        cnt1 -= 0x1000;
      } else if (_data_rf == _const_data_b_plus) { // Key B+ (Key Program +)
        cnt1 += 0x100;
      } else if (_data_rf == _const_data_b_minus) { // Key B- (Key Program -)
        cnt1 -= 0x10;
      } else if (_data_rf == _const_data_s_plus) { // Key S+ (Key Quick)
        cnt1 += 0x10;
      } else if (_data_rf == _const_data_s_minus) { // Key S- (Key Slow)
        cnt1 -= 0x100;
      } else if (_data_rf == _const_data_m_plus) { // Key M+ (Key Length +)
        cnt1 += 0x1;
      } else if (_data_rf == _const_data_m_minus) { // Key M- (Key Length -)
        cnt1 -= 0x1;
      }

      if (cnt1 > 0xFFFF) {
        cnt1 = 0;
      } else if (cnt1 < 0) {
        cnt1 = 0xFFFF;
      }

      _data_bits_rf = 0;
      _cnt_bits_rf = 0;
    }
  }
}

void key_interr (void) {
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
}

void display_init(void) {
  char init1[2] = {0xFF, 0};

  UART1_DeInit();
  UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  pinMode(Serial_TX, OUTPUT);

  send_string(init1);

  delay(42);

  send_string(init1);
  send_string(init1);

  delay(90);

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

  delay(600);
}

void keyboard_init(void) {
  pinMode(key_0, INPUT_PULLUP);
  pinMode(key_1, INPUT_PULLUP);
  pinMode(key_2, INPUT_PULLUP);
  pinMode(key_3, INPUT_PULLUP);
  pinMode(key_4, INPUT_PULLUP);
  pinMode(key_5, INPUT_PULLUP);
  pinMode(key_6, INPUT_PULLUP);
  pinMode(key_7, INPUT_PULLUP);

  // ref.: https://www.stm32duino.com/viewtopic.php?t=719
  GPIO_Init(GPIOA, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1, GPIO_MODE_IN_PU_IT);
  GPIO_Init(GPIOC, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_IN_PU_IT);

  disableInterrupts();
  EXTI_SetExtIntSensitivity( EXTI_PORT_GPIOA, EXTI_SENSITIVITY_RISE_FALL);
  EXTI_SetExtIntSensitivity( EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_FALL);
  enableInterrupts();

  attachInterrupt(INT_PORTA & 0xFF, key_interr, CHANGE); // Group: GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1
  attachInterrupt(INT_PORTC & 0xFF, key_interr, CHANGE); // Group: GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7
}

void rf_ctrl_init(void) {
  pinMode(RF_in, INPUT);

  // ref.: https://www.stm32duino.com/viewtopic.php?t=719
  // GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_IT);
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_FL_IT);
  disableInterrupts();
  //EXTI_SetExtIntSensitivity( EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
  EXTI_SetExtIntSensitivity( EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
  enableInterrupts();

  // attachInterrupt(INT_PORTB & 0xFF, RF_interr, CHANGE);
  attachInterrupt(INT_PORTD & 0xFF, RF_interr, CHANGE);

  // https://sites.google.com/site/klaasdc/stm8s-projects/rpm-counter-1
  TIM1_DeInit();
  TIM1_TimeBaseInit(7, TIM1_COUNTERMODE_UP, 65535, 0);
  TIM1_Cmd(ENABLE);

  // https://community.st.com/s/question/0D50X0000AlgLvtSQE/tim1getcounter-results
  // TIM1_SetCounter(0);
  // TIM1_GetCounter();
}

void setup (void) {
  keyboard_init(); // External interrupt

  rf_ctrl_init();  // External interrupt + Timer 1

  display_init();  // Serial UART1
}

void loop (void) {
  if ((millis() - millis1) >= 250) {
    millis1 = millis();

    if (cnt1 > 0xFFFF) {
      cnt1 = 0;
    } else if (cnt1 < 0) {
      cnt1 = 0xFFFF;
    }

    print_4_dig_hex(cnt1);
  }
}
