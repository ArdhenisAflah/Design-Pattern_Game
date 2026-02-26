#include "ModifierFactory.h"
#include "modifiers/FlatBonusModifier.h" // Isinya FlatBonusModifier
#include "modifiers/DoubleScoreModifier.h" // Isinya DoubleScoreModifier
#include <iostream>

IModifier* ModifierFactory::CreateModifier(std::string type) {
    if (type == "flat") {
        // Ini dari file ModifierA
        return new FlatBonusModifier(100); 
    } 
    else if (type == "double") {
        // Ini dari file ModifierB, tapi nama classnya DoubleScoreModifier
        return new DoubleScoreModifier(); 
    }
    
    return nullptr;
}