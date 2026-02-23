#ifndef SCORINGSYSTEM_H
#define SCORINGSYSTEM_H

#include <string>

// Enum untuk jenis-jenis tangan poker.
enum HandType {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH
};

class ScoringSystem {
public:
    // Fungsi menghitung skor
    // handType: Jenis tangan (misal: PAIR)
    // sumOfCardValues: Total angka di kartu (misal: kartu 5 dan 5 = 10)
    int CalculateScore(HandType handType, int sumOfCardValues);

    // Bikin Helper untuk ngubah Enum ke String (for display)
    std::string GetHandName(HandType type);
};

#endif