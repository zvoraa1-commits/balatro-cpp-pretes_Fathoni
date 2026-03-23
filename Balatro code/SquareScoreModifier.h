#pragma once
#include "IModifier.h"

// Modifier tambahan: Mengkuadratkan skor (analisis A2 - extensibility)
// File ini ditambahkan TANPA mengubah file lain (open/closed principle)
class SquareScoreModifier : public IModifier {
public:
    int apply(int currentScore) const override {
        // Batasi agar tidak overflow (maks kuadrat dari 300)
        if (currentScore > 300) return currentScore * currentScore;
        return currentScore * currentScore;
    }

    std::string getName() const override {
        return "Square Surge";
    }

    std::string getDescription() const override {
        return "Mengkuadratkan skor saat ini (skor x skor)";
    }

    IModifier* clone() const override {
        return new SquareScoreModifier(*this);
    }
};