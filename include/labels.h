#if !defined(LABELS_H)
#define LABELS_H

#include "types.h"

struct labelInf {
    char LabelName[100];
    int  DestAddres;
};

struct label_t {
    size_t counter;
    size_t size;
    labelInf* inform;
};

labelInf* CtorLabel (label_t* label);
labelInf* ResizeLabel(label_t* label);

int LabelCheck(const char* NameOfLabel, label_t* label);
int MakeLabel(label_t* label, char* NameOfLabel, size_t NumbOfComs);
void PrintOfLabels(label_t* label);

#endif // LABELS_H
