#include "ScoringSystem.h"
#include <map>
#include <algorithm>
#include <iostream>


// Constructor: Inisialisasi Database saat game mulai
ScoringSystem::ScoringSystem() {
    InitializeHandStats();
}

void ScoringSystem::InitializeHandStats() {
    // Format: {Level, BaseChips, BaseMult, UpgradeChips, UpgradeMult}
    // Angka disesuaikan mirip Balatro (Planet Cards)
    handDatabase[HIGH_CARD]       = {1, 5,  1,  10, 1};
    handDatabase[PAIR]            = {1, 10, 2,  15, 1};
    handDatabase[TWO_PAIR]        = {1, 20, 2,  20, 1};
    handDatabase[THREE_OF_A_KIND] = {1, 30, 3,  20, 2};
    handDatabase[STRAIGHT]        = {1, 30, 4,  30, 2};
    handDatabase[FLUSH]           = {1, 35, 4,  25, 2};
    handDatabase[FULL_HOUSE]      = {1, 40, 4,  25, 2};
    handDatabase[FOUR_OF_A_KIND]  = {1, 60, 7,  30, 3};
    handDatabase[STRAIGHT_FLUSH]  = {1, 100, 8, 40, 4};
}

// Fungsi Upgrade Level
void ScoringSystem::UpgradeHandLevel(HandType type) {
    if (handDatabase.count(type)) {
        handDatabase[type].level++;
        // Feedback visual
        std::cout << "   >>> " << GetHandName(type) << " Upgraded to Level " 
                  << handDatabase[type].level << "!" << std::endl;
    }
}


// --- FUNGSI UTAMA YANG DIPANGGIL RUNSESSION ---
int ScoringSystem::PlayHand(std::vector<Card> playedCards, HandType& outType) {
    if (playedCards.empty()) {
        outType = HIGH_CARD;
        return 0;
    }

    // 1. Urutkan kartu dari kecil ke besar (Penting untuk Straight)
    std::sort(playedCards.begin(), playedCards.end(), [](const Card& a, const Card& b) {
        return a.rank < b.rank;
    });

    // 2. Deteksi Tipe Tangan (Logic Cerdas di sini)
    outType = DetermineHandType(playedCards);

    // 3. Hitung Total Nilai Kartu (Chips dasar dari kartu)
    int sum = 0;
    for (const auto& c : playedCards) {
        // Aturan Balatro: 2-9 sesuai angka, 10, J, Q, K bernilai 10. As bernilai 11.
        int val = c.rank;
        if (val >= 10 && val < 14) val = 10;
        if (val == 14) val = 11;
        
        sum += val;
    }

    // 4. Hitung Skor Akhir (Base Chips + Sum) * Mult
    return CalculateScore(outType, sum);
}

// --- LOGIKA DETEKSI TANGAN (STRATEGY PATTERN CORE) ---
HandType ScoringSystem::DetermineHandType(const std::vector<Card>& cards) {
    bool flush = IsFlush(cards);
    bool straight = IsStraight(cards);

    // 1. Cek Straight Flush (Kasta Tertinggi)
    if (flush && straight) return STRAIGHT_FLUSH;

    // 2. Analisis Frekuensi (Untuk Pair, Full House, 4-kind, dll)
    std::map<int, int> counts;
    for (const auto& c : cards) {
        counts[c.rank]++;
    }

    bool fourOfAKind = false;
    bool threeOfAKind = false;
    int pairCount = 0;

    // --- BAGIAN INI SAYA UBAH AGAR KOMPATIBEL SAMA SEMUA COMPILER ---
    // Mengganti Structured Binding [rank, count] dengan iterator biasa
    for (auto const& pair : counts) {
        // int rank = pair.first; // Kita tidak butuh rank-nya di logic ini
        int count = pair.second;  // Kita cuma butuh jumlahnya

        if (count == 4) fourOfAKind = true;
        if (count == 3) threeOfAKind = true;
        if (count == 2) pairCount++;
    }
    // -------------------------------------------------------------

    // 3. Cek Hierarki dari atas ke bawah
    if (fourOfAKind) return FOUR_OF_A_KIND;
    if (threeOfAKind && pairCount >= 1) return FULL_HOUSE; // 3 + 2
    if (flush) return FLUSH;
    if (straight) return STRAIGHT;
    if (threeOfAKind) return THREE_OF_A_KIND;
    if (pairCount >= 2) return TWO_PAIR;
    if (pairCount == 1) return PAIR;

    // Kalau tidak ada pola apa-apa
    return HIGH_CARD;
}

// --- HELPER FUNCTIONS ---

bool ScoringSystem::IsFlush(const std::vector<Card>& cards) {
    // Flush minimal 5 kartu di Balatro (biasanya)
    if (cards.size() < 5) return false;

    Suit firstSuit = cards[0].suit;
    for (size_t i = 1; i < cards.size(); i++) {
        if (cards[i].suit != firstSuit) return false;
    }
    return true;
}

bool ScoringSystem::IsStraight(const std::vector<Card>& cards) {
    // Straight minimal 5 kartu
    if (cards.size() < 5) return false;

    // Cek urutan angka (Ingat, kartu sudah di-sort di awal fungsi PlayHand)
    for (size_t i = 0; i < cards.size() - 1; i++) {
        // Kalau kartu selanjutnya TIDAK tepat +1 dari kartu sekarang, gagal straight
        if (cards[i+1].rank != cards[i].rank + 1) {
            return false;
        }
    }
    return true;
}

// --- KALKULASI SKOR ---
int ScoringSystem::CalculateScore(HandType handType, int sumOfCardValues) {
    //ambil data dari database
    HandStats stats = handDatabase[handType];


   // Rumus: Base + (LevelBonus * (Level - 1))
    int currentChips = stats.baseChips + (stats.perLevelChips * (stats.level - 1));
    int currentMult  = stats.baseMult  + (stats.perLevelMult  * (stats.level - 1));

   // Total Chips = Chips Hand + Chips Kartu
    int totalChips = currentChips + sumOfCardValues;

    std::cout << "   (Lvl." << stats.level << " " << GetHandName(handType) 
              << ": " << currentChips << "x" << currentMult << ") ";

    return totalChips * currentMult;
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