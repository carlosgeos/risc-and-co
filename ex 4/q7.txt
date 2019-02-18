// inputs
movi 1, 0x0000
//nop
movi 2, 0x0001
//nop

// prepare data for carry verification. Use memory address 0, 1
movi 7, 0x8000
nop
sw 7, 0, 0
movi 7, 0xffff
nop
sw 7, 0, 1

// Calculates the sum and use save it in memory address 2
add 7, 1, 2
sw 7, 0, 2

// Nand of registers 1, 2 with 0x8000 and save it in memory address 3, 4
lw 7, 0, 0
nand 7, 7, 1
sw 7, 0, 3
lw 7, 0, 0
nand 7, 7, 2
sw 7, 0, 4

// Save flag 1 if R1 and R2 MBS is 1. Save it in memory address 5, 6
// R1
movi 6, 0xffff
nop
lw 7, 0, 3
beq 6, 7, flagoff_r1
movi 7, 1
nop
beq 0, 0, endflagoff_r1
flagoff_r1: movi 7, 0
endflagoff_r1: sw 7, 0, 5

// R2
lw 7, 0, 4
beq 6, 7, flagoff_r2
movi 7, 1
nop
beq 0, 0, endflagoff_r2
flagoff_r2: movi 7, 0
endflagoff_r2: sw 7, 0, 6

// sum up both carries if value is 2 then there is carry
// if value is 1 then check for MBS = 0
// else no carry
// load carry flag of R1 and R2 into R6 and R7
lw 6, 0, 5
lw 7, 0, 6
// load one bit in R5 in order to compare
add 7, 7, 6

// movi R6 <- 2
movi 6, 2

// if carry sum = 2
beq 7, 6, carry
beq 0, 0, continue_1
carry: movi 4, 1
beq 0, 0, end

// move R6 <- 1
continue_1: movi 6, 1
nop

//if carry sum = 1
beq 7, 6, carry_1_verifier
beq 0, 0, no_carry
// load sum in R5
carry_1_verifier: lw 5, 0, 2
// mask
movi 6, 0x8000
nop
nand 6, 6, 5
movi 5, 0xffff
nop
beq 6, 5, carry
beq 0, 0, no_carry

no_carry: movi 4, 0
end: lw 3, 0, 2
halt
