#include "RunSession.h"
#include <iostream>
#include <algorithm> 
#include <ctime>     
#include <sstream>   
#include <iomanip>   

RunSession::RunSession() {
    // Modal Awal Besar Cuz its make easier for player (not too hard or easy i think)
    this->money = 2000; 
    
    this->currentAnte = 1;
    this->scoringSystem = new ScoringSystem();
    this->shopSystem = new ShopSystem();
    
    std::srand(std::time(0)); 
    InitializeDeck();
}

RunSession::~RunSession() {
    delete scoringSystem; delete shopSystem;
    for (IModifier* mod : playerModifiers) delete mod;
    playerModifiers.clear();
}

void RunSession::InitializeDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            deck.push_back({(Suit)s, r});
        }
    }
}

void RunSession::ShuffleDeck() {
    std::random_shuffle(deck.begin(), deck.end());
}

void RunSession::StartGame() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CYBER SURVIVAL: DATA DEFENSE v1.0    " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "MISSION: Protect your Integrity ($) from Virus Threats." << std::endl;
    std::cout << "WARNING: If Integrity reaches 0, System crashes.\n" << std::endl;
    
    // Buka toko awal untuk persiapan
    shopSystem->GenerateStock();
    
    while (currentAnte <= MAX_WAVES && money > 0) {
        PlayBlind(); 
        currentAnte++;
    }
    
    if (money > 0)
        std::cout << "\n=== [SYSTEM SECURED] ALL THREATS ELIMINATED! ===" << std::endl;
    else 
        std::cout << "\n=== [CRITICAL FAILURE] SYSTEM CRASHED. GAME OVER. ===" << std::endl;
}

