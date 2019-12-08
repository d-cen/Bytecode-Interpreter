# ECE 30862 Project: Interpreter

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
### cmpe: 132, or 10000100
Compare the top two elements on the runtime stack and make the new top of the runtime
stack 1 if the elements are equal, and 0 otherwise.
rstack[sp-1] = rstack[sp-1] == rstack[sp]
sp--;


## Testing
Test your main.c by compiling it with the following command:
```
g++ -Wall -Werror -std=c++11 main.cpp -o main
```
