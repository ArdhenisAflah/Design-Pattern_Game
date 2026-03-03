#include "IEnhancementEffect.h"
#include <cstdlib>

// [CRP] Corrupted: x2 Amp & Risiko Pecah 80%
void CorruptedEffect::Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) {
    bonusMult *= 2.0f; 
    if ((std::rand() % 100) < 80) isBroken = true; 
}

// [FWL] Firewall: Nilai tetap 50, Rank diabaikan
void FirewallEffect::Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) {
    val = 50; 
}

// [OVC] Overclocked: +4 Amp
void OverclockedEffect::Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) {
    extraAmp += 4;
}

// [OPT] Optimized: +30 Base Code
void OptimizedEffect::Execute(int& val, float& bonusMult, int& extraAmp, bool& isBroken) {
    val += 30;
}