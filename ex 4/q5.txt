// The idea is to mask the number with 0x8000 and use the nand command
// if the result is 0xffff then the MSB will be 0
// else it will be 1.

// init R1 with some value
movi 1, 0xffff
nop

// create mask
movi 2, 0x8000
nop

// save 0xffff in R3 to use it later
movi 3, 0xffff

// apply mask and save result into R4
nand 4, 2, 1

// if result is eq. to 0xffff then R7 <- 1 else R7 <- 0
if: beq 4, 3, else
addi 7, 0, 1
beq 0, 0, endif
else: addi 7, 0, 0
endif: halt
