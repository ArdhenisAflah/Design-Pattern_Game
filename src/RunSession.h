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
    
    // Variabel Baru (Ante System)
    int currentAnte;        
    const int MAX_ANTES = 8; 
    const int BASE_TARGET = 100; 

    ScoringSystem* scoringSystem;
    ShopSystem* shopSystem;
    
    std::vector<IModifier*> playerModifiers;
    std::vector<Card> deck;

    // Helper Functions
    void InitializeDeck();
    void ShuffleDeck();
    
    // Fungsi Baru (Gantikan PlayRound)
    void PlayBlind(); 

public:
    RunSession();
    ~RunSession();
    void StartGame();
};

#endif