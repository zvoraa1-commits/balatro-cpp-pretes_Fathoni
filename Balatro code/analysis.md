# Design Pattern Analysis
**Project:** Card-Run Game Prototype  
**Language:** C++17  
**Author:** [Nama Kamu]

---

## PART A — Creational Pattern Analysis

### A1 — Factory

**Di mana objek modifier dibuat?**

Semua objek `IModifier` dibuat di dalam fungsi `ModifierFactory::create()` yang ada di file `src/ModifierFactory.cpp`.

```cpp
// src/ModifierFactory.cpp — fungsi create()
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
    return nullptr;
}
```

**Kelas yang membuat objek:** `ModifierFactory` (di `src/ModifierFactory.cpp`)

**Mengapa memusatkan pembuatan itu menguntungkan:**

Tanpa Factory, setiap kelas yang butuh modifier (seperti `ShopSystem`) harus menulis `new DoubleScoreModifier()` sendiri. Jika nama kelas berubah atau ada parameter baru, kita harus mengubah banyak file. Dengan Factory, perubahan hanya terjadi di satu tempat yaitu `ModifierFactory.cpp`. `ShopSystem` hanya memanggil `ModifierFactory::create("double")` tanpa perlu tahu kelas konkret apa yang dibuat.

**Referensi:** `src/ShopSystem.cpp` — fungsi `openShop()` memanggil `ModifierFactory::create(choice)`.

---

### A2 — Extensibility

**Bagaimana `SquareScoreModifier` ditambahkan?**

Langkah yang dilakukan:

1. **Buat file baru:** `src/modifiers/SquareScoreModifier.h` — berisi kelas `SquareScoreModifier` yang mengimplementasikan `IModifier`.
2. **Tambah satu case di Factory:** `src/ModifierFactory.cpp` — tambahkan `else if (type == "square")`.
3. **Tambah satu baris di listAvailable:** `src/ModifierFactory.cpp` — tampilkan opsi baru di toko.

**File yang berubah:**
- `src/modifiers/SquareScoreModifier.h` (file baru, tidak mengubah yang lain)
- `src/ModifierFactory.cpp` (tambah 2 baris)

**File yang TIDAK perlu berubah:**
- `IModifier.h` — interface tidak berubah
- `ScoringSystem.h/.cpp` — tidak perlu tahu kelas baru
- `ShopSystem.h/.cpp` — tidak perlu tahu kelas baru
- `RunSession.h/.cpp` — tidak perlu tahu kelas baru

Desain ini sangat mudah di-extend. Menambah modifier baru hanya membutuhkan 1 file baru dan 2-3 baris perubahan di Factory. Ini adalah contoh nyata **Open/Closed Principle** — sistem terbuka untuk ekstensi, tertutup untuk modifikasi.

---

### A3 — Factory Method (Konseptual)

**Jika ada beberapa tipe toko, bagaimana Factory Method membantu?**

Misalnya ada `NormalShop`, `PremiumShop`, dan `BlackMarketShop`, masing-masing membuat set modifier yang berbeda. Dengan Factory Method:

```cpp
// Interface abstrak untuk toko
class IShopFactory {
public:
    virtual IModifier* createModifier(const std::string& type) = 0;
    virtual ~IShopFactory() = default;
};

// Toko normal hanya buat modifier dasar
class NormalShopFactory : public IShopFactory {
public:
    IModifier* createModifier(const std::string& type) override {
        if (type == "double") return new DoubleScoreModifier();
        if (type == "flat")   return new FlatBonusModifier(50);
        return nullptr;
    }
};

// Toko premium buat modifier dengan nilai lebih tinggi
class PremiumShopFactory : public IShopFactory {
public:
    IModifier* createModifier(const std::string& type) override {
        if (type == "flat")   return new FlatBonusModifier(200); // bonus lebih besar
        if (type == "square") return new SquareScoreModifier();
        return nullptr;
    }
};
```

`ShopSystem` hanya menyimpan pointer `IShopFactory*` dan memanggil `factory->createModifier(type)`, tanpa perlu tahu toko mana yang aktif. Ini membuat `ShopSystem` bisa dipakai ulang dengan tipe toko apapun.

---

### A4 — Prototype (Konseptual)

**Bagaimana `Clone()` bisa membantu menduplikasi modifier?**

