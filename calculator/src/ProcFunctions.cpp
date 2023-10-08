#include "proc.h"

void SPUCtor(SPU_t* proc, FILE* PtrToCm) {
    int check[3] = {};
    
    fread(check, sizeof(int), 3, PtrToCm);

    if (check[0] != signature) {
        printf(RED);
        printf("Wrong signature!\n");
        abort();
    } else if (check[1] != version) {
        printf(RED);
        printf("Wrong version! %d\n", check[1]);
        abort();
    }
    proc->NumbOfComs = check[2];
    proc->CurrentCommand = 0;
    
    #if defined(LINUX)
        key_t key = ftok(PathToCm, proj_id);
        size_t size = sizeof(elem_t) * (proc->NumbOfComs + 3);
        int shm_id = shmget(key, size, 0666 | IPC_CREAT |IPC_EXCL);
        if (shm_id == -1) {
            shm_id = shmget(key, size, 0666);
        }
        proc->command = (elem_t*) shmat(shm_id, nullptr, 0);
    #else
        size_t size = (proc->NumbOfComs + 1);
        proc->command = (elem_t*) calloc(size, sizeof(elem_t));
    #endif // LINUX
    
    STACK_CTOR(&proc->stk);
    proc->rax = 0;
    proc->rbx = 0;
    proc->rcx = 0;
    proc->rdx = 0;
}

void SPUDtor(SPU_t* proc) {
    proc->rax = POISON;
    proc->rbx = POISON;
    proc->rcx = POISON;
    proc->rdx = POISON;

    for (size_t counter = 0; counter < proc->NumbOfComs; counter++) {
        proc->command[counter] = POISON;
    }
    proc->NumbOfComs = POISON;
    #if defined(LINUX)
        shmdt(proc->command);
    #else
        free(proc->command);
    #endif // LINUX
    proc->command = nullptr;
    StackDtor(&proc->stk);
    proc = nullptr;
}

void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line) {
    PointerToDump = StackDump(&proc->stk, file, function, line);

    fprintf(PointerToDump, "\nAll registers \n%d\t%d\t%d\t%d\n\n", proc->rax, proc->rbx, proc->rcx, proc->rdx);
    fprintf(PointerToDump, "Number of commands %d\n\n", proc->NumbOfComs);
    fprintf(PointerToDump, "All commands\n");
    for (size_t counter = 0; counter < proc->NumbOfComs; counter++) {
        fprintf(PointerToDump, "%.2d ", counter);
    }
    fprintf(PointerToDump, "\n");
    for (size_t counter = 0; counter < proc->NumbOfComs; counter++) {
        if (counter == proc->CurrentCommand) {
            fprintf(PointerToDump, "[%.2d] ", proc->command[counter + 3]);
        } else {
            fprintf(PointerToDump, "%.2d ", proc->command[counter + 3]);
        }
    }
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

int VirtualMachine(SPU_t* proc, FILE* PtrToCm) {
    int tmp      = 0;
    char symbol  = 0;
    char BitComm = 0;
    char Identif = 0;
    bitwise(&BitComm ,&Identif);

    for (size_t i = 0; i < proc->command[2] + 3; i++) {
        printf("%d\n", proc->command[i]);
    }
    
    for (size_t counter = 3;; counter++) {
        SPU_DUMP(proc);
        switch ((proc->command[counter] & BitComm)) {
        case HLT:
            printf("\nProgramm ended succesfully\n");
            exit(0);
            break;
        case STACK_PUSH:
            tmp = proc->command[++counter];
            proc->CurrentCommand++;
            switch (proc->command[counter - 1] & Identif) {
            case regis:
                switch (tmp) {
                case RAX:
                    StackPush(&proc->stk, proc->rax);
                    break;
                case RBX:
                    StackPush(&proc->stk, proc->rbx);
                    break;
                case RCX:
                    StackPush(&proc->stk, proc->rcx);
                    break;
                case RDX:
                    StackPush(&proc->stk, proc->rdx);
                    break;
                }
                break;
            case immed:
                StackPush(&proc->stk, tmp * SIGNS);
                break;
            }
            break;
        case STACK_IN:
            printf("Enter number from keyboard\n");
            while (scanf("%d", &tmp) != 1) {
                printf("You entered not a number, try again\n");
                while ((symbol = getchar()) != EOF && symbol != '\n') { }
            }
            tmp *= SIGNS;
            StackPush(&proc->stk, tmp);
            break;
        case STACK_POP:
        tmp = proc->command[++counter];
        proc->CurrentCommand++;
            switch (proc->command[counter - 1] & Identif) {
            case regis:
                switch (tmp) {
                case RAX:
                    StackPop(&proc->stk, &proc->rax);
                    break;
                case RBX:
                    StackPop(&proc->stk, &proc->rbx);
                    break;
                case RCX:
                    StackPop(&proc->stk, &proc->rcx);
                    break;
                case RDX:
                    StackPop(&proc->stk, &proc->rdx);
                    break;
                }
                break;
            case immed:
                printf(RED);
                printf("You trying to pop constant! Are you idiot?\n");
                abort();
                break;
            }
            break;
        case ADD:
            add(&proc->stk);
            break;
        case SUB:
            sub(&proc->stk);
            break;
        case DIV:
            div(&proc->stk);
            break;
        case MUL:
            mul(&proc->stk);
            break;
        case SQRT:
            proc_sqrt(&proc->stk);
            break;
        case SIN:
            proc_sin(&proc->stk);
            break;
        case COS:
            proc_cos(&proc->stk);
            break;
        case OUT:
            printf("RESULT:\n");
            out(&proc->stk);
            break;
        default:
            printf(RED);
            printf("\nSYNTAX ERROR\n");
            printf("NUMBER OF COMMAND IS %d\n", proc->command[counter]);
            abort();
        break;
        }
        proc->CurrentCommand++;
    }
    return 0;
}

int bitwise(char* BitComm, char* Identif) {
    for (size_t counter = 0; counter < 4; counter++) {
        *BitComm += 1 << counter;
        *Identif += 16 << counter;
    }
    return 0;
}