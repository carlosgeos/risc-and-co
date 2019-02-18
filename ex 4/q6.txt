// Input:
movi 6, 0x0001
//nop
movi 5, 0xffff
//nop

// prepare registers for carry verification (See q5, same logic)
movi 2, 0x8000
nop
movi 3, 0xffff
nop
nand 4, 2, 5

// Shift both R5, R6 registers
add 6, 6, 6
add 5, 5, 5

// if carry add one bit at the LSB of R6:
if: beq 4, 3, endif
addi 6, 6, 1
endif: halt
