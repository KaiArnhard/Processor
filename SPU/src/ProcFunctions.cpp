#include <unistd.h>
#include "ProcFunctions.h"
#include "command.h"

void SPUCtor(SPU_t* proc, stack_t userStack, stack_t callStack, elem_t* commands, size_t numbOfComms) {
    *proc = {
        .regis         = {0},
        .ram            = {0},
        .userStack      = userStack,
        .callStack      = callStack,
        .numbOfComs     = numbOfComms,
        .currentCommand = 0,
        .command        = commands,
    };
}

void SPUCtor(SPU_t* proc, elem_t* commands, size_t numbOfComms) {
    stack_t UserStack;
    STACK_CTOR(&UserStack);
    stack_t CallStack;
    STACK_CTOR(&CallStack);
    SPUCtor(proc, UserStack, CallStack, commands, numbOfComms);
}

void SPUCtor(SPU_t* proc, FILE* ptrToCm) {
    struct {
        size_t signature;
        size_t version;
        size_t NumbOfComms;
    } binHeader;
    
    fread(&binHeader, sizeof(binHeader), 1, ptrToCm);

    if (binHeader.signature != signature) {
        printf(RED "Wrong signature! Your signature: %d\n" WHITE, binHeader.signature);
        abort();
    } else if (binHeader.version != version) {
        printf(RED "Wrong version! %d\n" WHITE, binHeader.version);
        abort();
    }
    #if defined(SHM)
        proc->command = SPUShmCtor();
    #else
        elem_t* commands = (elem_t*) calloc(binHeader.NumbOfComms, sizeof(elem_t));
        fread(commands, sizeof(*commands), binHeader.NumbOfComms, ptrToCm);
    #endif // SHM
    
    fclose(ptrToCm);

    SPUCtor(proc, commands, binHeader.NumbOfComms);
    
}

#if defined(SHM)
    elem_t* SPUShmCtor() {
        key_t key = ftok(PathToCm, proj_id);
        size_t size = sizeof(elem_t) * (proc->NumbOfComs + 3);
        int shm_id = shmget(key, size, 0666 | IPC_CREAT |IPC_EXCL);
        if (shm_id == -1) {
            shm_id = shmget(key, size, 0666);
        }
        return (elem_t*) shmat(shm_id, nullptr, 0);
    }
#endif // SHM


void SPUDtor(SPU_t* proc) {
    for (size_t counter = 0; counter < numbOfRegs; counter++) {
        proc->regis[counter] = POISON;
    }
    for (size_t counter = 0; counter < proc->numbOfComs; counter++) {
        proc->command[counter] = POISON;
    }
    proc->numbOfComs = POISON;
    #if defined(SHM)
        shmdt(proc->command);
    #else
        free(proc->command);
    #endif // SHM
    
    StackDtor(&proc->userStack);
    StackDtor(&proc->callStack);

    proc->command = nullptr;
    proc = nullptr;
    fclose(pointerToDump);
}

void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line) {
    pointerToDump = StackDump(&proc->userStack, file, function, line);
    
    fprintf(pointerToDump, "\nAll registers \n");
    for (size_t counter = 0; counter < numbOfRegs; counter++) {
        fprintf(pointerToDump, "%d\t", proc->regis[counter]);
    }
    fprintf(pointerToDump, "\n\n");
    fprintf(pointerToDump, "Number of commands %d\n\n", proc->numbOfComs);
    fprintf(pointerToDump, "All commands\n");

    for (size_t counter = 0; counter < proc->numbOfComs; counter++) {
        fprintf(pointerToDump, "%.2d ", counter);
    }
    fprintf(pointerToDump, "\n");
    
    for (size_t counter = 0; counter < proc->numbOfComs; counter++) {
        if (counter == proc->currentCommand) {
            fprintf(pointerToDump, "[%.2d] ", proc->command[counter]);
        } else {
            fprintf(pointerToDump, "%.2d ", proc->command[counter]);
        }
    }
    fprintf(pointerToDump, "\n\nCurent command %d\n\n", proc->currentCommand);
    fprintf(pointerToDump, "\n");
}

