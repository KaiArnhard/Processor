#include "StackFunctions.h"
#include <assert.h>

void StackCtor (stack_t* stk, size_t capacity, const char* name, const size_t line, const char* file, const char* function) {
    PTR_STACK_CHECK(stk);

    size_t size = 0;
    stk->capacity = capacity;
    stk->size = 0;
    stk->OldCapacity = 0;
    stk->var = {name, line, file, function};

    #if defined(CANARY_PROT)
        char* ptr = nullptr;
        ptr = (char* ) calloc(capacity * sizeof(elem_t) + 2 * sizeof(canary_t), sizeof(char));
        assert(ptr);
        
        ((canary_t*) ptr)[0] = stk->LeftCanary;

        ((canary_t*) (ptr + sizeof(canary_t) + capacity * sizeof(elem_t)))[0] = stk->RightCanary;

        stk->data = (elem_t*) (ptr + sizeof(canary_t));
        PoisStack(stk);
        #if defined(HASH_PROT)
            STACK_HASH(stk);
        #endif // HASH_PROT
        

    #else
        stk->data = (elem_t*) calloc(capacity, sizeof(elem_t));   
        PoisStack(stk);
        #if defined(HASH_PROT)
            STACK_HASH(stk);
        #endif // HASH_PROT
    
    #endif // CANARY_PROT
    
    STACK_CHECK(stk);
}

void StackDtor(stack_t* stk) {
    STACK_CHECK(stk);

    #if defined(CANARY_PROT)
        PoisStack(stk);
        free(((char*)stk->data - sizeof(canary_t)));
    #else
        PoisStack(stk);
        free(stk->data);
    #endif // CANARY_PROT
    

    stk->data = nullptr;
    stk->capacity = UINT_MAX;
    stk->size = UINT_MAX;
    stk->var = {"alsdjfas", UINT_MAX, "aslkdfjaslkf", "lasdkjfsaklf"};
    stk = nullptr;
}

void PoisStack(stack_t* stk) {
    for (size_t counter = stk->size; counter < stk->capacity; counter++) {
        stk->data[counter] = POISON;
    }
}

void StackPush(stack_t* stk, const elem_t variable) {
    STACK_CHECK(stk);
    if (stk->size + 1 == stk->capacity) {
        stk->OldCapacity = StackResize(stk, UP);
    }
    
    stk->data[stk->size] = variable;
    stk->size++;
    
    #if defined(HASH_PROT)
        STACK_HASH(stk);
    #endif // HASH_PROT
    
    STACK_CHECK(stk);
}

void StackPop(stack_t* stk, elem_t* ptr) {
    STACK_CHECK(stk);
    if (stk->OldCapacity != 0 && stk->size <= stk->OldCapacity - 3) {
        stk->OldCapacity = StackResize(stk, DOWN);
    }
    stk->size--;
    *ptr = stk->data[stk->size];
    stk->data[stk->size] = POISON;

     #if defined(HASH_PROT)
        STACK_HASH(stk);
    #endif // HASH_PROT
    STACK_CHECK(stk);
}

size_t StackResize(stack_t* stk, Resize_t CodeOfResize) {
    STACK_CHECK(stk);
    assert(CodeOfResize == 0 || CodeOfResize == 1);
    char* ptr = nullptr;
    size_t OldCapacity = 0;
    #if defined(CANARY_PROT)
        switch (CodeOfResize) {
            case DOWN:
                ptr = (char*) stk->data - sizeof(canary_t);
                ptr = (char*) realloc(ptr, sizeof(elem_t) * stk->OldCapacity + 2 * sizeof(canary_t));
                assert(ptr);
                stk->data = (elem_t*) (ptr + sizeof(canary_t));
                stk->capacity = stk->OldCapacity;
                ((canary_t *) (stk->data + stk->capacity))[0] = stk->RightCanary;
                break;
            case UP:
                OldCapacity = stk->capacity;
                stk->capacity *= ResizeConst;
                ptr = (char*) realloc(((char*) stk->data) - sizeof(canary_t), sizeof(elem_t) * stk->capacity + 2 * sizeof(canary_t));
                assert(ptr);
                stk->data = (elem_t*) (ptr + sizeof(canary_t));
                ((canary_t *) (stk->data + stk->capacity))[0] = stk->RightCanary;
                PoisStack(stk);
                break;
            }
    #else
        switch (CodeOfResize) {
        case DOWN:
            ptr = (char*) realloc(stk->data, sizeof(elem_t) * stk->OldCapacity);
            PTR_STACK_CHECK(ptr);
            stk->data = (elem_t*) ptr;
            stk->capacity = stk->OldCapacity;
            break;
        case UP:
            OldCapacity = stk->capacity;
            stk->capacity *= ResizeConst;
            ptr = (char*) realloc(stk->data, sizeof(elem_t) * stk->capacity);
            PTR_STACK_CHECK(ptr);
            stk->data = (elem_t*) ptr;
            PoisStack(stk);
            break;
        }
    #endif // CANARY_PROT

    #if defined(HASH_PROT)
        STACK_HASH(stk);
    #endif // HASH_PROT
    
    STACK_CHECK(stk);
    return OldCapacity;
}

