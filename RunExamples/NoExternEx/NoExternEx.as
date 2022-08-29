.entry LOOP
.entry END
MAIN: mov S1.1, LOOP
 add r2,STR
LOOP: jmp LOOP
 prn #-5
 sub r1, r4
macro m1
 inc K

mov S1.2, r2
endmacro
 bne END
END: hlt
m1
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
S1: .struct 8, "ab" 
