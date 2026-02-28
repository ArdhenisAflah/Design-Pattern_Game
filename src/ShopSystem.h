#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include <vector>
#include "modifiers/IModifier.h"
#include "consumables/IHackerCard.h"
#include "Card.h"
#include "ScoringSystem.h"

class ShopSystem {
private:
    std::vector<IModifier*> jokerStock; //Etalase Joker Card
    std::vector<IHackerCard*> hackerStock;   //  Etalase Hacker Card
    const int REROLL_COST = 20; // Biaya Reroll

    void ClearStocks(); // Bersihkan kedua Etalase

public:
    ~ShopSystem();
    
    // Generate isi toko (Dipanggil saat start game atau saat Reroll)
    void GenerateStock(); 

    // Buka menu toko
    void OpenShop(int& money, std::vector<IModifier*>& inventory, std::vector<Card>& deck, ScoringSystem* scoringSystem);
};

#endif