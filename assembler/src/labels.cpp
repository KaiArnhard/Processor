#include "AsmFunctions.h"
#include "types.h"

LabelInf* CtorLabel(label_t* label) {
    label->inform = (LabelInf*) calloc(DefaultLabelSize, sizeof(LabelInf));
    label->size = DefaultLabelSize;
    label->counter = 0;
    assert(label->inform != nullptr && "Pointer to array of labels is null");
    for (size_t counter = 0; counter < label->size; counter++) {
        label->inform[counter].DestAddres = DefaultLabelAdress;
    }
    return label->inform;
}

LabelInf* ResizeLabel(label_t* label) {
    label->size *= ResizeLabelConst; 
    label->inform = (LabelInf*) realloc(label->inform, sizeof(LabelInf) * label->size);
    assert(label->inform != nullptr && "Pointer to array of labels is null");
    for (size_t counter = label->size / ResizeLabelConst; counter < label->size; counter++) {
        label->inform[counter].DestAddres = DefaultLabelAdress;
    }
    
    return label->inform;
}

int MakeLabel(label_t* label, char* NameOfLabel, size_t NumbOfComs) {
    if (label->counter >= label->size) {
        label->inform = ResizeLabel(label);
    } else if (label->counter >= MaxLabelSize - 1) {
        printf(RED "Labels are full\n");
    } 
    for (size_t counter = 0; counter < label->counter; counter++) {
        if (!strcmp(label->inform[counter].LabelName, NameOfLabel)) {
            return label->counter;
        } 
    }
    strcpy(label->inform[label->counter].LabelName, NameOfLabel);
    label->inform[label->counter].DestAddres = NumbOfComs;
    label->counter++;
    return label->counter;
}

int LabelCheck(const char* NameOfLabel, label_t* label) {
    for (size_t counter = 0; counter < label->counter; counter++) {
        if (!strcmp(NameOfLabel, label->inform[counter].LabelName)) {
            return label->inform[counter].DestAddres;
        }
    }
    return DefaultLabelAdress;
}

void PrintOfLabels(label_t* label) {
    for (size_t counter = 0; counter < label->counter; counter++) {
        printf("%d  %s  %d\n", counter, label->inform[counter].LabelName, label->inform[counter].DestAddres);
    }
    printf("Label counter %d\n", label->counter);
}