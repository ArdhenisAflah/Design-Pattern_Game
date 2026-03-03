#ifndef IENHANCEMENTEFFECT_H
#define IENHANCEMENTEFFECT_H

#include <string>
#include <vector>
#include "Card.h"

// Interface Utama
class IEnhancementEffect {
public:
    virtual ~IEnhancementEffect() {}
    // Gunain referensi (&) agar bisa mengubah nilai di ScoringSystem secara langsung 
    virtual void Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) = 0;
};

// --- TURUNAN EFEK ENHANCEMENT (Concrete Strategies di Each Class /consumables) ---

class CorruptedEffect : public IEnhancementEffect {
public:
    void Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) override;
};

class FirewallEffect : public IEnhancementEffect {
public:
    void Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) override;
};

class OverclockedEffect : public IEnhancementEffect {
public:
    void Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) override;
};

class OptimizedEffect : public IEnhancementEffect {
public:
    void Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) override;
};

#endif