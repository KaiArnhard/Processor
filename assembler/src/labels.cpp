#include "asm.h"
#include "types.h"

label_t* CtorLabel(label_t* label) {
    label = (label_t*) calloc(DefaultLabelSize, sizeof(label_t));
    label->LabelSize = DefaultLabelSize;
    assert(label != nullptr && "Pointer to array of labels is null");
    for (size_t counter = 0; counter < label->LabelSize; counter++) {
        label[counter].DestAddres = DefaultLabelAdress;
    }
    return label;
}

label_t* ResizeLabel(label_t* label) {
    label->LabelSize *= ResizeLabelConst; 
    label = (label_t*) realloc(label, sizeof(label_t) * label->LabelSize);
    assert(label != nullptr && "Pointer to array of labels is null");
    for (size_t counter = DefaultLabelSize; counter < label->LabelSize; counter++) {
        label[counter].DestAddres = -1;
    }
    
    return label;
}