FILE* StackDump(stack_t* stk, const char* file, const char* function, size_t line) {
    
    fprintf(pointerToDump, "Stack [%p], %s  from %s line: %d %s \n\n", stk, stk->var.name, stk->var.file, stk->var.line, stk->var.function);

    #if defined(CANARY_PROT)

        fprintf(pointerToDump, "Left stack canary %llX \n",  stk->LeftCanary);
        fprintf(pointerToDump, "Right stack canary %llX \n", stk->RightCanary);
    #endif // CANARY_PROT
    
    fprintf(pointerToDump, "Called from %s(%d), %s\n", file, line, function);
    fprintf(pointerToDump, "Error numbers \n");
    size_t Error[11] = {};
    ErrorDecoder(Error);
    
    #if defined(HASH_PROT)
        fprintf(pointerToDump, "Stack hash %lld\n", stk->hash);
    #endif // HASH_PROT

    fprintf(pointerToDump, "size = %d, capacity = %d \n", stk->size, stk->capacity);
    fprintf(pointerToDump, "data [%p] \n", stk->data);

    if (Error[1] || Error[3] || Error[4]) {
        exit(-1);
    }
    
    if (!Error[4] && !Error[5]) {
        
        PrintOfData(stk, pointerToDump);
    
    } else if (Error[3]) {
        for (size_t counter = 0; counter < stk->size; counter++) {
            fprintf(pointerToDump, "[%d] = %d \n", counter, stk->data[counter]);
        }
    }
    return pointerToDump;
}

size_t StackVerify(stack_t* stk) {
    PTR_STACK_CHECK(stk);
    if (stk->data == nullptr) {
        MyErrorno |= STACK_ERROR_PTR_TO_DATA_ZERO;
    
    } if (stk->capacity < stk->size) {
        MyErrorno |= STACK_ERROR_SIZE_OVER_CAPACITY;
    
    } if (stk->size == __UINT64_MAX__) {
        MyErrorno |= STACK_ERROR_SIZE_LOWER_ZERO;
    
    } if (stk->capacity == __UINT64_MAX__) {
        MyErrorno |= STACK_ERROR_CAPACITY_LOWER_ZERO;
    
    } if (stk->capacity == 0) {
        MyErrorno |= STACK_ERROR_CAPACITY_EQUAL_ZERO;
    
    } if (stk->capacity < DefaultSize) {
        MyErrorno |= STACK_ERROR_CAPACITY_LOWER_DEFAULT;
    }

    #if defined(CANARY_PROT)
    
        if (stk->LeftCanary != 0xDED320BED) {
            MyErrorno |= STACK_ERROR_LEFT_CANARY_DIED;
        
        } if (stk->RightCanary != 0xBADC0FFEE) {
            MyErrorno |= STACK_ERROR_LEFT_CANARY_DIED;
        
        }
        if (((canary_t*) stk->data)[-1] != 0xDED320BED) {
            MyErrorno |= STACK_ERROR_DATA_LEFT_CANARY_DIED;
    
        } if (((canary_t*) (stk->data + stk->capacity))[0] != 0xBADC0FFEE) {
            MyErrorno |= STACK_ERROR_DATA_RIGHT_CANARY_DIED;
        }        
                
    #endif // CANARY_PROT
    
    #if defined(HASH_PROT)
        if (!StackHashCheck(stk)) {
            printf("dbg\n");
            MyErrorno |= STACK_ERROR_WRONG_HASH;
        }
        
    #endif // HASH_PROT
    
    return MyErrorno;
}

