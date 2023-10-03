#if !defined(COMMAND_H)
#define COMMAND_H

enum Commands {
    STACK_PUSH =  0,
    STACK_POP  =  1,
    STACK_IN   =  2,
    ADD        =  3,
    SUB        =  4,
    DIV        =  5,
    MUL        =  6,
    SQRT       =  7,
    SIN        =  8,
    COS        =  9,
    HLT        = -1
};

int assembly(const char* fname1, const char* fname2);

#endif // COMMAND_H
