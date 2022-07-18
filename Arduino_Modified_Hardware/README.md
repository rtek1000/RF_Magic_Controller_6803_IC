# Modified hardware:
- The RF signal input from the remote control is originally connected to the PB4 pin, but on this pin there is the I2C hardware module.
To expand the capacity of available pins, without needing much hardware change, I chose to use a PCF8574 port expander.
For this I modified (cut) the track of the RF receiver of the remote control and connected it to the PD4 pin of the uC STM8.

- To run sample code from this folder you must use modified hardware.

- Note: Unfortunately I still couldn't get the I2C port to work correctly, there are some posts that say they had problems with I2C on these STM8S, and in my case the I2C_GetFlagStatus() function seems to be unable to read the register and so the routine freezes. The problem seems to be the lack of interrupt, because the initialization of the I2C module does not trigger interrupt events, so asking the code to wait for an interrupt, the code is stuck forever. Basically I found two initializations, one that activates the interrupt and one that does not activate. The arduino core for STM8S I'm using should be the one that doesn't activate the interrupt:
- - https://github.com/timypik/STM8S-Library/blob/master/libstm8s/inc/stm8s_i2c.h
- This other file shows the activation of the interrupt on line 88:
- - https://github.com/HomeSmartMesh/STM8_IoT_HelloWorld/blob/master/18_STM8L_HelloI2C/i2c.c

- I2C port ref.:
- - https://www.youtube.com/watch?v=gR7oqUz4CW4
- - https://github.com/timypik/STM8S-Library/blob/master/libstm8s/src/stm8s_i2c.c
- - https://cdn.hackaday.io/files/278671192261728/AN3281%20Application%20note%20STM8%20I2C%20optimized%20examples.pdf

Reference:

![alt text](https://circuitdigest.com/sites/default/files/inlineimages/u2/Arduino-Pin-Mapping-for-STM8S103F3.png?raw=true)

# Note:
- The Test_remote_control_HM folder contains the code separated into parts, to try to improve the readability
