# Micropro Arch Lab 1(&2): RISC16
16 Feb 2026: Lab 1  

## 1. Processor description

reg0 is special: read-only 0 value  
ALU: add, bitwise NAND, no modification, and operand comparison  
CTL: decodes opcode  
PCO: program counter  

blocks linked with buses of width 1,3,6,10 or 16bitss  
control signals  

instruction set has 8 elementary instructions  

### Instructions:
- ADD: add two registers and store in destination register  
- ADDI: add register and immediate value and store in destination register  
- NAND: bitwise NAND of two registers and store in destination register  
- LUI(Load Upper Immediate): load immediate 10bit constant sign extended into destination register  
- LW(Load Word): read a word at address regB+immidiate in memory and store in destination register (indirect addressing with offset)  
- SW(Store Word): write a word from source register to address regB+immidiate (indirect addressing with offset)  
- BEQ(Branch if EQual): compares two registers,if equal, PC is updated to PC_BEQ+1+immediate, else PC+1  
- JALR(Jump And Link usgin Register): jumps to address in a register, and writes PC+1 to another register (this is a call with a return address)  

### Pseudo-instructions

- NOP = ADD 0,0,0 : does nothing because reg0 is read-only  
- RESET = JALR 0,0 : changes PC to 0 (reset processor), registers and data memory remain unchanged!  
- MOVI... = LUI...; ADDI... : load a 16bit immediate constant into a register (uses 2 instructions)  
- HALT: stops the simulator  

### Execution

4 stages: IF (instruction fetch), ID/RF (instruction decode/register fetch), EX (execute), WR (write back)  
all instructions have the same EX time (20 half clock cycles)

## 2. Simulation dynamics

...  

## 3. The simulator

either write in the Assembly tab, or import external file "File import ROM"  

## 4. Questions - Manipulation

```risc16
		addi 	2,0,1 // immediate value 1 added with 0 into reg2
		sw   	2,1,0 // store word from reg2 into address reg1+immediate0
		addi 	1,1,1 // immediate value 1 added with reg1 into reg1
		sw   	2,1,0 // store word from reg2 into address reg1+immediate0
		addi 	1,1,1 // immediate value 1 added with reg1 into reg1
		add  	3,2,2 // add reg2 with reg2 into reg3
		sw   	3,1,0 // store word from reg3 into address reg1+immediate0
		addi 	1,1,1 // immediate value 1 added with reg1 into reg1
		addi 	7,0,7 // immediate value 7 added with 0 (reg0) into reg7
boucle: beq  	7,0,end // compares reg7 with reg0 (value0), if true, PC updated to value PC_BEQ+1+immediate(end), else next line (PC+1)
		lw	2,1,-2 // load word from address reg1+immediate(-2) into reg2
		add 	3,3,2 // add reg3 with reg2 into reg3
		sw   	3,1,0 // store word from reg3 into address reg1+immediate0
		addi 	1,1,1 // immediate value 1 added with reg1 into reg1
		addi 	7,7,-1 // immediate value -1 added with reg7 into reg7
		beq  	0,0,boucle // compares reg0 (value 0) with reg0 (value 0), if true, update to value PC_BEQ+1+immediate(@boucle), else next line (never happens, always true)
end:	halt // stops simulation
```

### Q1
**Explain what is the example on listing 1 on the preceding page doing? Detail the state of registers and the state of the PC after each instruction.**  



store 1 into reg2  
store reg2 value into data memory at address reg1+0, so 0x0000 so 0 => value 1 stored at address 0 of data memory => mem0 = 1  
add value 1 to reg1, so reg1 now has value 1 => reg1 = 1  
store reg2 value into data memory at address reg1+0, so 0x0001 so 1 => value 1 stored at address 1 of data memory => mem1 = 1  
add value 1 to reg1, so reg1 now has value 2 => reg1 = 2  
add reg2 value with itself and store in reg3, so reg3 now has value 2 => reg3 = 2  
store reg3 value into data memory at address reg1+0, so 0x0002 so 2 => value 2 stored at address 2 of data memory => mem2 = 2  
add value 1 to reg1, so reg1 now has value 3 => reg1 = 3  
add value 7 to reg0 (which is always 0) and store in reg7, so reg7 now has value 7 => reg7 = 7
compare reg7 with reg0, since they are not equal, PC is updated to next line => PC = 8, else go to end label (not taken)  
load word from address reg1+(-2) which is 3-1 so 0x0001 so 1 into reg2, so reg2 now has value 1 => reg2 = 1  
add reg3 value (2) with reg2 value (1) and store in reg3, so reg3 now has value 3 => reg3 = 3  
store reg3 value (3) into data memory at address reg1+0, so 3+0 so 0x0003 so 3 => value 3 stored at address 3 of data memory => mem3 = 3  
add value 1 to reg1, so reg1 now has value 4 => reg1 = 4  
add value -1 to reg7, so reg7 now has value 6 => reg7 = 6 => decrement reg7 by 1  
compare reg0 with reg0, since they are equal, PC is updated to value PC_BEQ+1+immediate(@boucle) which is 10+1-6 so 5 => PC = 5, else next line (not taken)  
AGAIN: compare reg7 with reg0, since they are not equal (6!=0), PC is updated to next line, repeat until end.  

