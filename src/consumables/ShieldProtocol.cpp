#include "ShieldProtocol.h"
#include <iostream>

void ShieldProtocol::Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) {
    if (targetDeck.empty()) return;
    int idx = rand() % targetDeck.size();
    targetDeck[idx].enhancement = FIREWALL; // Set status ke Firewall
    std::cout << "\n[>] DEPLOYING FIREWALL: " << targetDeck[idx].ToString() << std::endl;
}