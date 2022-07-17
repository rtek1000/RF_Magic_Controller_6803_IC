/*
    This sample code does the counting in hexadecimal

    Created by RTEK1000
    Date: 2022-07-16
    Repo.: https://github.com/rtek1000/RF_Magic_Controller_6803_IC/blob/main/Arduino_Modified_Hardware/Test_remote_control_HM/Test_remote_control_HM.ino

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

void setup (void) {
  keyboard_init(); // External interrupt

  rf_ctrl_init();  // External interrupt + Timer 1

  display_init();  // Serial UART1
}
