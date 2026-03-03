#include "LevelPatch.h"
#include <iostream>

LevelPatch::LevelPatch(HandType hand, std::string name) {
    this->targetHand = hand;
    this->handName = name;
}

void LevelPatch::Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) {
    std::cout << "\n[>] SYSTEM UPDATE INITIATED..." << std::endl;
    
    // Naikkan Level
    scoringSystem->UpgradeHandLevel(targetHand); 
    
    std::cout << "    >> Optimization Complete: " << handName << " algorithm updated." << std::endl;
    std::cout << "    >> Damage Output Increased." << std::endl;
}