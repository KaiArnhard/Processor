#include "proc.h"

int main() {
    SPU_t proc = {};
    FILE* PtrToCM = fopen(PathToCm1, "r");

    SPUCtor(&proc, PtrToCM);

    VirtualMachine(&proc, PtrToCM);
    
    SPUDtor(&proc);
    return 0;
}