#include "proc.h"
#include "command.h"

void SPUCtor(SPU_t* proc, const char* PathToCm) {
    FILE* PtrToCM = fopen(PathToCm, "rb");
    assert(PtrToCM != nullptr && "You entered not a file");
    size_t check[3] = {};
    
    fread(check, sizeof(size_t), sizeof(check) / sizeof(size_t), PtrToCM);

    if (check[0] != signature) {
        printf(RED "Wrong signature! Your signature: %d\n", check[0]);
        abort();
    } else if (check[1] != version) {
        printf(RED "Wrong version! %d\n", check[1]);
        abort();
    }
    proc->NumbOfComs = check[2];
    proc->CurrentCommand = 0;
    
    #if defined(SHM)
        proc->command = SPUShmCtor();
    #else
        size_t size = proc->NumbOfComs;
        proc->command = (elem_t*) calloc(size, sizeof(elem_t));
        fread(proc->command, sizeof(int), proc->NumbOfComs, PtrToCM);
    #endif // SHM
    
    STACK_CTOR(&proc->stk);
    STACK_CTOR(&proc->SourceAddress);
    for (size_t counter = 0; counter < NumbOfRegs; counter++) {
        proc->Register[counter] = 0;
    }
    
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
    proc->command = nullptr;
    StackDtor(&proc->stk);
    proc = nullptr;
}

void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line) {
    PointerToDump = StackDump(&proc->stk, file, function, line);
    
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

elem_t add(stack_t *stk) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(stk, &tmp1);
    StackPop(stk, &tmp2);
    StackPush(stk, (tmp1 + tmp2));

    return (tmp1 + tmp2);
}

elem_t sub(stack_t* stk) {
    elem_t deduct =  0;
    elem_t redused = 0;

    StackPop(stk, &deduct);
    StackPop(stk, &redused);
    StackPush(stk, (redused - deduct));

    return (redused - deduct);
}

elem_t div(stack_t* stk) {
    elem_t divider   = 0;
    elem_t divisible = 0;

    StackPop(stk, &divider);
    StackPop(stk, &divisible);
    StackPush(stk, (divisible * SIGNS / divider));

    return (divisible * SIGNS) / divider;
}

elem_t mul(stack_t* stk) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(stk, &tmp1);
    StackPop(stk, &tmp2);
    StackPush(stk, tmp1 * tmp2 / SIGNS);

    return tmp1 * tmp2;
}

elem_t proc_sqrt(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    double tmp1 = sqrt(tmp * SIGNS);
    tmp = tmp1;
    StackPush(stk, tmp);
    return tmp;
}

elem_t proc_sin(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    double tmp1 = sin(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(stk, tmp);
    return tmp;
}

elem_t proc_cos(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    double tmp1 = cos(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(stk, tmp);
    return tmp;
}

elem_t in(stack_t *stk) {
    elem_t tmp = 0;
    scanf("%d", &tmp);
    return tmp;
}

elem_t out(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    
    printf(GREEN);
    printf("%d\n", tmp);
    
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