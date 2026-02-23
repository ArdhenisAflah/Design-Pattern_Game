#include "ScoringSystem.h"

int ScoringSystem::CalculateScore(HandType handType, int sumOfCardValues) {
    int baseChips = 0;
    int baseMult = 0;

    // Menentukan Base Chips & Mult berdasarkan jenis playing hands
    switch (handType) {
        case HIGH_CARD:
            baseChips = 5; baseMult = 1; 
            break;
        case PAIR:
            baseChips = 15; baseMult = 2; 
            break;
        case TWO_PAIR:
            baseChips = 25; baseMult = 2;
            break;
        case THREE_OF_A_KIND:
            baseChips = 35; baseMult = 3; 
            break;
        case STRAIGHT:
            baseChips = 40; baseMult = 4; 
            break;
        case FLUSH:
            baseChips = 50; baseMult = 4; 
            break;
        case FULL_HOUSE:
            baseChips = 60; baseMult = 4; 
            break;
        case FOUR_OF_A_KIND:
            baseChips = 80; baseMult = 8; 
            break;
        case STRAIGHT_FLUSH:
            baseChips = 120; baseMult = 10; 
            break;
        default:
            baseChips = 5; baseMult = 1;
            break;
    }

    // Rumus Dasar: (Base Chips + Nilai Kartu) * Multiplier
    // Hitungan Contoh: (15 + 10) * 2 = 50 Poin.
    return (baseChips + sumOfCardValues) * baseMult;
}

std::string ScoringSystem::GetHandName(HandType type) {
    switch (type) {
        case HIGH_CARD: return "High Card";
        case PAIR: return "Pair";
        case TWO_PAIR: return "Two Pair";
        case THREE_OF_A_KIND: return "Three of a Kind";
        case STRAIGHT: return "Straight";
        case FLUSH: return "Flush";
        case FULL_HOUSE: return "Full House";
        case FOUR_OF_A_KIND: return "Four of a Kind";
        case STRAIGHT_FLUSH: return "Straight Flush";
        default: return "Unknown";
    }
}