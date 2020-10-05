# Digital Password Lock

## Description
This lock is a keypad lock that starts off in the locked stage. Once the correct 4 digit pin has been entered the lock unlocks. Once in the unlocked stage, the user can press the # button to reset their pin and will be prompted to insert a new 4 digit pin. After the new pin has been inserted, the lock will lock. In the unlocked stage pressing any button besides the ‘#’ will return the user to the locked stage with the same pin. If the user inserts the wrong pin 3 times in a row they will get put in timeout for 10 seconds before being brought back to the locked stage to attempt guesses at the pin. Lastly in the locked stage if the user presses the ‘*’ it will clear their input and will not count as an attempt. 

## Materials
- LaunchPad MSP432P401R Board
- Jumper Wire M/F 6” 20 Pieces
- Switch Keypad 12 Key Non-ILLUM
- NHD-C0220AA-FSW-FTW LCD Display
- 830 Point Solderless Breadboard 3PCS


## Instructions
This project was entirely completed in Code Composer Studio. Plug the MSP432 board into the USB-A ports of the computer. Then build the project and run the code on the board.
