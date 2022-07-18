/*
    This sample code does the MCP4725 test

    Created by RTEK1000
    Date: 2022-07-18
    Repo.: https://github.com/rtek1000/RF_Magic_Controller_6803_IC/blob/main/Arduino_Modified_Hardware/dac_test.ino

    Here's how to program:
    https://circuitdigest.com/microcontroller-projects/programming-stm8s-microcontrollers-using-arduino-ide

    (Tested using the STM8S103F3 model selected in the Arduino IDE)

*/

#include"stm8s.h"
#include "stm8s_i2c.h"

#define MCP4725_Addr 0x62
#define uC_Addr 0x1
#define I2C_Speed 100000

void setup (void);
void loop (void);
void I2C_WriteRegister2(u8 u8_regAddr, u8 u8_NumByteToWrite, u8 *u8_DataBuffer);
void I2C_Setup(u8 u8_regAddr);

void setup (void) {
    I2C_Setup(uC_Addr); // 
}

void loop (void) {
  u8 buff[2];

  while (1) {
    for (int i = 0; i < 4095; i++) {

      buff[0] = (i >> 8) & 0x0F;
      buff[1] = i & 0xFF;

      I2C_WriteRegister2(MCP4725_Addr, 2, buff);
    }
  }
}

/******************************************************************************
  Function name : I2C_WriteRegister2
  Description   : write defined number bytes to slave memory starting with defined offset
  Input param   : offset in slave memory, number of bytes to write, starting address to send
  Return        : None.
  See also      : i2c_master_poll.c - STM8 optimized I2C examples (AN3281)
*******************************************************************************/
void I2C_WriteRegister2(u8 u8_regAddr, u8 u8_NumByteToWrite, u8 *u8_DataBuffer)
{
  uint8_t I2C_Slave_Address = u8_regAddr << 1; // from 8 bits to 7 bits + R/W

  I2C_GenerateSTART(ENABLE);
  while ((I2C->SR1 & 1) == 0); // Wait for start bit detection (SB)

  I2C_Send7bitAddress(I2C_Slave_Address, I2C_DIRECTION_TX);
  while (!(I2C->SR1 & 2));     // Wait for address ack (ADDR)
  (void)I2C->SR3; // ADDR=1, cleared by reading SR1 register followed by reading SR3

  while (u8_NumByteToWrite--)
  { // write data loop start
    while (!(I2C->SR1 & 0x80));                 // test EV8 - wait for TxE
    I2C_SendData(*u8_DataBuffer++);                           // send next data byte
  }

  while ((I2C->SR1 & 0x84) != 0x84); // Wait for TxE & BTF

  I2C_GenerateSTOP(ENABLE);
  while (I2C->CR2 & 2); // wait until stop is performed
}

void I2C_Setup(u8 u8_regAddr)
{
  GPIO_DeInit(GPIOB);

  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST);

  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);

  I2C_DeInit();

  I2C_Init(I2C_Speed, u8_regAddr, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, (CLK_GetClockFreq() / 1000000));

  // I2C->ITR = 1;                      // enable error interrupts
  //  I2C1_ITR_ITEVTEN = 1;          //Event  Enables               : SB, ADDR, ADD10, STOPF, BTF, WUFH
  //  I2C1_ITR_ITBUFEN = 1;          //Buffer Enables (if ITEVTEN)  : RXNE, TXE
  //  I2C1_ITR_ITERREN = 1;          //Error  Enables               : BERR, ARLO, AF, OVR

  I2C_Cmd(ENABLE);
}
