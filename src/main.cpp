#include "RunSession.h"
#include <iostream>

int main() {
    // 1. Buat Instance Game Manager
    RunSession gameSession;

    // 2. Mulai Game Loop
    try {
        gameSession.StartGame();
    } 
    catch (const std::exception& e) {
        // Jaga-jaga kalau ada error tak terduga
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}