#include "ModifierFactory.h"
#include "modifiers/FlatBonusModifier.h" 
#include "modifiers/DoubleScoreModifier.h"
#include "modifiers/MemoryExpansionModifier.h"
#include <iostream>

IModifier* ModifierFactory::CreateModifier(std::string type) {
    if (type == "flat") {
        // 300 biar lebih kuat buat Survival
        return new FlatBonusModifier(300); 
    } 
    else if (type == "double") {
        // Logika Menggandakan Score
        return new DoubleScoreModifier(); 
    }
    else if (type == "memory") {
        // Logika untuk membuat Memory Expansion
        return new MemoryExpansionModifier();
    }
    
    return nullptr;
}