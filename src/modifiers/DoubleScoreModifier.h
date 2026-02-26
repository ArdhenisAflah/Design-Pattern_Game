#ifndef DOUBLESCOREMODIFIER_H  
#define DOUBLESCOREMODIFIER_H

#include "IModifier.h"

// Ini Joker Modifier Untuk Double Score.
class DoubleScoreModifier : public IModifier {
public:
    // Override fungsi dari Interface
    void Apply(int& score) override;
    std::string GetName() override;
    int GetCost() override;
};

#endif