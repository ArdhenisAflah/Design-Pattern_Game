#include "HackerFactory.h"
#include <cstdlib> // rand()

IHackerCard* HackerFactory::CreateRandomHackerCard() {
    int rng = rand() % 100;

    if (rng < 25) {
        return new GlassInjection(); // Fragmenter (Corrupted)
    } 
    else if (rng < 45) {
        return new Antivirus(); // Healing
    }
    else if (rng < 60) {
        return new ShieldProtocol(); // Firewall
    }
    else if (rng < 75) {
        return new SignalBooster(); // Optimized / Overclocked
    }
    else {
        int handRng = rand() % 9; 
        HandType type = (HandType)handRng;
        
        std::string name = "Unknown Protocol";
        // Leveling Up for Each Known Protocol Played Hand
        switch(type) {
            case HIGH_CARD:       name = "Protocol: High Card"; break;
            case PAIR:            name = "Protocol: Pair"; break;
            case TWO_PAIR:        name = "Protocol: Two Pair"; break;
            case THREE_OF_A_KIND: name = "Protocol: 3-Kind"; break;
            case STRAIGHT:        name = "Protocol: Straight"; break;
            case FLUSH:           name = "Protocol: Flush"; break;
            case FULL_HOUSE:      name = "Protocol: Full House"; break;
            case FOUR_OF_A_KIND:  name = "Protocol: 4-Kind"; break;
            case STRAIGHT_FLUSH:  name = "Protocol: Str. Flush"; break;
        }

        return new LevelPatch(type, name);
    }
}