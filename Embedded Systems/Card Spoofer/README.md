# Card Spoofer

## DESCRIPTION
This project is a card emulator that sends a card’s magnetic strip (1st and 2nd tracks only) as a string through the UART to the MSP432. The MSP432 then generates electrical outputs that drive an optoisolator which in turn drives our inductor. Magnetic stripes are read through the changes in the surrounding magnetic field around the card reader. The voltages output from the MSP432 induced changes in the magnetic field near the card reader, which effectively tricks the card reader into scanning a “card”. As a result, the card reader will read the information sent from our inductor and write it to a text document.

## Materials
- LaunchPad MSP432P401R Board

- Jumper Wire M/F 6” 20 Pieces

- Switch Keypad 12 Key Non-ILLUM

- USB Swipe Magnetic Credit Card Reader

- 830 Point Solderless Breadboard 3PCS

- 22 AWG Magnet Wire - Enameled Copper Wire

- Logic Gate Optocoupler


## Instructions
This project was entirely completed in Code Composer Studio. Plug the MSP432 board and card reader into the USB-A ports of the computer. Then build the project and run the code on the board.
