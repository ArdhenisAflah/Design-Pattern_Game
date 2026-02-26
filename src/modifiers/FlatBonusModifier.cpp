#include "FlatBonusModifier.h"

FlatBonusModifier::FlatBonusModifier(int amount) {
    this->bonusAmount = amount;
}

void FlatBonusModifier::Apply(int& score) {
    score += this->bonusAmount; // Menambahkan nilai ke skor
}

std::string FlatBonusModifier::GetName() {
    return "Flat Bonus (+" + std::to_string(bonusAmount) + ")";
}

int FlatBonusModifier::GetCost() {
    return 50; // Harga statis contoh
}