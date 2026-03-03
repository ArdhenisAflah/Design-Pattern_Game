#ifndef GLASSINJECTION_H
#define GLASSINJECTION_H

#include "IHackerCard.h"

class GlassInjection : public IHackerCard {
public:
    std::string GetName() override { return "Fragmenter.exe"; } // Nama Baru
    std::string GetDescription() override { return "Overclock 2 packets (2x Dmg, chance to break)"; }
    int GetCost() override { return 4; } 

    void Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) override;
};

#endif