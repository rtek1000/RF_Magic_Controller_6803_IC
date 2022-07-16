# RF_Magic_Controller_6803_IC
RF magic Controller 6803 IC 133 Modes for Dream Color Chasing 5050 RGB LED Strip (Hardware hacking)

I was interested in using this device for other purposes, so I tried to find out which microcontroller (uC) is used on the controller board.

It appears to be an STM8S003F3 or similar, see the pinout found on the Internet. The GND, VCC, Vcap and NRST pins are in the same position as the board tracks.

The uC supply is 5V, and apparently the 4-digit display is controlled by some other microcontroller.
Apparently the display triggering communication is via serial, and when comparing with the STM8S003F3 pinout, pin 2 is the TX output of UART1.

Control board ICs and remote control board IC do not have the codes printed on the housing.

The remote control transmitter consumes approximately 2.4uA at rest with the 12V battery, model 23A. And when the button is pressed, the consumption is greater than 13mA.

The controller board has an IC that receives the signal from the remote control, this IC has a 6.7458MHz crystal. The signal is passed directly to the uC, like any 433MHz RF receiver. The signal enters pin 12 of the main uC.

For model reference only, this video shows the model and shape of the parts:
https://www.youtube.com/watch?v=5VTxYW1q9to
