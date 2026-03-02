#ifndef LEVELPATCH_H
#define LEVELPATCH_H

#include "IHackerCard.h"
#include "../ScoringSystem.h" // Kita butuh akses enum HandType

class LevelPatch : public IHackerCard {
private:
    HandType targetHand; // Jenis tangan yang mau dinaikkan (misal: Pair)
    std::string handName;

public:
    LevelPatch(HandType hand, std::string name);
    
    std::string GetName() override { return "Patch: " + handName; }
    std::string GetDescription() override { return "Level up " + handName; }
    int GetCost() override { return 5; }

    void Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem) override;
};

#endif