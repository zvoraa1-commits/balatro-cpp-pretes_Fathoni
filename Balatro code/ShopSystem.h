#pragma once
#include "ScoringSystem.h"
#include "ModifierFactory.h"

// ShopSystem: tempat pemain membeli modifier baru
// Menggunakan ModifierFactory untuk membuat modifier (tidak tahu kelas konkretnya)
class ShopSystem {
private:
    int playerCoins;

public:
    explicit ShopSystem(int startingCoins = 10);

    // Tampilkan toko dan minta pilihan pemain
    // Mengembalikan true jika pemain membeli sesuatu
    bool openShop(ScoringSystem& scoring);

    int getCoins() const;
    void addCoins(int amount);
};