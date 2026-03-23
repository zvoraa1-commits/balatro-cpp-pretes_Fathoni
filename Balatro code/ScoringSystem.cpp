#include "ScoringSystem.h"
#include <iostream>

ScoringSystem::~ScoringSystem() {
    for (auto* mod : modifiers) {
        delete mod;
    }
}

void ScoringSystem::addModifier(IModifier* modifier) {
    modifiers.push_back(modifier);
}

// Inti dari sistem: terapkan setiap modifier secara berurutan
// Ini seperti Decorator chain - setiap modifier "membungkus" hasil sebelumnya
int ScoringSystem::calculateFinalScore(int baseScore) const {
    int score = baseScore;
    std::cout << "\n  [Scoring] Skor dasar: " << score << "\n";

    for (const auto* mod : modifiers) {
        int before = score;
        score = mod->apply(score);
        std::cout << "  [Scoring] " << mod->getName()
                  << " diterapkan: " << before << " -> " << score << "\n";
    }

    return score;
}

void ScoringSystem::showModifiers() const {
    if (modifiers.empty()) {
        std::cout << "  (tidak ada modifier aktif)\n";
        return;
    }
    for (size_t i = 0; i < modifiers.size(); i++) {
        std::cout << "  [" << (i+1) << "] " << modifiers[i]->getName()
                  << " - " << modifiers[i]->getDescription() << "\n";
    }
}

bool ScoringSystem::hasModifiers() const {
    return !modifiers.empty();
}