#pragma once
#include "ScoringSystem.h"
#include "ShopSystem.h"

// RunSession: mengatur seluruh loop permainan
// State pattern (konseptual): PLAY_HAND -> SCORE -> SHOP -> repeat
class RunSession {
private:
    ScoringSystem scoringSystem;
    ShopSystem    shopSystem;
    int           totalScore;
    int           currentRound;
    int           maxRounds;
    int           targetScore;

    // Simulasi bermain kartu - mengembalikan skor dasar ronde ini
    int playHand(int round); // round reserved untuk difficulty scaling

    // Tampilkan status ronde
    void showRoundStatus() const;

public:
    RunSession();

    // Jalankan seluruh sesi permainan (minimum 3 ronde)
    void startRun();
};
