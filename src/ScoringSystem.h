#ifndef SCORINGSYSTEM_H
#define SCORINGSYSTEM_H

#include <string>
#include "Card.h" // Pastikan Card.h sudah ada
#include <map>
#include <vector>


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

// Struct untuk menyimpan Level dan Bonus tiap tangan
struct HandStats {
    int level = 1;
    int baseChips;      // Chips dasar (Level 1)
    int baseMult;       // Mult dasar (Level 1)
    int perLevelChips;  // Penambahan Chips tiap naik level
    int perLevelMult;   // Penambahan Mult tiap naik level
};

class ScoringSystem {
public:
    // --- FUNGSI UTAMA (PUBLIC) ---
    ScoringSystem(); //Constructor wajib ada untuk isi database
    // Ini fungsi yang dicari RunSession
    int PlayHand(std::vector<Card> playedCards, HandType& outType);

    // Ini fungsi yang dicari error log kamu (CalculateScore & GetHandName)
    int CalculateScore(HandType handType, int sumOfCardValues);
    std::string GetHandName(HandType type);
    // Fungsi yang dipanggil oleh Hacker Card (Level Patch)
    void UpgradeHandLevel(HandType type);

private:

    // Database Statistik tangan.
    std::map<HandType, HandStats> handDatabase;

    // --- FUNGSI PEMBANTU (PRIVATE) ---
    // Wajib dideklarasikan juga meski private!
    HandType DetermineHandType(const std::vector<Card>& cards);
    bool IsFlush(const std::vector<Card>& cards);
    bool IsStraight(const std::vector<Card>& cards);
    // Fungsi inisialisasi data awal (dipanggil di constructor)
    void InitializeHandStats();
};

#endif

