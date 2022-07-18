# Modified hardware:
- The RF signal input from the remote control is originally connected to the PB4 pin, but on this pin there is the I2C hardware module.
To expand the capacity of available pins, without needing much hardware change, I chose to use a PCF8574 port expander.
For this I modified (cut) the track of the RF receiver of the remote control and connected it to the PD4 pin of the uC STM8.

- To run sample code from this folder you must use modified hardware.

- Note: Unfortunately I still couldn't get the I2C port to work correctly, there are some posts that say they had problems with I2C on these STM8S, and in my case the I2C_GetFlagStatus() function seems to be unable to read the register and so the routine freezes.
- I2C port ref.:
- - https://www.youtube.com/watch?v=gR7oqUz4CW4
- - https://github.com/timypik/STM8S-Library/blob/master/libstm8s/src/stm8s_i2c.c

Reference:

![alt text](https://circuitdigest.com/sites/default/files/inlineimages/u2/Arduino-Pin-Mapping-for-STM8S103F3.png?raw=true)

# Note:
- The Test_remote_control_HM folder contains the code separated into parts, to try to improve the readability