Setiap kelas turunan `IModifier` sudah memiliki method `clone()` yang dideklarasikan di `src/modifiers/IModifier.h`:

```cpp
// src/modifiers/IModifier.h
virtual IModifier* clone() const = 0;
```

Dan diimplementasikan di setiap kelas konkret, contoh di `src/modifiers/ModifierA.h`:

```cpp
IModifier* clone() const override {
    return new DoubleScoreModifier(*this); // salin dengan copy constructor
}
```

**Di mana Clone() berguna:**  
Jika pemain mendapat bonus "duplikasi modifier" — alih-alih membuat modifier baru lewat Factory (yang butuh nama tipe), kita bisa langsung clone modifier yang sudah ada. Ini berguna juga untuk menyimpan "template" modifier di sebuah registry, lalu setiap kali dibutuhkan, registry memanggil `clone()` untuk memberi salinan baru tanpa merusak template aslinya.

---

### A5 — Singleton (Konseptual)

**Apakah ada objek global dalam kode ini?**

Tidak. `ScoringSystem`, `ShopSystem`, dan `ModifierFactory` semuanya dibuat sebagai member atau objek lokal di dalam `RunSession`.

**Kapan Singleton bisa digunakan:**  
`ModifierFactory` bisa dijadikan Singleton jika sistem menjadi sangat besar dan banyak kelas di berbagai tempat butuh mengaksesnya. Dengan Singleton, hanya ada satu instance Factory di seluruh program.

```cpp
class ModifierFactory {
private:
    ModifierFactory() = default; // konstruktor privat
public:
    static ModifierFactory& getInstance() {
        static ModifierFactory instance; // dibuat sekali, thread-safe di C++11
        return instance;
    }
    IModifier* create(const std::string& type);
};
```

**Keuntungan:** Akses global yang mudah, objek hanya dibuat sekali.  
**Risiko:** Membuat kode sulit diuji (unit test sulit mengganti instance), menciptakan ketergantungan tersembunyi antar kelas, dan bisa menjadi bottleneck jika diakses dari banyak thread secara bersamaan.

Dalam proyek ini, Factory tidak perlu Singleton karena hanya `ShopSystem` yang memanggilnya, dan semua method-nya adalah `static`.

---

## PART B — Structural Pattern Analysis

### B1 — Decorator Concept

**Bagaimana sistem modifier berperilaku seperti Decorator?**

Dalam pola Decorator klasik, sebuah objek dibungkus oleh objek lain yang menambah perilaku tanpa mengubah interface-nya. Dalam sistem ini, `ScoringSystem` menerapkan chain of modifiers secara berurutan di fungsi `calculateFinalScore()` di `src/ScoringSystem.cpp`:

```cpp
// src/ScoringSystem.cpp — calculateFinalScore()
int score = baseScore;
for (const auto* mod : modifiers) {
    score = mod->apply(score); // setiap modifier "membungkus" hasil sebelumnya
}
```

**Yang didekorasi:** Nilai integer `score` yang dihasilkan dari tangan kartu.

Setiap `IModifier::apply(score)` menerima skor saat ini dan mengembalikan skor yang sudah dimodifikasi. Modifier berikutnya menerima output modifier sebelumnya — persis seperti Decorator membungkus Decorator lain.

Perbedaan dengan Decorator murni: dalam Decorator klasik, setiap wrapper menyimpan referensi ke objek sebelumnya. Di sini, chain dikelola oleh `ScoringSystem` menggunakan `std::vector`. Hasilnya fungsional sama namun lebih mudah dikelola karena kita bisa menambah/melihat/menghapus modifier dari vector dengan mudah.

---

### B2 — Modifier Order

**Apakah urutan modifier mempengaruhi skor?**

**Ya, sangat berpengaruh.** Ini bisa dilihat di output program pada ronde 3:

```
Skor dasar: 68
Double Boost diterapkan: 68 -> 136   (68 * 2)
Flat Boost diterapkan: 136 -> 186    (136 + 50)
Skor akhir: 186
```

Jika urutan dibalik (Flat dulu, baru Double):
```
Skor dasar: 68
Flat Boost: 68 -> 118    (68 + 50)
Double Boost: 118 -> 236  (118 * 2)
Skor akhir: 236
```

