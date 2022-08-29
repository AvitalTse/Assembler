# Assembler32bit
A program that imitates the way the assembler works and is designed for a specific 32bit central processing unit.
"Hardware": 
Note:
The description of the computer model below is only partial, apparently to perform the tasks in the project. the assembly language and the model against the imaginary computer, for this project.
The computer in the project consists of a CPU (central processing unit), registers and memory
 RAM. Part of the memory is also used as a cartridge.
and the 9th bit is most significant as a number. Register names are always written with a lowercase letter. The size of each register is 10 bits. The least effective bit will be specified as bit number 0, for the processor 8 general registers, named: r7,r6,r5,r4,r3,r2,r1,r0.

The processor also has a register called PSW (word status program), which contains a number of characterizing flags
This calculator only works with positive and negative integers, no real support. A cell in memory is also called a "word".
The bits in each word are numbered like a register. The size of the memory is 256 cells, at addresses 0-255) in decimal base), and each cell is 10 bits in size. The use of these flags. the state of activity in the processor at any given moment. See below, in the description of the machine's instructions, explanations, regarding the arithmetic is done using the 2's complement method. There is also support for characters (characters, represented in the ascii code.
