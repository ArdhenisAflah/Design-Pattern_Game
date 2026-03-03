#ifndef ANTIVIRUS_H
#define ANTIVIRUS_H

#include "IHackerCard.h"

class Antivirus : public IHackerCard {
public:
    std::string GetName() override { return "Antivirus.exe"; }
    std::string GetDescription() override { return "Restores +500 Integrity (HP)"; }
    int GetCost() override { return 8; } // Agak mahal karena nyawa

    void Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) override;
};

#endif