#if !defined(COMMAND_H)
#define COMMAND_H

enum Commands {
    STACK_PUSH =  0,
    STACK_IN   =  1,
    ADD        =  2,
    SUB        =  3,
    DIV        =  4,
    MUL        =  5,
    SQRT       =  6,
    SIN        =  7,
    COS        =  8,
    OUT        =  9, 
    HLT        = -1
};

int assembly(const char* fname1, const char* fname2);

#endif // COMMAND_H