void RunSession::PlayBlind() {
    // 1. PROGRESSING SYSTEM: HP Virus makin tebal tiap Ante
    long long virusHP = BASE_THREAT * currentAnte * 1.5; 
    
    //tidak membolehkan script fragmenter exe jalan soalnya reset 
    std::vector<Card> drawPile = deck;
    std::random_shuffle(drawPile.begin(), drawPile.end());

    ShuffleDeck();

    int hands = 4;      // Batas serangan player (Turns)
    int discards = 3;   // Batas ganti kartu
    
    // Hitung kapasitas tangan + bonus joker
    int handSizeLimit = 8;
    for (IModifier* mod : playerModifiers) handSizeLimit += mod->GetHandSizeBonus();

    std::vector<Card> hand;
    auto DrawCards = [&]() {
        while (hand.size() < handSizeLimit && !drawPile.empty()) {
            hand.push_back(drawPile.back()); drawPile.pop_back();
        }
    };
    DrawCards(); 

    // === BATTLE LOOP ===
    while (hands > 0 && virusHP > 0) {
        
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "WAVE " << currentAnte << " | VIRUS THREAT: " << virusHP << std::endl;
        std::cout << "INTEGRITY ($): " << money << " | Attacks: " << hands << " | Rerolls: " << discards << std::endl;
        
        std::cout << "Active Scripts: ";
        if(playerModifiers.empty()) std::cout << "(None)";
        for(auto* m : playerModifiers) std::cout << "[" << m->GetName() << "] ";
        std::cout << "\n----------------------------------------" << std::endl;

        std::cout << "DATA BUFFER (HAND):" << std::endl;
        for (size_t i = 0; i < hand.size(); i++) {
            std::cout << "[" << i << ":" << hand[i].ToString() << "] ";
        }
        std::cout << std::endl;

        // Input Logic
        std::vector<Card> selectedCards;
        std::vector<int> selectedIndices;
        bool isDiscard = false;
        
        while (true) {
            std::cout << "\nCmd (idx OR 'd' idx): ";
            std::string line;
            if (!std::getline(std::cin, line) || line.empty()) continue;

            isDiscard = (line[0] == 'd' || line[0] == 'D');
            if (isDiscard) line = line.substr(1);

            std::stringstream ss(line);
            int idx;
            bool valid = true;
            selectedIndices.clear(); selectedCards.clear();
            
            while (ss >> idx) {
                if (idx >= 0 && idx < hand.size()) {
                    bool dup = false;
                    for(int x : selectedIndices) if(x==idx) dup=true;
                    
                    if(dup) { 
                        // JIKA DUP: Tandai sebagai input tidak valid
                        valid = false; 
                    } else { 
                        selectedIndices.push_back(idx); 
                        selectedCards.push_back(hand[idx]); 
                    }
                } else {
                    valid = false;
                }
            }

            if (isDiscard && discards <= 0) { std::cout << "No rerolls left!" << std::endl; continue; }
            if (valid && !selectedCards.empty() && selectedCards.size() <= 5) break;
            std::cout << "Invalid input." << std::endl;
        }

        if (isDiscard) {
            discards--;
            std::cout << ">> Flushing bad data (Discarding)..." << std::endl;
        } else {
            // PLAY HAND FOR ATTACK VIRUS
            hands--; 
            HandType type;
            std::vector<Card> brokenCards; // Penampung kartu yang meledak (discard after dis )

            int damage = scoringSystem->PlayHand(selectedCards, type, brokenCards);
            // 1. Hapus kartu yang pecah dari DECK PERMANEN!!!!!!!!!!!!
            if (!brokenCards.empty()) {
                for (const auto& brokenCard : brokenCards) {
                    // Cari kartu yang sama di dalam deck dan hapus
                    for (auto it = deck.begin(); it != deck.end(); ++it) {
                        if (it->rank == brokenCard.rank && it->suit == brokenCard.suit) {
                            deck.erase(it);
                            break; 
                        }
                    }
                }
            }
            std::cout << "\n   > Executing: " << scoringSystem->GetHandName(type) << " | Base Dmg: " << damage;

            // Apply Modifiers (Buff Attack)
            for (IModifier* mod : playerModifiers) {
                int old = damage;
                mod->Apply(damage);
                if(damage != old) std::cout << " -> [Buff: " << damage << "]"; 
            }
            std::cout << std::endl;

            // Kurangi HP Virus
            virusHP -= damage;
            if (virusHP < 0) virusHP = 0;
            
            std::cout << "   > Virus Threat Reduced! Remaining: " << virusHP << std::endl;
        }

        // Cleanup & Redraw
        std::sort(selectedIndices.rbegin(), selectedIndices.rend());
        for (int idx : selectedIndices) hand.erase(hand.begin() + idx);
        DrawCards();
    }

    // === LOGIC TIAP AKHIR WAVE ===
    if (virusHP <= 0) {
        // MENANG: Dapat bonus Integrity (Healing)
        int reward = 100 + (hands * 50); 
        money += reward;
        std::cout << "\n[SUCCESS] VIRUS DELETED! Integrity Restored +$" << reward << std::endl;
        
        // Buka Toko untuk persiapan Wave berikutnya
        shopSystem->OpenShop(money, playerModifiers, deck, scoringSystem);
    } else {
        // KALAH WAVE: Virus menyerang balik! (Kurangi Uang if any)
        std::cout << "\n[WARNING] DEFENSE FAILED! Virus attacks System!" << std::endl;
        std::cout << "Integrity Damage Taken: -" << virusHP << std::endl;
        
        money -= virusHP;
        
        if (money <= 0) {
            std::cout << "Integrity Critical... SYSTEM SHUTDOWN." << std::endl;
            exit(0);
        } else {
            std::cout << "System unstable. Remaining Integrity: $" << money << std::endl;
            std::cout << "Emergency Shop Protocol Initiated..." << std::endl;
            // Masih hidup tapi sekarat, buka toko siapa tau bisa beli bantuan wok :) 
            shopSystem->OpenShop(money, playerModifiers, deck, scoringSystem);
        }
    }
}