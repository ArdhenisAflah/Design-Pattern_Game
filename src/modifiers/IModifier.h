#ifndef IMODIFIER_H
#define IMODIFIER_H

#include <string>

class IModifier {
public:
    // Virtual destructor wajib ada di interface C++
    virtual ~IModifier() {}

    // Fungsi utama: Mengubah skor. 
    // Pakai "int& score" (Pass by Reference) agar nilai skor asli berubah.
    // Here we implement Decorator-style concept.
    virtual void Apply(int& score) = 0;

    // Info modifier
    virtual std::string GetName() = 0;
    virtual int GetCost() = 0;

    //  Modifier bisa nambah hand size!
    // Kita kasih default return 0, biar modifier lama gak error.
    virtual int GetHandSizeBonus() { return 0; }
};

#endif