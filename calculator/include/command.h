#if !defined(COMMAND_H)
#define COMMAND_H

enum Commands {
    HLT        = -1,
    STACK_PUSH =  0,
    STACK_IN   =  1,
    STACK_POP  =  2,
    ADD        =  3,
    SUB        =  4,
    DIV        =  5,
    MUL        =  6,
    SQRT       =  7,
    SIN        =  8,
    COS        =  9,
    OUT        =  10 
};

#endif // COMMAND_H