void PrintOfRam(SPU_t* proc) {
    for (size_t counter = 0; counter < sizeOfRam; counter += 10) {
        for (size_t counter1 = counter; counter1 < 10 + counter; counter1++) {
            if (proc->ram[counter1] != 0) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("programm is paused, press enter to continue\n");
    getchar();
    system("clear");
    sleep(0.5);
}

elem_t Add(stack_t *userStack) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(userStack, &tmp1);
    StackPop(userStack, &tmp2);
    StackPush(userStack, (tmp1 + tmp2));

    return (tmp1 + tmp2);
}

elem_t Sub(stack_t* userStack) {
    elem_t deduct =  0;
    elem_t redused = 0;

    StackPop(userStack, &deduct);
    StackPop(userStack, &redused);
    StackPush(userStack, (redused - deduct));

    return (redused - deduct);
}

elem_t Div(stack_t* userStack) {
    elem_t divider   = 0;
    elem_t divisible = 0;

    StackPop(userStack, &divider);
    StackPop(userStack, &divisible);
    StackPush(userStack, (divisible * SIGNS / divider));

    return (divisible * SIGNS) / divider;
}

elem_t Mul(stack_t* userStack) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(userStack, &tmp1);
    StackPop(userStack, &tmp2);
    StackPush(userStack, tmp1 * tmp2 / SIGNS);

    return tmp1 * tmp2;
}

elem_t ProcSqrt(stack_t* userStack) {
    elem_t tmp = 0;
    StackPop(userStack, &tmp);
    double tmp1 = sqrt(tmp * SIGNS);
    tmp = tmp1;
    StackPush(userStack, tmp);
    return tmp;
}

elem_t ProcSin(stack_t* userStack) {
    elem_t tmp = 0;
    StackPop(userStack, &tmp);
    double tmp1 = sin(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(userStack, tmp);
    return tmp;
}

elem_t ProcCos(stack_t* userStack) {
    elem_t tmp = 0;
    StackPop(userStack, &tmp);
    double tmp1 = cos(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(userStack, tmp);
    return tmp;
}

elem_t In(stack_t *userStack) {
    elem_t tmp = 0;
    char symbol  = 0;

    printf("Enter number from keyboard\n");
    while (scanf("%d", &tmp) != 1) {
        printf("You entered not a number, try again\n");
        while ((symbol = getchar()) != EOF && symbol != '\n') { }
    }
    tmp *= SIGNS;
    StackPush(userStack, tmp);
    return tmp;    
}

elem_t Out(stack_t* UserStack) {
    elem_t tmp = 0;
    StackPop(UserStack, &tmp);
    
    printf(GREEN "%d\n" WHITE, tmp);
    
    return tmp;
}

size_t Jump(SPU_t* proc, size_t destAddress) {
    proc->currentCommand = destAddress - 1;
    return proc->currentCommand;
}

int VirtualMachine(SPU_t* proc) {

    #define DEF_CMD(name, numb, arg, code)  \
        case (CMD_##name & BITCOMM):        \
            code                            \
        break;
    

    int tmp      = 0;
    int tmp1     = 0;

    for (size_t i = 0; i < proc->numbOfComs; i++) {
        printf("%d\n", proc->command[i]);
    }
    
    while(true) {
        switch (proc->command[proc->currentCommand] & BITCOMM) {
        #include "dsl.h"
            default:
                MyAssert(0 && "SYNTAX ERROR!", (char*) &(proc->command[proc->currentCommand]));
                Error = -1;
            break;
        }
        proc->currentCommand++;
    }
    return 0;
}

void MyAssertionFailed(const char* condition, const char* command, const char* file, const char* function, const int line) {
    printf(RED "Assertion failed on file %s on function %s:%d\n", file, function, line);
    printf("Wrong condition: %s\n", condition);
    printf("%d\n" WHITE, *((int*) command));
}