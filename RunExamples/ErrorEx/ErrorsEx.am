;errors check - first pass phase

;label errors
myLabel: .string "This should be fine"
1myLabel: .string "This shouldn't"
thisIsAveryVeryVeryVeryVeryLongLabel: .data 12, 4, 56, 78, 10
this label : mov L1, L1
myLabel: .string "This label was defined"

;comma errors
myArray: .data 12,6, -9, 10,
    myArray3: .data ,12,6, -9, , 10
myArray5: .data 12, 6, -9, 0, -10       ,
    mov, r3, K
cmp K, , #-6
sub ,r1, r4
add , L3  L
inc , r1

;data errors 
    .data
    .data lost, 4, 8, 15, 16, 23, 42
    .data --433, 653, 30
    .data 763, 599, ++875, 5
    .data 4,8,15,16,23,4 +2
    .data  4, 8, 15, x, 16, 23, 42
    .data 3, 4, 6.5, 9

;string errors
    .string
    .string za wardo
    .string za wardo"
    .string "za wardo
    .string za "wardo".
    .string ",
;extraneous words
    .data 943 .data
    .string "sdf" .string
    .extern Hello World
    .entry Goodbye Earth
    
mov A, r1, r2
cmp A, r1 r2
add A, r0, r1
sub #3, r1 r5
lea HAHAHA, r3, r4
clr r2 r6
inc r1, r4
dec Count Monte Cristo
jmp %ROOF, %FLOOR
bne Bob Sponge
jsr BAT, MAN
red r3 r5
prn r1, r2
customLabel1:.data 943 .data
customLabel2:.string "sdf" .string
customLabel3:.extern Hello World
customLabel4:.entry Goodbye Earth
customLabel5:mov A, r1, r2
customLabel8:cmp A, r1 r2
customLabel9:add A, r0, r1
customLabel12:sub #3, r1 r5
customLabel13:lea HAHAHA, r3, r4
customLabel16:clr r2 r6
customLabel17:inc r1, r4
customLabel20:dec Count Monte Cristo
customLabel21:jmp %ROOF, %FLOOR
customLabel24:bne Bob Sponge
customLabel25:jsr BAT, MAN
customLabel28:red r3 r5
customLabel29:prn r1, r2
;missing word
    .data
    .string
    .extern
    .entry
mov A
cmp A
add A
sub #3
lea HAHAHA
clr
inc
dec
jmp
bne
jsr
red
prn
someLabel31:.data
someLabel32:.string
someLabel33:.extern
someLabel34:.entry
someLabel35:mov A
someLabel36:cmp A
someLabel37:add A
someLabel38:sub #3
someLabel39:lea HAHAHA
someLabel40:clr
someLabel41:inc
someLabel42:dec
someLabel43:jmp
someLabel44:bne
someLabel45:jsr
someLabel46:red
someLabel47:prn
;unknown words
COOK tasty, pie
    .Data 12, 675
    .sTring "Whoops"
;operand addressing errors
mov %ohno, %ohyes
mov %ohno, r3
mov #-1, #5
cmp %ohno, %ohyes
add #3, #1
add %ohno, %ohyes
add %ohno, someLabel47
sub %ohno, #1
sub %ohno, r3
lea #-1, #8
lea #-1, %ohno
lea %ohno, %ohyes
lea r1, #-1
clr #-1
clr %ohno
not #-1
not %ohno
inc #-1
inc %ohno
dec #-1
dec %ohno
jmp #-1
jmp r1,r2
bne #-1
jsr #-1
prn %ohno
rts #-1
rts %ohno
stop r1
