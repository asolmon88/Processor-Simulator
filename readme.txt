Description

This processor is a 4-way superscalar, pipelined, Out-of-Order
processor with static branch prediction. There are some test
programs are provided in the tests folder.

It has it's own instruction set, which is described in the file called
"instructions.pdf" in the documentation (doc) folder. This instruction
set is mostly based on the x86 processor.

One important thing to keep into a count, when creating recursive programs,
use the R0 as a calling convention. After calling the function you have to
save the result on the R0. Another very important thing is that, if you
want to create a program that uses arrays, please keep in mind that the
processor can only handle 32 integer arrays.

NOTE: The Makefile for this simulator is made for
UNIX based operating systems, linux or macOS.

Compilation

To compile the program, there is a makefile done for the processor.
Open up your terminal, navigate to the Processor-Simulator folder
and type the following:

make

That command should compile the program and create an executable "main".

Running

To run the program you can do 2 things. After compiling, type the following:

make run

This command will run the simulator and the default test program is the
sum.txt. If you want to change the test program then you can type the
following:

./main nameOfProgram

So, for instance, if you want to run the fibonacci test, then you would
type ./main fibonacci
