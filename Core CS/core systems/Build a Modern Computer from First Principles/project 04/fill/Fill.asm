// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// while(true)
// {
//     if(KBD == 0) set "white"
//     else set "black"
// }

// set screen:
// for(i=0; i<8192; i++)  //8192 = 256*(512/16), 256 rows, (512/16) 16-bit chunks in each row 
// {
//     *(SCREEN+i) = 0 or -1  // 0 for "white", -1 for "black"
// }

    @8192
    D=A
    @n
    M=D  // set n=8192
    
(LOOP)
    @0
    D=A
    @i
    M=D  // set i=0
    
    @KBD
    D=M
    @WHITE
    D;JEQ   // if KBD == 0 then set "white"

  (LB)
    @SCREEN
    D=A
    @i
    A=D+M   // set A=SCREEN+i
    M=-1    // set Mem[SCREEN+i]=-1
    
    @i
    M=M+1   // i=i+1
    
    D=M
    @n
    D=D-M
    @LB
    D;JLT   // if i<8192 then LB (Loop for Black)
    @LOOP
    0;JMP
    
(WHITE)
  (LW)
    @SCREEN
    D=A
    @i
    A=D+M   // set A=SCREEN+i
    M=0     // set Mem[SCREEN+i]=0
    
    @i
    M=M+1   // i=i+1
    
    D=M
    @n
    D=D-M
    @LW
    D;JLT   // if i<8192 then LW (Loop for White)
    @LOOP
    0;JMP