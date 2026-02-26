#ifndef MODIFIER_FACTORY_H
#define MODIFIER_FACTORY_H

#include "modifiers/IModifier.h"
#include <string>

class ModifierFactory {
public:
    static IModifier* CreateModifier(std::string type);
};

#endif