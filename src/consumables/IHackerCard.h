#ifndef IHACKERCARD_H
#define IHACKERCARD_H

#include <string>
#include <vector>
#include "../Card.h"

// Forward Declaration (Biar tidak error circular dependency)
class ScoringSystem; 

class IHackerCard {
public:
    virtual ~IHackerCard() {}
    
    virtual std::string GetName() = 0;
    virtual std::string GetDescription() = 0;
    virtual int GetCost() = 0;

    // FUNGSI UTAMA: Memicu efek kartu
    // targetDeck: Referensi ke deck pemain (untuk diacak-acak)
    // scoringSystem: Referensi ke sistem skor (untuk upgrade level)
    virtual void Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem) = 0;
};

#endif