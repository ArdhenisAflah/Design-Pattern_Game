#include "ShopSystem.h"
#include "ModifierFactory.h"
#include "HackerFactory.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>

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
    ClearStocks(); 
    std::cout << "\n[!] CONNECTING TO DARK WEB VENDOR..." << std::endl;
    std::cout << "[!] NEW STOCK GENERATED." << std::endl;

    // 1. Generate 3 Script (Joker)
    for (int i = 0; i < 2; i++) {
        int rng = rand() % 100; 
        // 50% Peluang Firewall (Flat Bonus)
        if (rng < 50) {
            jokerStock.push_back(ModifierFactory::CreateModifier("flat"));
        }
        // 30% Peluang RAM Upgrade (Memory)
        else if (rng < 80) {
            jokerStock.push_back(ModifierFactory::CreateModifier("memory"));
        }
        // 20% Peluang Overclock (Double Score)
        else {
            jokerStock.push_back(ModifierFactory::CreateModifier("double"));
        }
    }

    // 2. Generate 2 Hacker Protocols
    for (int i = 0; i < 2; i++) {
        hackerStock.push_back(HackerFactory::CreateRandomHackerCard());
    }
}

void ShopSystem::OpenShop(int& money, std::vector<IModifier*>& inventory, 
                          std::vector<Card>& deck, ScoringSystem* scoringSystem) {
    
    bool shopping = true;
    while (shopping) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "   DARK WEB VENDOR (INTEGRITY: " << money << ")   " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "WARNING: Spending Integrity reduces your current defense!" << std::endl;
        
        // --- DISPLAY SCRIPTS (Jokers) ---
        std::cout << "\n--- DEFENSE SCRIPTS (Passive Buffs) ---" << std::endl;
        if (jokerStock.empty()) std::cout << "   [SOLD OUT]" << std::endl;
        
        for (int i = 0; i < jokerStock.size(); i++) {
            std::cout << " " << (i + 1) << ". INSTALL " << jokerStock[i]->GetName() 
                      << " (Cost: " << jokerStock[i]->GetCost() << " Integrity)" << std::endl;
        }

        // --- DISPLAY HACKER PROTOCOLS ---
        std::cout << "\n--- HACKER PROTOCOLS (One-Time Use) ---" << std::endl;
        if (hackerStock.empty()) std::cout << "   [SOLD OUT]" << std::endl;
        
        for (int i = 0; i < hackerStock.size(); i++) {
            int displayNum = i + 1 + jokerStock.size(); 
            std::cout << " " << displayNum << ". EXECUTE " << hackerStock[i]->GetName() 
                      << " (Cost: " << hackerStock[i]->GetCost() << " Integrity)" << std::endl;
        }

        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "R. Refresh Stock (Cost: " << REROLL_COST << ")" << std::endl;
        std::cout << "E. Disconnect & Next Wave" << std::endl;
        std::cout << "Choice >> ";


        //Reminder: keslahan besar enih pake cin gua
        // char choice;
        // std::cin >> choice;
        
        //Getline for select shop item.
        std::string input;
        if (!std::getline(std::cin, input) || input.empty()) continue; 
        
        char choice = input[0]; // Ambil karakter pertama
        

        if (isdigit(choice)) {
            int selection = (choice - '0'); 
            int jokerCount = jokerStock.size();
            int hackerCount = hackerStock.size();

            // LOGIKA BELI JOKER
            if (selection >= 1 && selection <= jokerCount) {
                int idx = selection - 1;
                IModifier* item = jokerStock[idx];

                //Limit Deck maks 4
                if (inventory.size() >= 4) {
                    std::cout << "\n[ERROR] System Memory Full! You can only install up to 4 Active Scripts." << std::endl;
                }
                else if (money > item->GetCost()) { // Harus sisa minimal 1 nyawa
                    money -= item->GetCost();
                    inventory.push_back(item);
                    jokerStock.erase(jokerStock.begin() + idx); 
                    
                    std::cout << "\n[SUCCESS] Script Installed: " << item->GetName() << std::endl;
                } else {
                    std::cout << "\n[ERROR] Insufficient Integrity! Cannot risk system failure." << std::endl;
                }
            }
            // LOGIKA BELI HACKER CARD
            else if (selection > jokerCount && selection <= (jokerCount + hackerCount)) {
                int idx = selection - 1 - jokerCount;
                IHackerCard* item = hackerStock[idx];
                
                if (money > item->GetCost()) {
                    money -= item->GetCost();
                    
                    // Langsung eksekusi efeknya, (player bought)
                    item->Trigger(deck, scoringSystem, money);
                    
                    delete item; 
                    hackerStock.erase(hackerStock.begin() + idx);
                } else {
                    std::cout << "\n[ERROR] Insufficient Integrity!" << std::endl;
                }
            }
        }
        else if (choice == 'r' || choice == 'R') {
            if (money > REROLL_COST) {
                money -= REROLL_COST;
                GenerateStock(); 
            } else {
                std::cout << "\n[ERROR] Not enough Integrity to refresh connection!" << std::endl;
            }
        }
        else if (choice == 'e' || choice == 'E') {
            shopping = false;
            std::cout << "\n[LOG] Disconnecting from Dark Web..." << std::endl;
        }
    }
}