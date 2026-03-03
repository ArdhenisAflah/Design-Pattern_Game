#ifndef HACKERFACTORY_H
#define HACKERFACTORY_H

#include "consumables/IHackerCard.h"
#include "consumables/GlassInjection.h"
#include "consumables/LevelPatch.h"
#include "consumables/Antivirus.h" 
#include "consumables/ShieldProtocol.h"
#include "consumables/SignalBooster.h"

#include <string>

class HackerFactory {
public:
    // Fungsi statis untuk membuat kartu hacker acak
    static IHackerCard* CreateRandomHackerCard();
};

#endif