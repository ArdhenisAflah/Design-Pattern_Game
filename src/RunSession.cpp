#include "RunSession.h"
#include <iostream>
#include <algorithm> 
#include <ctime>     
#include <sstream>   
#include <iomanip>   

// Constructor
RunSession::RunSession() {
    this->money = 4; // Modal awal $4
    this->currentAnte = 1;
    this->scoringSystem = new ScoringSystem();
    this->shopSystem = new ShopSystem();
    
    std::srand(std::time(0)); // Seed random
    InitializeDeck();
}

// Destructor
RunSession::~RunSession() {
    delete scoringSystem;
    delete shopSystem;
    for (IModifier* mod : playerModifiers) {
        delete mod;
    }
    playerModifiers.clear();
}

// Reset Deck jadi 52 kartu standar
void RunSession::InitializeDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            deck.push_back({(Suit)s, r});
        }
    }
}

// Acak Deck
void RunSession::ShuffleDeck() {
    std::random_shuffle(deck.begin(), deck.end());
}

// Main Loop Game (Antar Ante)
void RunSession::StartGame() {
    std::cout << "=== BALATRO C++ LITE EDITION ===" << std::endl;
    // Isi toko pertama kali
    shopSystem->GenerateStock();
    
    while (currentAnte <= MAX_ANTES) {
        PlayBlind(); 
        currentAnte++;
    }
    
    std::cout << "\n=== CONGRATULATIONS! YOU BEAT ANTE 8! ===" << std::endl;
}

