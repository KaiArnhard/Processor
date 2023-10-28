#define MAKE_COND_JUMP(name, count, cond)                           \
    if (tmp == -1) {                                                \
        MyAssert(tmp != -1 && "You entered wrong label", "pampam"); \
        Error = -1;                                                 \
    } else if(CMD_##name == CMD_JMP) {                              \
        jump(proc, tmp);                                            \
    } else if(count cond proc->Register[1]) {                       \
        jump(proc, tmp);                                            \
    }    
     

DEF_CMD(HLT, -1, 0, 
                    printf(BLUE "\nProgramm ended succesfully\n" WHITE);
                    return Error;
            )
DEF_CMD(PUSH, 0, 1,
                    tmp = proc->command[++proc->CurrentCommand];
                    switch (proc->command[proc->CurrentCommand - 1] & IDENTIF) {
                    case RAMIdentif:
                        sleep(0.5);
                        StackPush(&proc->UserStack, proc->RAM[tmp]);
                        PrintfOfRAM(proc);
                        break;
                    case regis:
                        StackPush(&proc->UserStack, proc->Register[tmp]);
                        break;
                    case immed:
                        StackPush(&proc->UserStack, tmp * SIGNS);
                        break;
                    })
DEF_CMD(POP,  1, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    switch (proc->command[proc->CurrentCommand - 1] & IDENTIF) {
                    case RAMIdentif:
                        sleep(0.5);
                        StackPop(&proc->UserStack, proc->RAM + tmp);
                        PrintfOfRAM(proc);
                        break;
                    case regis:
                        StackPop(&proc->UserStack, proc->Register + tmp);
                        break;
                    case immed:
                        MyAssert((proc->command[proc->CurrentCommand - 1] & IDENTIF == immed) && "You are trying to pop a constant! Are you idiot!?", (char*) &(proc->command[proc->CurrentCommand]));
                        Error = -1;
                        break;
                    })
DEF_CMD(IN,   2, 0, 
                    printf("Enter number from keyboard\n");
                    while (scanf("%d", &tmp) != 1) {
                        printf("You entered not a number, try again\n");
                        while ((symbol = getchar()) != EOF && symbol != '\n') { }
                    }
                    tmp *= SIGNS;
                    StackPush(&proc->UserStack, tmp);)
DEF_CMD(ADD,  3, 0,
                    add(&proc->UserStack);)
DEF_CMD(SUB,  4, 0,
                    sub(&proc->UserStack);)
DEF_CMD(MUL,  5, 0,
                    mul(&proc->UserStack);)
DEF_CMD(DIV,  6, 0,
                    div(&proc->UserStack);)
DEF_CMD(SQRT, 7, 0,
                    proc_sqrt(&proc->UserStack);)
DEF_CMD(SIN,  8, 0,
                    proc_sin(&proc->UserStack);)
DEF_CMD(COS,  9, 0,
                    proc_cos(&proc->UserStack);)
DEF_CMD(OUT, 10, 0,
                    printf(GREEN "RESULT:\n" WHITE);
                    out(&proc->UserStack);)
DEF_CMD(JMP, 11, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    MAKE_COND_JUMP(JMP, 0, ==)
                    )
DEF_CMD(JA, 12, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->UserStack, &tmp1);
                    MAKE_COND_JUMP(JA, tmp1, >)
                    )
DEF_CMD(JAE, 13, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->UserStack, &tmp1);
                    MAKE_COND_JUMP(JAE, tmp1, >=)
                    )
DEF_CMD(JB, 14, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->UserStack, &tmp1);
                    MAKE_COND_JUMP(JB, tmp1, <)
                    )
DEF_CMD(JBE, 15, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->UserStack, &tmp1);
                    MAKE_COND_JUMP(JBE, tmp1, <=)
                    )
DEF_CMD(JE, 16, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->UserStack, &tmp1);
                    MAKE_COND_JUMP(JE, tmp1, ==)
                    )

DEF_CMD(JNE, 17, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->UserStack, &tmp1);
                    MAKE_COND_JUMP(JNE, tmp1, !=)
                    )
DEF_CMD(CALL, 18, 1,
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPush(&proc->CallStack, proc->CurrentCommand + 1);
                    jump(proc, tmp);
                    )
DEF_CMD(RET, 19, 0,
                    StackPop(&proc->CallStack, &tmp);
                    jump(proc, tmp);)

#include "DslUndef.h"