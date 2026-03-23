#pragma once
#include <string>

// Interface untuk semua modifier (Structural: Decorator concept)
// Setiap modifier membungkus skor dan mengubahnya
class IModifier {
public:
    virtual ~IModifier() = default;

    // Terapkan efek modifier ke skor saat ini
    virtual int apply(int currentScore) const = 0;

    // Nama modifier untuk ditampilkan ke pemain
    virtual std::string getName() const = 0;

    // Deskripsi efek modifier
    virtual std::string getDescription() const = 0;

    // Prototype pattern (konseptual): salin modifier ini
    virtual IModifier* clone() const = 0;
};