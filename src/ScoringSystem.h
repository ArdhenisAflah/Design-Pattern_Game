#ifndef SCORINGSYSTEM_H
#define SCORINGSYSTEM_H

#include <string>
#include <vector>
#include <map>
#include "Card.h" 
#include "IEnhancementEffect.h"

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

// [FIX] Update nama variabel agar sesuai dengan .cpp
struct HandStats {
    int level = 1;
    int baseDmg;        // Dulu baseChips
    int baseAmp;        // Dulu baseMult
    int upgradeDmg;     // Dulu perLevelChips
    int upgradeAmp;     // Dulu perLevelMult
};

class ScoringSystem {
public:
    ScoringSystem(); 
    ~ScoringSystem();
    int PlayHand(std::vector<Card> playedCards, HandType& outType, std::vector<Card>& brokenCardIndices);
    int CalculateScore(HandType handType, int sumOfCardValues);
    std::string GetHandName(HandType type);
    void UpgradeHandLevel(HandType type);

private:
    std::map<HandType, HandStats> handDatabase;
    std::map<CardEnhancement, IEnhancementEffect*> effectMap;

    void InitializeHandStats();
    void InitializeEffects();
    HandType DetermineHandType(const std::vector<Card>& cards);
    HandType DetermineScoringCards(const std::vector<Card>& allCards, std::vector<Card>& scoringCards);
    bool IsFlush(const std::vector<Card>& cards);
    bool IsStraight(const std::vector<Card>& cards);
};

#endif