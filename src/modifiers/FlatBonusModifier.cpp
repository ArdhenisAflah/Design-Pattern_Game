#include "FlatBonusModifier.h"

FlatBonusModifier::FlatBonusModifier(int amount) {
    this->bonusAmount = amount;
}

void FlatBonusModifier::Apply(int& score) {
    score += this->bonusAmount; // Menambah Damage ke Virus
}

std::string FlatBonusModifier::GetName() {
    //Tema my game? Yes.
    // Nama baru yang lebih Cyberpunk:2077 (damn game) 
    return "Firewall Script (+" + std::to_string(bonusAmount) + " Dmg)";
}

int FlatBonusModifier::GetCost() {
    return 50; // Harga murah untuk survival awal2
}