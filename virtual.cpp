
#include "virtual.hpp"
#include <iostream>

CPointer vt::GetMethod(CPointer base, uint32_t index) {
    if (!base.IsValid()) {
        std::cout << "shevecadet virtualur methodis gamodzaxeba null klasidan." << std::endl;
        return 0;
    }

    CPointer VTable = base.GetField<uintptr_t>(0);
    if (!VTable.IsValid()) {
        std::cout << "shevecadet virtualuri metodidan vtables gamodzaxeba" << std::endl;
        return 0;
    }

    return VTable.GetField<uintptr_t>(index * sizeof(uintptr_t));
}