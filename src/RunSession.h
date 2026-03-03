#ifndef RUNSESSION_H
#define RUNSESSION_H

#include <vector>
#include "ScoringSystem.h"
#include "ShopSystem.h"
#include "modifiers/IModifier.h"
#include "Card.h"

class RunSession {
private:
    int money;
    
    // Variabel (WAVE System)
    int currentAnte;        
    const int MAX_WAVES = 8; 
    const int BASE_THREAT = 300; // HP awal Virus (Musuh)

    ScoringSystem* scoringSystem;
    ShopSystem* shopSystem;
    
    std::vector<IModifier*> playerModifiers;
    std::vector<Card> deck;

    // Helper Functions
    void InitializeDeck();
    void ShuffleDeck();
    
    
    void PlayBlind(); 

public:
    RunSession();
    ~RunSession();
    void StartGame();
};

#endif