#include "DoubleScoreModifier.h"

void DoubleScoreModifier::Apply(int& score) {
    // Kali 2 Damage Affect score 
    score = (int)(score * 2.4f);
}

std::string DoubleScoreModifier::GetName() {
    // Nama baru, sesuaikan tema, sama efeknya ditingkatkan sedikit.
    return "Overclock Daemon (x2.4 Dmg)";
}

int DoubleScoreModifier::GetCost() {
    return 400; // Mahal tapi wajib punya untuk late game
}