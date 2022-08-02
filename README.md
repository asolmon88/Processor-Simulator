# Processor-Simulator

This is a coursework where each student had to create a program that simulates a modern superscalar, Out-of-Order processor.

## Features

This processor features the following components:

- Pipelined execution with 3 stages: Fetch,Decode, Execute
- 4-way superscalar
- Out-of-Order execution
  - Scoreboard
- Static Branch Prediction
- 3 execution units
  - ALU
  - Load/Store Unit
  - Branch Unit
- Unified Reservation Station
- Latency for: multiply (2), compare (2), branches (1 or 2), load (4)

## How to Program

It has it's own instruction set, which is described in the file called "instructions.pdf" in the [documentation](./doc/) folder. This instruction set is mostly based on the x86 processor. On the [tests](./tests/) folder there are various examples of programs that can be run in this processor.

One important thing to keep into a count, when creating recursive programs, use the R0 as a calling convention. After calling the function you have to save the result on the R0. Another very important thing is that, if you want to create a program that uses arrays, please keep in mind that the processor can only handle 32 integer arrays.

**NOTE:** The Makefile for this simulator is made for
UNIX based operating systems, linux or macOS.

## Compilation

To compile the program, there is a makefile done for the processor. Open up your terminal, navigate to the Processor-Simulator folder and type the following:

```
$ make
```

That command should compile the program and create an executable "main".

## Running the Program

To run the program you can do 2 things. After compiling, type the following:

```
$ make run
```

This command will run the simulator and the default test program is the sum.txt. If you want to change the test program then you can type the
following:

```
$ ./main nameOfProgram
```

So, for instance, if you want to run the fibonacci test, then you would type

```
$ ./main fibonacci
```