```
store mem0 = 1
store mem1 = 1
store mem2 = 2 (reg3)
i = 7
while (i!=0) {
    get mem1 (1) into reg2,
    // => reg2 is mem(-1) value
    reg3 is mem2 value (2),
    // => reg3 is mem(0) value
    store mem3 = reg2 (mem1) + reg3 (mem2) = 3,
    // => mem(1) value is mem(-1) value + mem(0) value
    // reg1 is incremented by 1, so next mem address is accessed
    i--
}
```

_**this is Fibonacci sequence**_  
in C:  
```c
void main() {
    int[10] mem;
    mem[0] = 1;
    mem[1] = 1;
    mem[2] = 2;
    for (i=7; i!=0; i--) {
        a = mem[i-2];
        b = mem[i-1];
        mem[i] = a+b;
    }
}
```
### Q2
**Load exemple1.txt and run the simulation. Explain the internal behaviour for each instruction.**  

explained above  

### Q3
**Using the graph in annexe 3 on page 13, draw the chronogram for the BEQ instruction. Signals on the graph are output of blocks of the processor. For each of the exercises 4–9, you can automatically test your code against a set of test vectors (listed in section 6) using the “Online Verification Tool”. Read this handout to the end to get a glimpse on its capabilities and to understand how to use it.**  

forward by half cycles the simulator and see the orange and green parts and lines on the simulator  

### Q4
**Write a program which shifts to the left the content of reg5.**  

```risc16
        addi 	5,0,1 // load value 1 into reg5, initial value to be shifted
        //movi 	5,0xF34A // load value 0xF34A into reg5, initial value to be shifted
        addi 	6,0,0 // load value 0 into reg6 (counter)
        addi 	7,0,16 // load value 16 into reg7 (number of bits to shift)
loop:   beq 	6,7,end // if counter equals number of bits to shift, end loop
        add 	5,5,5 // shift left by adding reg5 to itself
        addi 	6,6,1 // increment counter
        beq 	0,0,loop // repeat loop
end:    halt // stop simulation
```
We shift left by one bit by adding the register to itself (equivalent to multiplying by 2). We repeat this process x(=16) times to shift left by x(=16) bits, value x put in reg7. The counter in reg6 keeps track of how many shifts we have done for the loop, and we compare it to reg7 to know when to stop.  

### Q5
**Write a program which extracts the most significant bit from reg1 and stores the value (0/1) in reg7.**  

In risc16, the MSB of a 16-bit register is the bit at position 15 (counting from 0). To extract it, we can shift the register left by 15 bits, which will move the MSB to the least significant bit position. After shifting, we can store the result in reg7. If the original MSB was 1, reg7 will be 1; if it was 0, reg7 will be 0.  

_VERIFY: !!!_  
```risc16
        addi 	7,0,0 // initialize reg7 to 0 (default value if MSB is 0)
        //addi 	  1,0,0xFFFF // initialize value 0xFFFF into reg1, which its MSB is 1
        //movi    1,0x063F // load value 0x063F into reg1, which its MSB is 0
        addi 	6,0,15 // load value 15 into reg6 (number of bits to shift)
loop:   beq 	6,0,end // if counter equals 0, end loop
        add 	1,1,1 // shift left by adding reg1 to itself <---- this does the left shifting of reg1
        addi 	6,6,-1 // decrement counter
        beq 	0,0,loop // repeat loop
//end:    addi 	7,7,0 // reg7 now contains the MSB of original reg1 (after shifting left 15 times)
end:    add 	7,0,1 // reg7 now contains the MSB of reg1 (aka the value of of reg1 after shifting left 15 times)
        halt // stop simulation
```


------------

# Micropro Arch Lab 2: RISC16
xx Feb 2026: Lab 2  

## 4. Questions - Manipulation (continued)

### Q6
**Write a program which shifts to the left a 32-bit value stored in reg6(MSB), reg5.**  

...  


