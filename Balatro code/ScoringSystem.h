#pragma once
#include <vector>
#include "modifiers/IModifier.h"

// Behavioral Pattern: Strategy
// ScoringSystem menggunakan chain of IModifier untuk menghitung skor akhir
// Urutan modifier mempengaruhi hasil (lihat analisis B2)
class ScoringSystem {
private:
    std::vector<IModifier*> modifiers; // chain modifier aktif

public:
    ~ScoringSystem();

    // Tambahkan modifier ke chain (Decorator-like)
    void addModifier(IModifier* modifier);

    // Hitung skor akhir dari skor dasar melalui seluruh chain modifier
    // Behavioral: Strategy - setiap modifier adalah "strategi" berbeda
    int calculateFinalScore(int baseScore) const;

    // Tampilkan semua modifier aktif
    void showModifiers() const;

    // Cek apakah ada modifier
    bool hasModifiers() const;
};