// Logika Satu Ronde (Blind)
void RunSession::PlayBlind() {
    // 1. Setup Target Skor (Makin lama makin susah)
    int targetScore = BASE_TARGET * currentAnte * 1.5; 
    
    // 2. Reset Deck & Hand State
    InitializeDeck();
    ShuffleDeck();

    int hands = 4;      // Nyawa (Kesempatan main)
    int discards = 3;   // Discard (Belum ada logika buang kartu, tapi variabelnya ada)
    int currentScore = 0; // Skor saat ini (Mulai dari 0)

    // Cek Modifier untuk bonus kapasitas tangan
    int handSizeLimit = 8;
    for (IModifier* mod : playerModifiers) {
        handSizeLimit += mod->GetHandSizeBonus();
    }

    std::vector<Card> hand;

    // Helper Lambda untuk refill kartu sampai penuh
    auto DrawCards = [&]() {
        while (hand.size() < handSizeLimit && !deck.empty()) {
            hand.push_back(deck.back());
            deck.pop_back();
        }
    };

    // Draw kartu awal
    DrawCards(); 

    // === GAME LOOP DALAM SATU BLIND ===
    // Main terus selama nyawa masih ada DAN target belum tercapai
    while (hands > 0 && currentScore < targetScore) {
        
        // --- UI HEADER ---
        std::cout << "\n========================================" << std::endl;
        std::cout << "ANTE " << currentAnte << " | SCORE: " << currentScore << " / " << targetScore << std::endl;
        std::cout << "Hands: " << hands << " | Discards: " << discards << " | Money: $" << money << std::endl;
        std::cout << "Jokers: ";
        if(playerModifiers.empty()) std::cout << "(None)";
        for(auto* m : playerModifiers) std::cout << "[" << m->GetName() << "] ";
        std::cout << "\n----------------------------------------" << std::endl;

        // Tampilkan Tangan
        std::cout << "YOUR HAND:" << std::endl;
        for (size_t i = 0; i < hand.size(); i++) {
            std::cout << "[" << i << ":" << hand[i].ToString() << "] ";
        }
        std::cout << std::endl;

        // --- INPUT USER ---
        std::vector<Card> selectedCards;
        std::vector<int> selectedIndices;

        // Discard Flag
        bool isDiscardAction = false; 
        
        while (true) {
            std::cout << "\nAction: Type indices to Play (e.g. '0 1 2')." << std::endl;
            std::cout << ">> ";
            
            selectedCards.clear();
            selectedIndices.clear();
            
            std::string line;
            // PENTING: Pakai getline biar input ga loncat-loncat
            if (!std::getline(std::cin, line) || line.empty()) continue; 


            // [BARU] Logika deteksi Discard
            isDiscardAction = false; // Reset flag
            if (line[0] == 'd' || line[0] == 'D') {
                if (discards <= 0) {
                    std::cout << "[!] No discards remaining!" << std::endl;
                    continue;
                }
                isDiscardAction = true;
                line = line.substr(1); // Potong huruf 'd' agar sisa angkanya saja
            }

            std::stringstream ss(line);
            int idx;
            bool valid = true;
            
            // Parsing angka input
            while (ss >> idx) {
                if (idx >= 0 && idx < hand.size()) {
                    // Cek duplikat input (biar ga mainin kartu yg sama 2x)
                    bool duplicate = false;
                    for(int existing : selectedIndices) if(existing == idx) duplicate = true;
                    
                    if(!duplicate) {
                        selectedIndices.push_back(idx);
                        selectedCards.push_back(hand[idx]);
                    }
                } else {
                    valid = false;
                }
            }

            // Validasi final
            if (valid && !selectedCards.empty() && selectedCards.size() <= 5) {
                break; // Input valid, keluar loop input
            }
            std::cout << "[!] Invalid selection. Pick 1-5 unique valid card indices." << std::endl;
        }

        // --- PROSES SKORING ---

        if (isDiscardAction) {
            // === JALUR 1: DISCARD ===
            discards--; 
            std::cout << "\n>> Discarding selected cards..." << std::endl;
            
            // Hapus kartu (Remove)
            std::sort(selectedIndices.rbegin(), selectedIndices.rend());
            for (int idx : selectedIndices) {
                hand.erase(hand.begin() + idx);
            }
            
            // Refill (Reroll) kartu baru
            DrawCards();
            
            continue; // Skip ke loop berikutnya (jangan hitung skor!)
        } 
        // === JALUR 2: PLAY HAND (Code lama masuk ke sini) ===
        hands--; // Kurangi nyawa
        
        HandType type;
        // Hitung Base Score (Strategy Pattern)
        int handScore = scoringSystem->PlayHand(selectedCards, type);
        
        std::cout << "\n   > Hand: " << scoringSystem->GetHandName(type);
        std::cout << " | Base: " << handScore;

        // Hitung Modifier Effect (Decorator Pattern)
        for (IModifier* mod : playerModifiers) {
            int old = handScore;
            mod->Apply(handScore);
            if(handScore != old) std::cout << " -> [Joker: " << handScore << "]"; 
        }
        std::cout << std::endl;

        // AKUMULASI SKOR (PENTING!)
        currentScore += handScore;
        std::cout << "   > Total Blind Score: " << currentScore << " (Target: " << targetScore << ")" << std::endl;

        // --- CLEANUP TANGAN ---
        // Hapus kartu yang dimainkan. 
        // Sort index dari besar ke kecil supaya saat dihapus, index sisanya tidak geser.
        std::sort(selectedIndices.rbegin(), selectedIndices.rend());
        for (int idx : selectedIndices) {
            hand.erase(hand.begin() + idx);
        }

        // Refill tangan dengan kartu baru
        DrawCards();
    }

    // === AKHIR BLIND (CEK MENANG/KALAH) ===
    if (currentScore >= targetScore) {
        int reward = 5 * currentAnte; // Duit hadiah dasar
        reward += (hands * 1); // Bonus sisa hands ($1 per hand)
        
        money += reward;
        std::cout << "\n########################################" << std::endl;
        std::cout << "BLIND CLEARED! Reward: $" << reward << std::endl;
        std::cout << "########################################" << std::endl;
        
        // Buka Toko
        shopSystem->OpenShop(money, playerModifiers, deck, scoringSystem);
    } else {
        std::cout << "\n========================================" << std::endl;
        std::cout << "GAME OVER" << std::endl;
        std::cout << "Final Score: " << currentScore << " / " << targetScore << std::endl;
        std::cout << "Reached Ante: " << currentAnte << std::endl;
        std::cout << "========================================" << std::endl;
        exit(0); // Keluar program total
    }
}