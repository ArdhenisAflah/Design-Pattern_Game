#include "Antivirus.h"
#include <iostream>

void Antivirus::Trigger(std::vector<Card>& targetDeck, ScoringSystem* scoringSystem, int& integrity) {
    std::cout << "\n[>] RUNNING: SYSTEM RECOVERY..." << std::endl;
    
    int healAmount = 500;
    integrity += healAmount; // Adding Integrity here....
    
    std::cout << "    >> Threats purged." << std::endl;
    std::cout << "    >> Integrity Restored: +" << healAmount << "$" << std::endl;
    std::cout << "    >> Current Integrity: " << integrity << "$" << std::endl;
}