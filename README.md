# RF_Magic_Controller_6803_IC
RF magic Controller 6803 IC 133 Modes for Dream Color Chasing 5050 RGB LED Strip (Hardware hacking)

I was interested in using this device for other purposes, so I tried to find out which microcontroller (uC) is used on the controller board.

It appears to be an STM8S003F3 or similar, see the pinout found on the Internet. The GND, VCC, Vcap and NRST pins are in the same position as the board tracks.

The uC supply is 5V, and apparently the 4-digit display is controlled by some other microcontroller.
Apparently the display triggering communication is via serial, and when comparing with the STM8S003F3 pinout, pin 2 is the TX output of USART1.

