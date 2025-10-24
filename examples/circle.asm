;main


:1 ; while (i < 10)

    PUSH 0
    POPR REG10

    :5

    PUSHR REG10
    PUSH 21

    JE :6

        PUSH 0 ; NEW CYCLE
        POPR REG11

        :7

        PUSHR REG11
        PUSH 21

        JE :8

            PUSHR REG10
            PUSH 21
            MUL
            PUSHR REG11
            ADD
            POPR REG6 ; REGISTER TO RAM
            PUSHR REG10
            PUSH 10
            SUB
            PUSHR REG10
            PUSH 10
            SUB
            MUL
            PUSHR REG11
            PUSH 10
            SUB
            PUSHR REG11
            PUSH 10
            SUB
            MUL
            ADD
            PUSH 100
            JA :9
            PUSH 1
            POPM [REG6]
            :9  
        PUSHR REG11 ;i++
        PUSH 1
        ADD
        POPR REG11

        JMP :7
    :8

    PUSHR REG10 ;i++
    PUSH 1
    ADD
    POPR REG10

    JMP :5
:6
DRAW
MEOW
HLT