#pragma once
#include "IModifier.h"

// Modifier A: Menggandakan skor saat ini (skor * 2)
class DoubleScoreModifier : public IModifier {
public:
    int apply(int currentScore) const override {
        return currentScore * 2;
    }

    std::string getName() const override {
        return "Double Boost";
    }

    std::string getDescription() const override {
        return "Menggandakan total skor (x2)";
    }

    IModifier* clone() const override {
        return new DoubleScoreModifier(*this);
    }
};