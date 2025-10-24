IN
IN
IN
POPR REG3
POPR REG2
POPR REG1

PUSH 0
PUSHR REG1
JE :1 ;  check if line equation

CALL :2

PUSHR REG4
PUSH 0
JB :5 ; no roots

PUSH 0
PUSHR REG2 ; -b
SUB

PUSHR REG4 ;sqrtDISK
SQRT

ADD

PUSHR REG1 ; 2a
PUSH 2
MUL

DIV
POPR REG5 ;root_1

PUSH 0
PUSHR REG2 ; -b
SUB

PUSHR REG4 ;sqrtDISK
SQRT

SUB

PUSHR REG1 ; 2a
PUSH 2
MUL

DIV
POPR REG6 ;root_2

JMP :7

: 2 ;disc

    PUSHR REG1
    PUSHR REG3
    PUSH 4
    MUL
    MUL

    PUSHR REG2
    PUSHR REG2
    MUL

    SUB
    POPR REG4
    RET

:1 ;line case

    PUSHR REG2
    PUSHR 0
    JE :3 ;inf cases

    PUSHR REG2
    PUSHR REG3
    PUSH 0
    SUB
    DIV
    POPR REG6
    JMP :6


:3 ; inf_case

    PUSH 0
    PUSHR REG3
    JA :4
    JMP :5

:4 ; end_inf

    PUSH 1488
    OUT
    HLT

:5 ; end_zero

    PUSH 322
    OUT
    HLT

:6 ; end_one

    PUSHR REG6
    OUT
    HLT

:7 ; end_two
    PUSHR REG6
    PUSHR REG5
    OUT
    OUT
    HLT