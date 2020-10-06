# TAR Archiver

## Description
This project implemented a simpler version of the built in TAR archiver in a linux environment. TAR is a software used for file compression, in which a file or directory is specified and added to an archive with a preceding header depending on the file information.


## Instructions
There are no additional libraries needed for this project. To compile and run, type the following commands into a terminal:

make

./mytar -fcvtxs [infile]


## Flags
-f: specify a file or directory to make an archive for
-c: create a new tar file to be output to
-v: verbose; print additional information while traversing directory
-t: make table to be output at the end of the .tar file
-x: extract a directory from an already existing .tar file
-s: include symbolic links in .tar file
