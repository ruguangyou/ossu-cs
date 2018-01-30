// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// for(i=0,sum=0; i<R1; i++)
// {
//     sum = sum + R0;
// }

    @0
    D=A
    @sum
    M=D   // set sum=0
    
    @0
    D=A
    @i
    M=D   // set i=0
    
(LOOP)
    @i
    D=M
    @R1
    D=D-M
    @STOP
    D;JEQ   // if i==R1 then END
    
    @R0
    D=M
    @sum
    M=M+D   // sum=sum+R0
    @i
    M=M+1   // i=i+1
    @LOOP
    0;JMP
    
(STOP)
    @sum
    D=M
    @R2
    M=D
    
(END)
    @END
    0;JMP