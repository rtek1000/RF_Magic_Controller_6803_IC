# Modified hardware:
- The RF signal input from the remote control is originally connected to the PB4 pin, but on this pin there is the I2C hardware module.
To expand the capacity of available pins, without needing much hardware change, I chose to use a PCF8574 port expander.
For this I modified (cut) the track of the RF receiver of the remote control and connected it to the PD4 pin of the uC STM8.

- To run sample code from this folder you must use modified hardware.

Reference:

![alt text](https://circuitdigest.com/sites/default/files/inlineimages/u2/Arduino-Pin-Mapping-for-STM8S103F3.png?raw=true)
