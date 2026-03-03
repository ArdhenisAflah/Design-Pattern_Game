#ifndef SIGNALBOOSTER_H
#define SIGNALBOOSTER_H
#include "IHackerCard.h"

class SignalBooster : public IHackerCard {
public:
    std::string GetName() override { return "SignalBooster.exe"; }
    std::string GetDescription() override { return "Enhance 2 cards with Optimized (+30 Base) or Overclocked (+4 Amp)"; }
    int GetCost() override { return 4; }
    void Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) override;
};
#endif