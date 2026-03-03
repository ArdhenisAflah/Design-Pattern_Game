#ifndef MEMORYEXPANSIONMODIFIER_H
#define MEMORYEXPANSIONMODIFIER_H

#include "IModifier.h"

// Script ini menambah kapasitas Hand (RAM) dan memberikan sedikit buff damage
class MemoryExpansionModifier : public IModifier {
public:
    void Apply(int& score) override;
    std::string GetName() override;
    int GetCost() override;
    
    // Override fungsi untuk menambah Hand Size
    int GetHandSizeBonus() override;
};

#endif