// In order to shift a bit to the right we just add
// R5 to R5.

movi 5, 0x8fff
nop

add 5, 5, 5

end: halt
