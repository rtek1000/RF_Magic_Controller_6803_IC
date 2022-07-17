# RF_Magic_Controller_6803_IC
RF magic Controller 6803 IC 133 Modes for Dream Color Chasing 5050 RGB LED Strip (Hardware hacking)

![alt text](https://raw.githubusercontent.com/rtek1000/RF_Magic_Controller_6803_IC/main/Description.jpg?raw=true)

I was interested in using this device for other purposes, so I tried to find out which microcontroller (uC) is used on the controller board. Control board ICs and remote control board IC do not have the codes printed on the housing.

It appears to be an ![STM8S003F3](https://www.st.com/en/microcontrollers-microprocessors/stm8s003f3.html) or similar, see the pinout found on the Internet. The VSS (GND), VDD, VCAP and NRST pins are in the same position as the board tracks.

![alt text](https://raw.githubusercontent.com/rtek1000/RF_Magic_Controller_6803_IC/main/stm8s003f3.png?raw=true)

The uC supply is 5V, and apparently the 4-digit display is controlled by some other microcontroller.
Apparently the display triggering communication is via serial, and when comparing with the STM8S003F3 pinout, pin 2 is the TX output of UART1.
- Async Serial 9600 bauds
- - Display 'H''0''0''1': uC sends 17, 128, 128, 129 (4 bytes, values in decimal, seemingly no gap between data sends)
- - Display 'H''0''1''7': uC sends 17, 128, 129, 135
- - Display 'H''1''3''3': uC sends 17, 129, 131, 131
- - Display 'P''P''P''P': uC sends 25, 25, 25, 25 (Interval between sends: 2 seconds)
- - Display '5''0''0''1': uC sends 5, 128, 128, 129
- - Display '5''0''2''8': uC sends 5, 128, 130, 136
- - Display '5''1''0''0': uC sends 5, 129, 128, 128
- - Display ' '' '' '' ' (All segments disabled): uC sends '>', '>', '>', '>' (ASCII 62, interval between sends: 2 seconds) 

- Display initialization sequence:
- - uC sends '$', 164, 164, 164 ('$': ASCII 36)
- - uC sends '%', 165, 165, 165 ('%': ASCII 37)
- - uC sends '&', 166, 166, 166 ('&': ASCII 38)
- - uC sends ''', 167, 167, 167 (''': ASCII 39)
- - uC sends '(', 168, 168, 168 ('(': ASCII 40)
- - uC sends ')', 169, 169, 169 (')': ASCII 41)
- - uC sends '\*', 170, 170, 170 ('*': ASCII 42)
- - uC sends '1', '\t', '0', '3' ('1': ASCII 49; '\t': ASCII 9; '0': ASCII 48; '3': ASCII 51)

Output data appears to be being sent at an incredible 2.4 Mbps. The signal seems to be inverted, with idle logic at 0V (low state), different from the display communication which has high state idle logic. Data capture (display, output control and also the signal from the remote control) are in the Logic folder. Note: Output data is only from the control module, it was not connected to the LED strip

The remote control transmitter consumes approximately 2.4uA at rest with the 12V battery, model 23A (It is recommended not to leave the 12V battery in the remote control transmitter when storing for more than a few days, the battery powers a voltage regulator directly). And when the button is pressed, the consumption is greater than 13mA.

![alt text](https://raw.githubusercontent.com/rtek1000/RF_Magic_Controller_6803_IC/main/Remote_control.png?raw=true)

The controller board has an IC that receives the signal from the remote control, this IC has a 6.7458MHz crystal. The signal is passed directly to the uC, like any 433MHz RF receiver. The signal enters pin 12 of the main uC.

![alt text](https://raw.githubusercontent.com/rtek1000/RF_Magic_Controller_6803_IC/main/Remote_control2.png?raw=true)

For reference only, this video shows the shape of the parts:

![RGB-DC99 Dreamcolor Chasing Color Changing RGB Controller & Light Strip SEDC series](https://www.youtube.com/watch?v=fhb3GJJXOHg)
