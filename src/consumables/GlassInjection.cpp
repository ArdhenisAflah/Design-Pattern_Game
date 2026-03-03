#include "GlassInjection.h"
#include <cstdlib> 
#include <iostream>

void GlassInjection::Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) {
    if (targetDeck.empty()) return;

    std::cout << "\n[>] RUNNING: FRAGMENTER.EXE..." << std::endl;

    for (int i = 0; i < 2; i++) {
        int idx = rand() % targetDeck.size();
        
        // [UPDATE] Gunakan Enum Baru: CORRUPTED
        targetDeck[idx].enhancement = CORRUPTED; 
        
        std::cout << "    >> Corrupting data sector: " << targetDeck[idx].ToString() 
                  << " (Status: UNSTABLE/HIGH_AMP)" << std::endl;
    }
}