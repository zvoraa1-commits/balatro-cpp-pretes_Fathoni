#pragma once
#include <string>
#include <memory>
#include "modifiers/IModifier.h"

// Creational Pattern: Factory
// Memusatkan pembuatan semua objek IModifier
// ShopSystem dan RunSession tidak perlu tahu kelas konkret mana yang dibuat
class ModifierFactory {
public:
    // Buat modifier berdasarkan nama tipe
    // Mengembalikan nullptr jika tipe tidak dikenal
    static IModifier* create(const std::string& type);

    // Daftar semua tipe modifier yang tersedia
    static void listAvailable();
};