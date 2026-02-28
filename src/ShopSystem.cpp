#include "ShopSystem.h"
#include "ModifierFactory.h"
#include "HackerFactory.h"
#include <iostream>
#include <cstdlib>

ShopSystem::~ShopSystem() {
    ClearStocks();
}

void ShopSystem::ClearStocks() {
    for (auto* item : jokerStock) delete item;
    jokerStock.clear();
    
    for (auto* item : hackerStock) delete item;
    hackerStock.clear();
}

void ShopSystem::GenerateStock() {
    ClearStocks(); // Bersihkan yang lama dulu

    std::cout << "\n[!] Shop Restocked (Jokers & Hacker Cards)..." << std::endl;

    // Generate 2 barang acak (Bisa diatur peluangnya di sini)
    for (int i = 0; i < 2; i++) {
        int rng = rand() % 100; 
        if (rng < 70) jokerStock.push_back(ModifierFactory::CreateModifier("flat"));
        else jokerStock.push_back(ModifierFactory::CreateModifier("double"));
    }

    // 2. Generate 2 Hacker Card Acak
    for (int i = 0; i < 2; i++) {
        hackerStock.push_back(HackerFactory::CreateRandomHackerCard());
    }
}

void ShopSystem::OpenShop(int& money, std::vector<IModifier*>& inventory, 
                          std::vector<Card>& deck, ScoringSystem* scoringSystem) {
  
    
    bool shopping = true;
    while (shopping) {
        std::cout << "\n=== SHOP (Gold: " << money << ") ===" << std::endl;
        
       // --- DISPLAY JOKERS (Pilihan 1-2) ---
        std::cout << "--- JOKERS (Passive) ---" << std::endl;
        if (jokerStock.empty()) std::cout << "   (Out of stock)" << std::endl;
        for (int i = 0; i < jokerStock.size(); i++) {
            std::cout << (i + 1) << ". Buy " << jokerStock[i]->GetName() 
                      << " ($" << jokerStock[i]->GetCost() << ")" << std::endl;
        }

        // --- DISPLAY HACKER CARDS (Pilihan 3-4) ---
        std::cout << "--- HACKER CARDS (Consumable) ---" << std::endl;
        if (hackerStock.empty()) std::cout << "   (Out of stock)" << std::endl;
        for (int i = 0; i < hackerStock.size(); i++) {
            // Offset index biar lanjut dari joker (misal Joker ada 2, berarti ini nomor 3)
            int displayNum = i + 1 + jokerStock.size(); 
            std::cout << displayNum << ". Buy " << hackerStock[i]->GetName() 
                      << " ($" << hackerStock[i]->GetCost() << ")" << std::endl;
        }

        std::cout << "R. Reroll ($" << REROLL_COST << ")\nE. Exit\nChoice: ";

        char choice;
        std::cin >> choice;

        if (isdigit(choice)) {
            int selection = (choice - '0'); // Konversi char ke int (1-based)
            int jokerCount = jokerStock.size();
            int hackerCount = hackerStock.size();

            // LOGIKA BELI JOKER
            if (selection >= 1 && selection <= jokerCount) {
                int idx = selection - 1;
                IModifier* item = jokerStock[idx];
                if (money >= item->GetCost()) {
                    money -= item->GetCost();
                    inventory.push_back(item);
                    
                    // Hapus dari rak (tanpa delete memory objectnya)
                    jokerStock.erase(jokerStock.begin() + idx); 
                    std::cout << ">> Purchased Joker: " << item->GetName() << "!" << std::endl;
                } else std::cout << ">> Not enough gold!" << std::endl;
            }
            // LOGIKA BELI HACKER CARD
            else if (selection > jokerCount && selection <= (jokerCount + hackerCount)) {
                int idx = selection - 1 - jokerCount; // Index relatif terhadap vector hackerStock
                IHackerCard* item = hackerStock[idx];
                
                if (money >= item->GetCost()) {
                    money -= item->GetCost();
                    
                    // LANGSUNG PAKAI (TRIGGER)
                    // Di Balatro asli disimpan dulu, tapi biar simpel kita langsung pakai saat beli
                    item->Trigger(deck, scoringSystem);
                    
                    // Hapus Memory (karena Consumable sekali pakai)
                    delete item; 
                    hackerStock.erase(hackerStock.begin() + idx);
                } else std::cout << ">> Not enough gold!" << std::endl;
            }
        }
        // Logika Reroll
        else if (choice == 'r' || choice == 'R') {
            if (money >= REROLL_COST) {
                money -= REROLL_COST;
                GenerateStock(); // Panggil Factory lagi!
            } else {
                std::cout << ">> Not enough gold to reroll!" << std::endl;
            }
        }
        // Logika Exit
        else if (choice == 'e' || choice == 'E') {
            shopping = false;
        }
    }
}