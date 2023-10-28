#include <unistd.h>
#include "ProcFunctions.h"
#include "command.h"

void SPUCtor(SPU_t* proc, stack_t UserStack, stack_t CallStack, elem_t* commands, size_t NumbOfComms) {
    *proc = {
        .Register       = {0},
        .RAM            = {0},
        .UserStack      = UserStack,
        .CallStack      = CallStack,
        .NumbOfComs     = NumbOfComms,
        .CurrentCommand = 0,
        .command        = commands,
    };
}

void SPUCtor(SPU_t* proc, const char* PathToCm) {
    FILE* PtrToCM = fopen(PathToCm, "rb");
    assert(PtrToCM != nullptr && "You entered not a file");
    
    struct {
        size_t signature;
        size_t version;
        size_t NumbOfComms;
    } BinHeader;
    
    fread(&BinHeader, sizeof(BinHeader), 1, PtrToCM);

    if (BinHeader.signature != signature) {
        printf(RED "Wrong signature! Your signature: %d\n", BinHeader.signature);
        abort();
    } else if (BinHeader.version != version) {
        printf(RED "Wrong version! %d\n", BinHeader.version);
        abort();
    }
    #if defined(SHM)
        proc->command = SPUShmCtor();
    #else
        elem_t* commands = (elem_t*) calloc(BinHeader.NumbOfComms, sizeof(elem_t));
        fread(commands, sizeof(*commands), BinHeader.NumbOfComms, PtrToCM);
    #endif // SHM
    
    fclose(PtrToCM);

    stack_t UserStack;
    STACK_CTOR(&UserStack);
    stack_t CallStack;
    STACK_CTOR(&CallStack);

    SPUCtor(proc, UserStack, CallStack, commands, BinHeader.NumbOfComms);
    
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
    for (size_t counter = 0; counter < NumbOfRegs; counter++) {
        proc->Register[counter] = POISON;
    }
    for (size_t counter = 0; counter < proc->NumbOfComs; counter++) {
        proc->command[counter] = POISON;
    }
    proc->NumbOfComs = POISON;
    #if defined(SHM)
        shmdt(proc->command);
    #else
        free(proc->command);
    #endif // SHM
    
    StackDtor(&proc->UserStack);
    StackDtor(&proc->CallStack);

    proc->command = nullptr;
    proc = nullptr;
    fclose(PointerToDump);
}

void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line) {
    PointerToDump = StackDump(&proc->UserStack, file, function, line);
    
    fprintf(PointerToDump, "\nAll registers \n");
    for (size_t counter = 0; counter < NumbOfRegs; counter++) {
        fprintf(PointerToDump, "%d\t", proc->Register[counter]);
    }
    fprintf(PointerToDump, "\n\n");
    fprintf(PointerToDump, "Number of commands %d\n\n", proc->NumbOfComs);
    fprintf(PointerToDump, "All commands\n");

    for (size_t counter = 0; counter < proc->NumbOfComs; counter++) {
        fprintf(PointerToDump, "%.2d ", counter);
    }
    fprintf(PointerToDump, "\n");
    
    for (size_t counter = 0; counter < proc->NumbOfComs; counter++) {
        if (counter == proc->CurrentCommand) {
            fprintf(PointerToDump, "[%.2d] ", proc->command[counter]);
        } else {
            fprintf(PointerToDump, "%.2d ", proc->command[counter]);
        }
    }
    fprintf(PointerToDump, "\n\nCurent command %d\n\n", proc->CurrentCommand);
    fprintf(PointerToDump, "\n");
}

void PrintfOfRAM(SPU_t* proc) {
    for (size_t counter = 0; counter < SizeOfRAM; counter += 10) {
        for (size_t counter1 = counter; counter1 < 10 + counter; counter1++) {
            if (proc->RAM[counter1] != 0) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    system("clear");
    sleep(0.5);
}

elem_t add(stack_t *UserStack) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(UserStack, &tmp1);
    StackPop(UserStack, &tmp2);
    StackPush(UserStack, (tmp1 + tmp2));

    return (tmp1 + tmp2);
}

elem_t sub(stack_t* UserStack) {
    elem_t deduct =  0;
    elem_t redused = 0;

    StackPop(UserStack, &deduct);
    StackPop(UserStack, &redused);
    StackPush(UserStack, (redused - deduct));

    return (redused - deduct);
}

elem_t div(stack_t* UserStack) {
    elem_t divider   = 0;
    elem_t divisible = 0;

    StackPop(UserStack, &divider);
    StackPop(UserStack, &divisible);
    StackPush(UserStack, (divisible * SIGNS / divider));

    return (divisible * SIGNS) / divider;
}

elem_t mul(stack_t* UserStack) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(UserStack, &tmp1);
    StackPop(UserStack, &tmp2);
    StackPush(UserStack, tmp1 * tmp2 / SIGNS);

    return tmp1 * tmp2;
}

elem_t proc_sqrt(stack_t* UserStack) {
    elem_t tmp = 0;
    StackPop(UserStack, &tmp);
    double tmp1 = sqrt(tmp * SIGNS);
    tmp = tmp1;
    StackPush(UserStack, tmp);
    return tmp;
}

elem_t proc_sin(stack_t* UserStack) {
    elem_t tmp = 0;
    StackPop(UserStack, &tmp);
    double tmp1 = sin(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(UserStack, tmp);
    return tmp;
}

elem_t proc_cos(stack_t* UserStack) {
    elem_t tmp = 0;
    StackPop(UserStack, &tmp);
    double tmp1 = cos(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(UserStack, tmp);
    return tmp;
}

elem_t in(stack_t *UserStack) {
    elem_t tmp = 0;
    scanf("%d", &tmp);
    return tmp;
}

elem_t out(stack_t* UserStack) {
    elem_t tmp = 0;
    StackPop(UserStack, &tmp);
    
    printf(GREEN "%d\n" WHITE, tmp);
    
    return tmp;
}

size_t jump(SPU_t* proc, size_t DestAddress) {
    proc->CurrentCommand = DestAddress - 1;
    return proc->CurrentCommand;
}

int VirtualMachine(SPU_t* proc) {

    #define DEF_CMD(name, numb, arg, code)  \
        case (CMD_##name & BITCOMM):        \
            code                            \
        break;
    

    int tmp      = 0;
    int tmp1     = 0;
    char symbol  = 0;

    for (size_t i = 0; i < proc->NumbOfComs; i++) {
        printf("%d\n", proc->command[i]);
    }
    
    while(true) {
        SPU_DUMP(proc);
        switch (proc->command[proc->CurrentCommand] & BITCOMM) {
        #include "dsl.h"
            default:
                MyAssert(1 && "SYNTAX ERROR!", (char*) &(proc->command[proc->CurrentCommand]));
                Error = -1;
            break;
        }
        proc->CurrentCommand++;
    }
    return 0;
}

void MyAssertionFailed(const char* condition, const char* command, const char* file, const char* function, const int line) {
    printf(RED "Assertion failed on file %s on function %s:%d\n", file, function, line);
    printf("Wrong condition: %s\n", condition);
    printf("%d\n" WHITE, *((int*) command));
}