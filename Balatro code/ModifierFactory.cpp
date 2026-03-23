#include "ModifierFactory.h"
#include "modifiers/ModifierA.h"
#include "modifiers/ModifierB.h"
#include "modifiers/SquareScoreModifier.h"
#include <iostream>

// Factory: satu-satunya tempat objek modifier dibuat
// Jika ingin menambah modifier baru, cukup tambah case di sini
IModifier* ModifierFactory::create(const std::string& type) {
    if (type == "double") {
        return new DoubleScoreModifier();
    }
    else if (type == "flat") {
        return new FlatBonusModifier(50);
    }
    else if (type == "square") {
        return new SquareScoreModifier();
    }
    // Modifier baru cukup ditambahkan di sini tanpa mengubah file lain
    return nullptr;
}

void ModifierFactory::listAvailable() {
    std::cout << "  [1] double  - Double Boost   : skor x2\n";
    std::cout << "  [2] flat    - Flat Boost      : skor +50\n";
    std::cout << "  [3] square  - Square Surge    : skor^2\n";
}