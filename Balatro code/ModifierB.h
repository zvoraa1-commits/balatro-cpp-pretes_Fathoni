#pragma once
#include "IModifier.h"

// Modifier B: Menambahkan nilai tetap ke skor
class FlatBonusModifier : public IModifier {
private:
    int bonusAmount;

public:
    explicit FlatBonusModifier(int amount = 50) : bonusAmount(amount) {}

    int apply(int currentScore) const override {
        return currentScore + bonusAmount;
    }

    std::string getName() const override {
        return "Flat Boost";
    }

    std::string getDescription() const override {
        return "Menambah " + std::to_string(bonusAmount) + " poin ke skor";
    }

    IModifier* clone() const override {
        return new FlatBonusModifier(*this);
    }
};