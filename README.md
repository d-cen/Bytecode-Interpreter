# Project: Interpreter
ECE 30862: Object Oriented Programming C++ and Java

## Description
In this project, the bytecode interpreter will read a bytecode file produced by the compiler, and execute it. The program consists of bytecodes and data that follows certain bytecodes. The first byte of the program contains the initial bytecode in the main routine. In general, execution proceeds by executing the byte code pointed to by the PC, updating the PC based on the instruction executing, and continuing until the program executes. The halt instruction will terminate the execution of the program.

## Data Structures
### A runtime stack (rstack)
All operations are executed against values in the runtime stack. All variables exist on the runtime stack. Each datum takes exactly one position in the runtime stacks. Since data are of different lengths, this is accomplished by having the stack be represented by a vector, where each element of the vector points to an object holding the actual data item. The object should have a field to hold the value of the data item, and a field to hold the type of the data items. Data items are of type char, short, int or float. The stack is initially empty.

### A runtime stack pointer (sp)
This points to the top of the runtime stack. Its value should initially be -1.

### A stack of frame pointers (fpstack)
This is a stack of indices into the start of the frame for a procedure call. It points to the first element in the functions stack frame.

### A frame pointer stack pointer (fpsp)
This points to the top of the frame stack. Its initial value should be -1.

### A program counter (pc)
The program counter contains the address of the next instruction to be executed. Its value is initially zero.

### Program memory (mem)
Program memory holds the program that is read into the interpreter. It is an array of bytes.

## Instructions
### Comparison Bytecodes
#### cmpe: 132, or 10000100
Compare the top two elements on the runtime stack and make the new top of the runtime stack 1 if the elements are equal, and 0 otherwise.

#### cmplt: 136, or 10001000
Compare the top two elements on the runtime stack and make the new top of the runtime stack 1 if the next to the top element is less than the top element, and 0 otherwise.

#### cmpgt: 140, or 10001100
Compare the top two elements on the runtime stack and make the new top of the runtime stack 1 if the next to the top element is greater than the top element, and 0 otherwise.

### Control Flow Bytecodes
#### jmp: 36, or 00100100
Jump to the location at the top of the runtime stack.

#### jmpc: 40, or 00101000
Jump to the location at the top of the runtime stack is the next to the top of the runtime stack
contains the integer value 1 (true)

#### call: 44, or 00101100
Save the frame stack pointer for the current frame in the fpstack (frame pointer stack). Jump to the location of the function being called, whose address is on the top of the runtime stack.

#### ret: 48, or 00110000
Restore the runtime stack pointer of the function being returned to. Set the PC to the value at the top of the runtime stack, which is the address of the instruction following the call or callr statement.

### Stack Manipulation Bytecodes
#### pushc: 68, or 01000100
Push a character literal onto the top of the runtime stack.

#### pushs: 69 or 01000101
Push a short literal onto the top of the runtime stack.

#### pushi: 70 or 01000110
Push an integer literal onto the top of the runtime stack.

#### pushf: 71 or 01000111
Push a float literal onto the top of the runtime stack.

#### pushvc: 72, or 01001000
Push a character variable’s value (where the variable location is at the top of the runtime stack) onto the runtime stack.

#### pushvs: 73, or 01001001
Push a short variable’s value (where the variable location is at the top of the runtime stack) onto the runtime stack.

#### pushvi: 74, or 01001010
Push an integer variable’s value (where the variable location is at the top of the runtime stack) onto the runtime stack.

#### pushvf: 75, or 01001011
Push a floating point variable’s value (where the variable location is at the top of the runtime stack) onto the runtime stack.

#### popm: 76, or 01001100
Pop multiple entries off of the runtime stack, discarding their values. The number of entries to pop is at the top of the runtime stack.

#### popv: 80, or 01010000
Pop a value off of the runtime stack into a variable. The variable’s location is given by the top of the stack, the value popped is the next element into the stack.

#### popa: 77, or 01001101
Pop all of the top entries to frame stack point from the runtime stack but keep val top entries. The number of entries to keep is at the top of the runtime stack.

#### peekc: 84 or 01011000
Take the character value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and put it into the variable whose address is given by the next to the top element of the runtime stack.

#### peeks: 85 or 01011001
Take the short value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and put it into the variable whose address is given by the next to the top element of the runtime stack.

#### peeki: 86 or 01011010
Take the integer value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and put it into the variable whose address is given by the next to the top element of the runtime stack.

#### peekf: 87 or 01011011
Take the float value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and put it into the variable whose address is given by the next to the top element of the runtime stack.

#### pokec: 88 or 01100000
Change the character value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and to the variable whose address is given by the next to the top element of the runtime stack.

#### pokes: 89 or 01100001
Change the short value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and to the variable whose address is given by the next to the top element of the runtime stack.

#### pokei: 90 or 01100010
Change the integer value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and to the variable whose address is given by the next to the top element of the runtime stack.

#### pokef: 91 or 01100011
Change the float value at the offset (given by the value of the top of the stack element) from the start of the current runtime stack frame and to the variable whose address is given by the next to the top element of the runtime stack.

#### swp: 94 or 01100100
Swap the top of the stack with the next to the top of the stack element.

### Arithmetic Bytecodes
#### add: 100, or 01100100

#### sub: 104, or 01101000

#### mul: 108, or 01101100

#### div: 112, or 01110000

### Special Opcodes
#### printc, 144, or 10010000. . Print the character at the top of the stack

#### prints, 145, or 10010001. Print the short at the top of the stack

#### printi, 146, or 10010010. Print the integer at the top of the stack

#### printf, 147, or 10010011. Print the float at the top of the stack.

#### halt, 0, or 00000000
Terminate the program. Print pc, sp, rstack, fpsp, fpstack. Print empty if a stack is empty. 

## Testing
Test your main.c by compiling it with the following command:
```
g++ -Wall -Werror -std=c++11 main.cpp -o main
```
