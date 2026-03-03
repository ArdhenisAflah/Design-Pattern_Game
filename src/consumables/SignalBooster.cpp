#include "SignalBooster.h"
#include <iostream>

void SignalBooster::Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) {
    for (int i = 0; i < 2; i++) {
        if (targetDeck.empty()) break;
        int idx = rand() % targetDeck.size();
        // Random antara Optimized atau Overclocked
        targetDeck[idx].enhancement = (rand() % 2 == 0) ? OPTIMIZED : OVERCLOCKED;
        std::cout << "    >> Signal Boosted: " << targetDeck[idx].ToString() << std::endl;
    }
}