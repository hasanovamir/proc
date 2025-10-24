IN
CALL :1
OUT
HLT
:1 ; FACTORIAL
    POPR REG1
    PUSHR REG1
    PUSHR REG1
    PUSH 1
    JNE :2
    PUSHR REG1
    MUL
    RET
:2
        PUSHR REG1
        PUSH 1
        SUB
        CALL :1
    MUL
    RET
