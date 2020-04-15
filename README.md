# Project 2 - Part 1 CMSC 421: Tic-Tac-Toe

Implementation of a virtual character device that interacts with the file in user-space
with normal file read and write operations. 

## Approaches

The approaches to make this implementation work are as follow:

* building a clean copy of the 5.5 kernel. Then, configuring and compiling the kernel and its modules by installing packages, such as linux-libc-dev and linux-headers

* creating a folder named module in the place of the project repositor /usr/src/project2-part1.
  Then, adding the files: tictactoe.c and Makefile.

* initializing a module and cleaning up the module named my_start and my_end. 
  As the module implements a virtual character device, it is needed to register character device, create its class and device along with the major number.

* implementing file operations methods based on data structure in <linux/fs.h>.
  My approach consists of the implementation of all four operations: open, release, read and write.

* Most challenging part is to implement read operation which returns the size of the response commands. 
  Checking always the data sent in by the user for validity and parsing those commands for execution. 
  And in write operation which return the same length given in the function in forms of bytes.
 

## References
https://linux.die.net/lkmpg/index.html
https://www.youtube.com/watch?v=CMMyrfoKPNQ&feature=youtu.be
https://www.csee.umbc.edu/~jtang/archives/cs421.f19/lectures/L18DeviceDrivers.pdf