void ErrorDecoder(size_t* Error) {
    if (MyErrorno & STACK_ERROR_STACK_OVERFLOW) {                  
        Error[0] = STACK_ERROR_STACK_OVERFLOW;
        fprintf(pointerToDump, "%d ", Error[0]);
    
    } if (MyErrorno & STACK_ERROR_PTR_TO_DATA_ZERO) {
        Error[1] = STACK_ERROR_PTR_TO_DATA_ZERO;
        fprintf(pointerToDump, "%d ", Error[1]);
    
    } if (MyErrorno & STACK_ERROR_SIZE_OVER_CAPACITY) {
        Error[2] = STACK_ERROR_SIZE_OVER_CAPACITY;
        fprintf(pointerToDump, "%d ", Error[2]);
    
    } if (MyErrorno & STACK_ERROR_SIZE_LOWER_ZERO) {
        Error[3] = STACK_ERROR_SIZE_LOWER_ZERO;
        fprintf(pointerToDump, "%d ", Error[3]);
    
    } if (MyErrorno & STACK_ERROR_CAPACITY_LOWER_ZERO) {
        Error[4] = STACK_ERROR_CAPACITY_LOWER_ZERO;
        fprintf(pointerToDump, "%d ", Error[4]); 
    
    } if (MyErrorno & STACK_ERROR_CAPACITY_EQUAL_ZERO) {
        Error[5] = STACK_ERROR_CAPACITY_EQUAL_ZERO;
        fprintf(pointerToDump, "%d ", Error[5]);
    
    } if (MyErrorno & STACK_ERROR_CAPACITY_LOWER_DEFAULT) {
        Error[6] = STACK_ERROR_CAPACITY_LOWER_DEFAULT;
        fprintf(pointerToDump, "%d ", Error[6]);
    }

    #if defined(CANARY_PROT)
    
        if (MyErrorno & STACK_ERROR_LEFT_CANARY_DIED) {
            Error[7] = STACK_ERROR_LEFT_CANARY_DIED;
            fprintf(pointerToDump, "%d ", Error[7]);
        } if (MyErrorno & STACK_ERROR_RIGHT_CANARY_DIED) {
            Error[8] = STACK_ERROR_RIGHT_CANARY_DIED;
            fprintf(pointerToDump, "%d ", Error[8]);
        } if (MyErrorno & STACK_ERROR_DATA_LEFT_CANARY_DIED) {
            Error[9] = STACK_ERROR_DATA_LEFT_CANARY_DIED;
            fprintf(pointerToDump, "%d ", Error[9]);
        } if (MyErrorno & STACK_ERROR_DATA_RIGHT_CANARY_DIED) {
            Error[10] = STACK_ERROR_DATA_RIGHT_CANARY_DIED;
            fprintf(pointerToDump, "%d ", Error[10]);
        }
    #endif // CANARY_PROT

    #if defined(HASH_PROT)
        if (MyErrorno & STACK_ERROR_WRONG_HASH) {
            Error[11] = STACK_ERROR_WRONG_HASH;
            fprintf(pointerToDump, "%d ", Error[11]);
        }
        
    #endif // HASH_PROT
    
    fprintf(pointerToDump, "\n");    

}

void PrintOfData(stack_t* stk, FILE* fp) {
    #if defined(CANARY_PROT)
    
        fprintf(pointerToDump, "Left data canary %llX\n", ((canary_t*)stk->data)[-1]);
        fprintf(pointerToDump, "Right data canary %llX\n", ((canary_t*)(stk->data + stk->capacity))[0]);
    
    #endif // CANARY_PROT
    
    size_t counter = 0;
        for (; counter < stk->size && counter < stk->capacity; counter++) {
            fprintf(pointerToDump, "[%d] = %d \n", counter, stk->data[counter]);
        }
    for (; counter < stk->capacity; counter++) {
        fprintf(fp, "[%d] = ", counter);
        for (size_t i = 0; i < 4; i++) {
            fputc(*((char*) (stk->data + counter) + i), fp);
        }
        fprintf(fp, "\n");
    }
}

#if defined(HASH_PROT)

    hash_t StackHash(char* ptr, size_t length) {
        hash_t hash = 0;

        for (size_t count = 0; count < (length - (length % 4)); count += 4) {
            for (size_t counter = 0; counter < 4; counter++) {
                ((char*) &hash)[counter] |= ptr[count + counter];
            }
        }
        for (int count = (length - 1 - (length % 4)); count >= 0; count -= 4) {
            for (int counter = 3; counter < 7; counter++) {
                ((char*) &hash)[counter] |= ptr[count - counter];
            }
        }
        hash ^= (size_t) ptr;
        
        return hash;
    }

    bool StackHashCheck(stack_t *stk) {
        hash_t hash = stk->hash;
        stk->hash = 0;
        size_t size = 0;
        
        stk->hash = StackHash((char*) stk, StackSize) + StackHash((char*) stk->data, stk->capacity * sizeof(elem_t));
        
        if (hash == stk->hash) {
            return true;
        }
        
        return false;
    }

#endif // HASH_PROT

