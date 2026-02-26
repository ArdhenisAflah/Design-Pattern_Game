#ifndef FLATBONUSMODIFIER_H
#define FLATBONUSMODIFIER_H

#include "IModifier.h"

class FlatBonusModifier : public IModifier {
private:
    int bonusAmount;

public:
    FlatBonusModifier(int amount); // Constructor
    void Apply(int& score) override;
    std::string GetName() override;
    int GetCost() override;
};

#endif