#include "ScoringSystem.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>

ScoringSystem::ScoringSystem() {
    InitializeHandStats();
    InitializeEffects();
}


ScoringSystem::~ScoringSystem() {
   for (auto const& pair : effectMap) {
        delete pair.second; // pair.second adalah kartu enhancement 'effect' (alias value dari map)
    }
    effectMap.clear();
}

void ScoringSystem::InitializeEffects() {
    effectMap[CORRUPTED] = new CorruptedEffect();
    effectMap[FIREWALL] = new FirewallEffect();
    effectMap[OVERCLOCKED] = new OverclockedEffect();
    effectMap[OPTIMIZED] = new OptimizedEffect();
}
void ScoringSystem::InitializeHandStats() {
    // Balancing untuk Survival Mode 
    // {Level, BaseCode, BaseAmp, UpCode(kalau naik level), UpAmp(kalau naik leveel)}
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

void ScoringSystem::UpgradeHandLevel(HandType type) {
    if (handDatabase.count(type)) {
        handDatabase[type].level++;
        std::cout << "   >>> [UPGRADE] " << GetHandName(type) << " algorithm updated to v" 
                  << handDatabase[type].level << ".0" << std::endl;
    }
}

int ScoringSystem::PlayHand(std::vector<Card> playedCards, HandType& outType, std::vector<Card>& brokenCardIndices) {
    if (playedCards.empty()) {
        outType = HIGH_CARD;
        return 0;
    }

    // 1. Sort kartu berdasarkan Rank -> (easier calculate for later...)
    std::sort(playedCards.begin(), playedCards.end(), [](const Card& a, const Card& b) {
        return a.rank < b.rank;
    });

   // Kita butuh fungsi pendeteksi  yang mengembalikan kartu yang valid saja
    std::vector<Card> scoringCards;
    outType = DetermineScoringCards(playedCards, scoringCards); // this man right here do the jobs so high card only one card counted (highest)

    // 3. Hitung Value Kartu (Base Code)
    int sum = 0;
     //Deklarasi Variabel Penampung dari modifier dan enhancement
    float bonusMult = 1.0f; 
    int extraAmp = 0;

    for (const auto& c : scoringCards) {
        int val = c.rank; // Ambil nilai Enum (2-14)

        // LOGIKA RANK (J,Q,K = 10, A = 11)
        if (val >= JACK && val <= KING) {
            val = 10;
        }
        if (val == ACE) {
            val = 11;
        }
        bool isBroken = false; 
       // Cek apakah kartu memiliki enhancement dan eksekusi strateginya
        if (effectMap.count(c.enhancement)) {
            // Memanggil strategi yang sesuai (Execute) secara dinamis
            effectMap[c.enhancement]->Execute(val, bonusMult, extraAmp, isBroken);
        }


        // Pastikan kartu yang pecah dicatat di sini agar bisa dihapus oleh RunSession
        if (isBroken) {
            brokenCardIndices.push_back(c);
            std::cout << "\033[31m" << "   [!] CRITICAL: " << c.ToString() 
                      << " EXCEEDED LOAD! EXPLODING..." << "\033[0m" << std::endl;
        }
        
        sum += val;
    }

    // 4. Hitung Skor Dasar (Base Damage)
    int finalDamage = CalculateScore(outType, sum);
    

    // Jika ada yang pecah, tambahkan "Explosion Damage" (Misal: 500 fixed damage per kartu)
    if (!brokenCardIndices.empty()) {
        int explosionDmg = brokenCardIndices.size() * 500;
        finalDamage += explosionDmg;
        std::cout << "   [BOOM] Data Leak Explosion: +" << explosionDmg << " DMG" << std::endl;
    }


    // [APLIKASI OVERCLOCKED] (+Amp)
    // hitung manual tambahan damage-nya karena CalculateScore sudah jalan duluan
    if (extraAmp > 0) {
        // Ambil info stats tangan saat ini untuk tahu total base code-nya
        HandStats stats = handDatabase[outType];
        int currentBaseCode = stats.baseDmg + (stats.upgradeDmg * (stats.level - 1));
        int totalCode = currentBaseCode + sum; // Total (Chips)
        
        int overclockBonus = totalCode * extraAmp; // Damage tambahan dari Overclock
        finalDamage += overclockBonus;

        std::cout << "   [MOD] OVERCLOCKED: +" << extraAmp << " Amp -> +" 
                  << overclockBonus << " DMG" << std::endl;
    }

    // [APLIKASI CORRUPTED] (xMult)
    if (bonusMult > 1.0f) {
        int oldDmg = finalDamage;
        finalDamage = (int)(finalDamage * bonusMult);
        
        std::cout << "   [AMP] CORRUPTED PROTOCOL: " << oldDmg 
                  << " x " << std::fixed << std::setprecision(1) << bonusMult 
                  << " = " << finalDamage << " DMG" << std::endl;
    }

    return finalDamage;
}

int ScoringSystem::CalculateScore(HandType handType, int sumOfCardValues) {
    HandStats stats = handDatabase[handType];

    // Rumus: Base + (LevelBonus * (Level - 1))
    int currentBase = stats.baseDmg + (stats.upgradeDmg * (stats.level - 1));
    int currentAmp  = stats.baseAmp + (stats.upgradeAmp * (stats.level - 1));

    // Total Base = Base Hand + Nilai Kartu
    int totalBase = currentBase + sumOfCardValues;
    
    // Final Calculation
    int finalDamage = totalBase * currentAmp;

    // Output Cyberpunk Style
    std::cout << "   [CALC] " << GetHandName(handType) << " v" << stats.level 
              << " :: " << totalBase << " (Code) x " << currentAmp << " (Amp) = " 
              << finalDamage << " DMG";

    return finalDamage;
}

// --- LOGIKA DETEKSI PLAYED HANDS ---
HandType ScoringSystem::DetermineHandType(const std::vector<Card>& cards) {
    bool flush = IsFlush(cards);
    bool straight = IsStraight(cards);

    if (flush && straight) return STRAIGHT_FLUSH;

    std::map<int, int> counts;
    for (const auto& c : cards) counts[c.rank]++;

    bool four = false, three = false;
    int pairs = 0;

    for (auto const& pair : counts) {
        if (pair.second == 4) four = true;
        if (pair.second == 3) three = true;
        if (pair.second == 2) pairs++;
    }

    if (four) return FOUR_OF_A_KIND;
    if (three && pairs >= 1) return FULL_HOUSE;
    if (flush) return FLUSH;
    if (straight) return STRAIGHT;
    if (three) return THREE_OF_A_KIND;
    if (pairs >= 2) return TWO_PAIR;
    if (pairs == 1) return PAIR;
    return HIGH_CARD;
}

bool ScoringSystem::IsFlush(const std::vector<Card>& cards) {
    if (cards.size() < 5) return false;
    Suit first = cards[0].suit;
    for (size_t i = 1; i < cards.size(); i++) {
        if (cards[i].suit != first) return false;
    }
    return true;
}

bool ScoringSystem::IsStraight(const std::vector<Card>& cards) {
    if (cards.size() < 5) return false;
    for (size_t i = 0; i < cards.size() - 1; i++) {
        if (cards[i+1].rank != cards[i].rank + 1) return false;
    }
    return true;
}

std::string ScoringSystem::GetHandName(HandType type) {
    switch (type) {
        case HIGH_CARD:       return "Protocol: High Card";
        case PAIR:            return "Protocol: Pair";
        case TWO_PAIR:        return "Protocol: Two Pair";
        case THREE_OF_A_KIND: return "Protocol: 3-Kind";
        case STRAIGHT:        return "Protocol: Straight";
        case FLUSH:           return "Protocol: Flush";
        case FULL_HOUSE:      return "Protocol: Full House";
        case FOUR_OF_A_KIND:  return "Protocol: 4-Kind";
        case STRAIGHT_FLUSH:  return "Protocol: Str. Flush";
        default: return "Unknown Protocol";
    }
}

HandType ScoringSystem::DetermineScoringCards(const std::vector<Card>& allCards, std::vector<Card>& scoringCards) {
    bool flush = IsFlush(allCards);
    bool straight = IsStraight(allCards);

    // Jika Flush atau Straight, semua kartu (5 kartu) dihitung
    if (flush || straight) {
        scoringCards = allCards;
        if (flush && straight) return STRAIGHT_FLUSH;
        return flush ? FLUSH : STRAIGHT;
    }

    std::map<int, std::vector<Card>> groups;
    for (const auto& c : allCards) groups[c.rank].push_back(c);

    bool four = false, three = false;
    std::vector<Card> pairs;
    std::vector<Card> trips;
    std::vector<Card> quads;

    for (auto const& item : groups) {
        if (item.second.size() == 4) quads = item.second;
        else if (item.second.size() == 3) trips = item.second;
        else if (item.second.size() == 2) {
            pairs.insert(pairs.end(), item.second.begin(), item.second.end());
        }
    }

    // Cek Hierarki dan masukkan hanya kartu yang relevan
    if (!quads.empty()) {
        scoringCards = quads; // Hanya 4 kartu yang sama yang dihitung
        return FOUR_OF_A_KIND;
    }
    if (!trips.empty() && !pairs.empty()) {
        scoringCards = trips;
        scoringCards.insert(scoringCards.end(), pairs.begin(), pairs.end());
        return FULL_HOUSE;
    }
    if (!trips.empty()) {
        scoringCards = trips; // Hanya 3 kartu yang sama
        return THREE_OF_A_KIND;
    }
    if (pairs.size() >= 4) { // Two Pair
        scoringCards = pairs; 
        return TWO_PAIR;
    }
    if (pairs.size() == 2) { // One Pair
        scoringCards = pairs; // HANYA 2 KARTU INI YANG DIHITUNG
        return PAIR;
    }

    // Jika High Card, hanya kartu tertinggi yang dihitung
    scoringCards.push_back(allCards.back());
    return HIGH_CARD;
}