Selisihnya adalah 50 poin (nilai flat bonus × 2 karena ikut digandakan). Ini penting dalam desain game: pemain yang membeli modifier lebih awal mendapat urutan yang berbeda, yang bisa menguntungkan atau merugikan tergantung kombinasi. Logika urutan ada di `src/ScoringSystem.cpp` — modifier diterapkan sesuai urutan push ke dalam `std::vector<IModifier*> modifiers`.

---

### B3 — Composite (Konseptual)

**Bagaimana grup modifier bisa diimplementasikan?**

Dengan Composite pattern, kita bisa membuat `ModifierGroup` yang juga mengimplementasikan `IModifier`. Grup ini berisi beberapa modifier dan menerapkan semuanya sekaligus:

```cpp
class ModifierGroup : public IModifier {
private:
    std::vector<IModifier*> children;
    std::string groupName;
public:
    void add(IModifier* mod) { children.push_back(mod); }

    int apply(int score) const override {
        for (auto* mod : children) score = mod->apply(score);
        return score;
    }

    std::string getName() const override { return groupName; }
    std::string getDescription() const override { return "Grup modifier"; }
    IModifier* clone() const override { return new ModifierGroup(*this); }
};
```

`ScoringSystem` tidak perlu berubah sama sekali — ia tetap memanggil `mod->apply(score)` seperti biasa, karena `ModifierGroup` juga adalah `IModifier`. Ini kekuatan Composite: leaf dan composite diperlakukan seragam.

---

### B4 — Adapter (Konseptual)

**Bagaimana mengintegrasikan sistem scoring eksternal menggunakan Adapter?**

Bayangkan ada library eksternal dengan interface yang berbeda:

```cpp
// Library eksternal yang tidak bisa kita ubah
class ExternalScoringLib {
public:
    double computeScore(double base, std::vector<double> multipliers);
};
```

Kita buat Adapter yang membungkusnya agar bisa dipakai sebagai `IModifier`:

```cpp
class ExternalScoringAdapter : public IModifier {
private:
    ExternalScoringLib externalLib;
    double multiplier;
public:
    ExternalScoringAdapter(double mult) : multiplier(mult) {}

    int apply(int currentScore) const override {
        // Adapter mengkonversi tipe dan memanggil library eksternal
        double result = externalLib.computeScore(
            static_cast<double>(currentScore),
            {multiplier}
        );
        return static_cast<int>(result);
    }

    std::string getName() const override { return "External Scorer"; }
    std::string getDescription() const override { return "Menggunakan scoring eksternal"; }
    IModifier* clone() const override { return new ExternalScoringAdapter(*this); }
};
```

`ScoringSystem` tetap tidak berubah. Adapter adalah jembatan antara interface kita (`IModifier`) dan interface library luar.

---

### B5 — Facade (Konseptual)

**Desain kelas `GameFacade`:**

`GameFacade` menyembunyikan kompleksitas interaksi antar subsistem. Client code (misalnya `main.cpp`) cukup memanggil fungsi facade tanpa perlu tahu ada `RunSession`, `ScoringSystem`, dan `ShopSystem` di baliknya.

```cpp
class GameFacade {
private:
    RunSession    session;
    ScoringSystem scoring;
    ShopSystem    shop;
public:
    // Fungsi sederhana yang disembunyikan di balik facade
    void startNewGame();          // inisialisasi semua subsistem
    void playRound();             // jalankan satu ronde penuh
    void openShop();              // buka toko modifier
    int  getTotalScore() const;   // ambil skor total
    bool isGameOver() const;      // cek apakah game sudah selesai
    void showStatus() const;      // tampilkan status game saat ini
};
```

Fungsi yang diekspos facade sengaja dibuat level tinggi dan mudah dipahami, menyembunyikan detail seperti urutan pemanggilan `calculateFinalScore()`, manajemen koin, dan state ronde.

---

## PART C — Behavioral Pattern Analysis

### C1 — Strategy

**Di mana logika scoring berada?**

Logika scoring utama ada di `src/ScoringSystem.cpp` dalam fungsi `calculateFinalScore(int baseScore)`. Setiap `IModifier` adalah satu "strategi" perhitungan yang bisa dipertukarkan.

**Bagaimana mendukung beberapa aturan scoring:**

Kita bisa buat interface `IScoringStrategy` yang mendefinisikan cara menghitung skor dasar dari tangan kartu:

