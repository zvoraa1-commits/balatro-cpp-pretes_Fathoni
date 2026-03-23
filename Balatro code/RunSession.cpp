#include "RunSession.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

RunSession::RunSession()
    : shopSystem(10), totalScore(0), currentRound(1),
      maxRounds(3), targetScore(500) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

// Simulasi bermain tangan kartu
// Skor dasar dihitung dari kombinasi kartu sederhana
int RunSession::playHand(int /*round*/) {
    // Kartu disimulasikan sebagai nilai acak berbobot per ronde
    int baseCards[5];
    for (int i = 0; i < 5; i++) {
        baseCards[i] = (std::rand() % 10) + 1; // 1-10
    }

    std::cout << "\n  Kartu yang kamu mainkan: ";
    int handScore = 0;
    for (int i = 0; i < 5; i++) {
        std::cout << baseCards[i];
        if (i < 4) std::cout << ", ";
        handScore += baseCards[i];
    }
    std::cout << "\n";

    // Bonus jika ada pasangan (dua kartu sama)
    for (int i = 0; i < 4; i++) {
        for (int j = i+1; j < 5; j++) {
            if (baseCards[i] == baseCards[j]) {
                handScore += 20;
                std::cout << "  Bonus pasangan +20!\n";
            }
        }
    }

    return handScore;
}

void RunSession::showRoundStatus() const {
    std::cout << "\n--- Ronde " << currentRound << " dari " << maxRounds << " ---\n";
    std::cout << "Total skor: " << totalScore
              << " | Target: " << targetScore << "\n";
    std::cout << "Modifier aktif:\n";
    scoringSystem.showModifiers();
}

void RunSession::startRun() {
    std::cout << "============================================\n";
    std::cout << "     CARD RUN GAME - Design Pattern Demo    \n";
    std::cout << "============================================\n";
    std::cout << "Target skor: " << targetScore << " poin\n";
    std::cout << "Kamu punya " << maxRounds << " ronde untuk mencapainya!\n";

    for (currentRound = 1; currentRound <= maxRounds; currentRound++) {
        showRoundStatus();

        // === FASE: PLAY HAND ===
        std::cout << "\n[FASE: BERMAIN KARTU]\n";
        int baseScore = playHand(currentRound);
        std::cout << "  Skor dasar tangan: " << baseScore << "\n";

        // === FASE: CALCULATE SCORE (Strategy pattern) ===
        std::cout << "\n[FASE: HITUNG SKOR]\n";
        int finalScore = scoringSystem.calculateFinalScore(baseScore);
        std::cout << "  => Skor akhir ronde ini: " << finalScore << "\n";
        totalScore += finalScore;

        // Pemain mendapat koin dari skor ronde
        int coinsEarned = finalScore / 20;
        shopSystem.addCoins(coinsEarned);
        std::cout << "  Koin diperoleh: +" << coinsEarned << "\n";

        // === FASE: SHOP (opsional) ===
        if (currentRound < maxRounds) {
            std::cout << "\n[FASE: TOKO]\n";
            shopSystem.openShop(scoringSystem);
        }
    }

    // === END RUN ===
    std::cout << "\n============================================\n";
    std::cout << "            AKHIR PERMAINAN\n";
    std::cout << "============================================\n";
    std::cout << "Total Skor Akhir  : " << totalScore << "\n";
    std::cout << "Target            : " << targetScore << "\n";

    if (totalScore >= targetScore) {
        std::cout << "HASIL             : *** MENANG! Selamat! ***\n";
    } else {
        std::cout << "HASIL             : Belum berhasil. Coba lagi!\n";
        std::cout << "Kurang            : " << (targetScore - totalScore) << " poin\n";
    }
    std::cout << "============================================\n";
}
