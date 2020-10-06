# Light-Weight Process Handler

## Description
This project implements support for lightweight processes on a linux machine. The library is built using our Makefile, and creates liblwp.so. I also used two header files, lwp.h and scheduler.h in order to separate and organize the code. Scheduler.h and scheduler.c contain the round robin doubly linked list structure, as well as function definitions for the default scheduler. lwp.h and lwp.c on the other hand handle initializing the stack frame and calling the threads.


## Instructions
There are no additional libraries needed for this project. To compile and run LWP, type the following commands into a terminal:

make

./testProj
