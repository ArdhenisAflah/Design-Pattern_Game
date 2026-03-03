#include "MemoryExpansionModifier.h"

void MemoryExpansionModifier::Apply(int& score) {
    // Karena fokusnya ke kapasitas tangan, buff damage-nya kecil saja
    score += 50; 
}

std::string MemoryExpansionModifier::GetName() {
    return "RAM Upgrade (+2 Hand Size, +50 Dmg)";
}

int MemoryExpansionModifier::GetCost() {
    return 120; // Harganya menengah, ga mahal ga murah.
}

int MemoryExpansionModifier::GetHandSizeBonus() {
    // Memberikan 2 ekstra kartu saat Draw!
    return 2; 
}