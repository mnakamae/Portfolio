# Function Generator

## Description
The goal of this project was to recreate a digital multimeter using the MSP432. Our ADC14 sampled the input at the specified rates below in the table. This project was fairly straightforward, since the ADC14 transmitted sampled data through the UART to communicate between the MSP432 board and the Analog Discovery 2. This sampled data was then interpreted to give a float value voltage reading from the input.


## Materials
- LaunchPad MSP432P401R Board
- MCP4912
- Digilent Analog Discovery 2


## Instructions
This project was entirely completed in Code Composer Studio. Plug the MSP432 board and card reader into the USB-A ports of the computer. Then build the project and run the code on the board.
