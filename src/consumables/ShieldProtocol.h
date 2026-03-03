#ifndef SHIELDPROTOCOL_H
#define SHIELDPROTOCOL_H
#include "IHackerCard.h"

class ShieldProtocol : public IHackerCard {
public:
    std::string GetName() override { return "Shield_v2.exe"; }
    std::string GetDescription() override { return "Convert 1 card to FIREWALL (50 Base Dmg, ignores Rank)"; }
    int GetCost() override { return 3; }
    void Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) override;
};
#endif