#include "DoubleScoreModifier.h"

void DoubleScoreModifier::Apply(int& score) {
    // Logika: Kali 2 skor saat ini
    score *= 2; 
}

std::string DoubleScoreModifier::GetName() {
    return "Double Score (x2)";
}

int DoubleScoreModifier::GetCost() {
    return 150; // Contoh harga lebih mahal karena efeknya kuat
}