```cpp
class IScoringStrategy {
public:
    virtual int calculateBase(const std::vector<int>& cards) const = 0;
    virtual ~IScoringStrategy() = default;
};

class SumStrategy : public IScoringStrategy {
public:
    int calculateBase(const std::vector<int>& cards) const override {
        int sum = 0;
        for (int c : cards) sum += c;
        return sum; // jumlahkan semua kartu
    }
};

class HighCardStrategy : public IScoringStrategy {
public:
    int calculateBase(const std::vector<int>& cards) const override {
        return *std::max_element(cards.begin(), cards.end()) * 10; // hanya kartu tertinggi
    }
};
```

`ScoringSystem` menyimpan pointer `IScoringStrategy*` dan bisa berganti strategi di runtime. Ini memungkinkan mode permainan berbeda (normal, hardcore, puzzle) tanpa mengubah struktur kelas utama.

---

### C2 — Observer (Konseptual)

**Event apa yang ada dalam sistem ini?**

- Pemain bermain kartu (skor dasar dihitung)
- Modifier diterapkan ke skor
- Pemain membeli modifier baru di toko
- Ronde berakhir
- Game selesai (menang/kalah)

**Bagaimana Observer meningkatkan desain:**

```cpp
class IGameObserver {
public:
    virtual void onModifierApplied(const std::string& name, int before, int after) = 0;
    virtual void onRoundEnd(int round, int roundScore) = 0;
    virtual void onModifierPurchased(const std::string& name) = 0;
    virtual ~IGameObserver() = default;
};
```

`ScoringSystem` dan `RunSession` menjadi "subject" yang menyimpan list observer dan memanggil `notify()` saat event terjadi. Contoh penggunaan: `StatisticsObserver` mencatat semua event untuk analisis, `UIObserver` menampilkan animasi saat modifier diterapkan. Keduanya bisa ditambah tanpa mengubah `ScoringSystem` sama sekali.

---

### C3 — Command (Konseptual)

**Aksi apa yang bisa direpresentasikan sebagai Command?**

- `PlayHandCommand` — aksi bermain tangan kartu
- `BuyModifierCommand` — aksi membeli modifier (menyimpan tipe dan harga)
- `ApplyModifierCommand` — aksi menerapkan modifier ke skor
- `SkipShopCommand` — aksi melewati toko

```cpp
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0; // untuk fitur undo
    virtual ~ICommand() = default;
};

class BuyModifierCommand : public ICommand {
private:
    ShopSystem&    shop;
    ScoringSystem& scoring;
    std::string    modifierType;
    IModifier*     purchased = nullptr;
public:
    BuyModifierCommand(ShopSystem& s, ScoringSystem& sc, std::string type)
        : shop(s), scoring(sc), modifierType(std::move(type)) {}

    void execute() override {
        purchased = ModifierFactory::create(modifierType);
        if (purchased) scoring.addModifier(purchased);
    }

    void undo() override {
        // kembalikan koin, hapus modifier dari chain
    }
};
```

Dengan Command, semua aksi pemain bisa disimpan dalam queue, di-replay untuk fitur replay, atau di-undo untuk fitur pembatalan pembelian.

---

### C4 — State (Konseptual)

**Bagaimana State pattern mengorganisir fase permainan?**

Saat ini fase permainan dikelola dengan loop `for` dan kondisi `if` di `RunSession::startRun()`. Dengan State pattern, setiap fase menjadi objek tersendiri:

```cpp
class IGameState {
public:
    virtual void enter(RunSession& session) = 0;  // dipanggil saat masuk state
    virtual void update(RunSession& session) = 0; // logika utama state
    virtual void exit(RunSession& session) = 0;   // dipanggil saat keluar state
    virtual ~IGameState() = default;
};

class PlayHandState : public IGameState {
public:
    void enter(RunSession& session) override { /* tampilkan pesan */ }
    void update(RunSession& session) override { /* jalankan playHand() */ }
    void exit(RunSession& session) override { /* simpan skor */ }
};

class ShopState : public IGameState { /* ... */ };
class ScoreState : public IGameState { /* ... */ };
class GameOverState : public IGameState { /* ... */ };
```

`RunSession` menyimpan `IGameState* currentState` dan memanggil `currentState->update()`. State sendiri yang memutuskan state berikutnya dengan memanggil `session.changeState(new ShopState())`. Ini membuat transisi antar fase eksplisit, mudah dibaca, dan mudah ditambah fase baru (misalnya `BossRoundState`) tanpa mengubah RunSession.
