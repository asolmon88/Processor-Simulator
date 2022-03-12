Description

This processor is on it's early stages. This is a non-pipelined, scalar
processor that computes simple or complex arithmetic programs. Some test
programs are provided in the tests folder.

It has it's own instruction set, which is described in the file called
"instructions.pdf" in the documentation (doc) folder.

One important thing to keep into a count, when creating recursive programs,
use the R0 as a calling convention. After calling the function you have to
save the result on the R0. This is undergoing work, you can see in the code
that there are parts meant for recursion but it's still not complete.

NOTE: The Makefile for this simulator is made for
UNIX based operating systems, linux or macOS.

Compilation

To compile the simulator just run the make command. On the
terminal, go to the folder where the makefile is, write "make" and
the code should compile.

Running

To run the program run the command run on the makefile. To
do, go to the folder where the makefile is, usind the terminal
on the terminal write "make run" and the program will run.

Things left to do:

*Incorporate recursion
*Other stages
