# Programming STM8S Microcontrollers using Arduino IDE:

https://circuitdigest.com/microcontroller-projects/programming-stm8s-microcontrollers-using-arduino-ide


## Notes:

[1]  ̶W̶a̶r̶n̶i̶n̶g̶:̶ ̶c̶o̶n̶n̶e̶c̶t̶i̶n̶g̶ ̶t̶h̶e̶ ̶S̶T̶-̶L̶i̶n̶k̶ ̶V̶2̶ ̶t̶o̶ ̶t̶h̶e̶ ̶u̶C̶ ̶m̶a̶y̶ ̶d̶a̶m̶a̶g̶e̶ ̶t̶h̶e̶ ̶N̶R̶S̶T̶ ̶i̶n̶p̶u̶t̶.̶.
- For some reason the NRST pin was not able to be kept in a high state (5V) via a pull-up resistor. I found that there was no 1.8V on the VCAP pin, I tried putting a pull-up resistor between the VDD (5V) and the VCAP, after putting the resistor (4k7) on the VCAP, the NRST signal returned to normal, and the VCAP now has 1.8V.

[2] When uploading code via Arduino IDE, the original program will be erased forever, be sure what you are doing.

Reference:

![alt text](https://circuitdigest.com/sites/default/files/inlineimages/u2/Arduino-Pin-Mapping-for-STM8S103F3.png?raw=true)


## Licence:

Software:

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
