# Card-Run Game — Design Pattern Pretest

Proyek ini adalah implementasi prototipe game berbasis kartu berbasis teks menggunakan C++17,  
dibuat untuk memenuhi tugas pretest mata kuliah **Design Pattern untuk Game**.

---

## Cara Kompilasi dan Menjalankan

```bash
# Kompilasi
g++ -std=c++17 -o card_run \
  src/main.cpp \
  src/RunSession.cpp \
  src/ScoringSystem.cpp \
  src/ShopSystem.cpp \
  src/ModifierFactory.cpp \
  -I src

# Jalankan
./card_run
```

**Persyaratan:** Compiler C++17 (g++ 7+ atau clang++ 5+). Tidak ada library eksternal.

---

## Struktur Direktori

```
balatro-cpp-pretest/
├── src/
│   ├── main.cpp                      # Entry point
│   ├── RunSession.h / .cpp           # Mengatur loop permainan
│   ├── ScoringSystem.h / .cpp        # Hitung skor + terapkan modifier chain
│   ├── ShopSystem.h / .cpp           # Toko pembelian modifier
│   ├── ModifierFactory.h / .cpp      # Factory untuk membuat modifier
│   └── modifiers/
│       ├── IModifier.h               # Interface abstrak modifier
│       ├── ModifierA.h               # DoubleScoreModifier (skor x2)
│       ├── ModifierB.h               # FlatBonusModifier (skor +50)
│       └── SquareScoreModifier.h     # SquareScoreModifier (skor^2)
├── analysis/
│   └── analysis.md                   # Analisis design pattern lengkap
├── evidence/
│   └── output.txt                    # Output program aktual
└── README.md
```

---

## Loop Permainan

```
Start Run
  └─> Ronde 1
        ├─> [PLAY HAND]   : 5 kartu dimainkan, skor dasar dihitung
        ├─> [SCORE]       : Modifier chain diterapkan ke skor dasar
        └─> [SHOP]        : Pemain bisa beli modifier (opsional, 5 koin)
  └─> Ronde 2 (sama)
  └─> Ronde 3 (tanpa toko)
End Run — tampilkan total skor vs target (500)
```

---

## Architecture Overview

### `RunSession` (`src/RunSession.h/.cpp`)
Kelas utama yang mengatur seluruh siklus permainan. Menyimpan instance `ScoringSystem` dan `ShopSystem`. Menjalankan loop 3 ronde: play hand → score → shop. Bertanggung jawab atas state permainan (ronde saat ini, total skor, target).

### `ScoringSystem` (`src/ScoringSystem.h/.cpp`)
Menyimpan daftar (chain) modifier yang aktif dalam `std::vector<IModifier*>`. Fungsi `calculateFinalScore(baseScore)` mengiterasi seluruh chain dan menerapkan setiap modifier secara berurutan. Menampilkan log setiap langkah transformasi skor.

### `ShopSystem` (`src/ShopSystem.h/.cpp`)
Mengelola koin pemain dan interaksi toko. Memanggil `ModifierFactory::create()` untuk membuat modifier yang dipilih pemain, lalu menyerahkan ownership-nya ke `ScoringSystem`. Tidak pernah langsung menginstansiasi kelas modifier konkret.

### `ModifierFactory` (`src/ModifierFactory.h/.cpp`)
Satu-satunya kelas yang boleh menginstansiasi kelas modifier konkret. Menyediakan fungsi statis `create(type)` dan `listAvailable()`. Menerapkan prinsip enkapsulasi kreasi objek.

### `IModifier` (`src/modifiers/IModifier.h`)
Interface abstrak dengan tiga method wajib: `apply(score)`, `getName()`, `getDescription()`, dan `clone()`. Semua modifier konkret mewarisi interface ini, memastikan `ScoringSystem` bisa bekerja dengan modifier apapun tanpa mengetahui kelas konkretnya.

---

## Pattern Usage

### Creational — Factory (`src/ModifierFactory.cpp`)
`ModifierFactory::create(type)` memusatkan pembuatan semua objek `IModifier`. Kelas lain (`ShopSystem`) hanya memanggil Factory dan menerima pointer `IModifier*` tanpa mengetahui kelas konkret yang dibuat. Menambah modifier baru hanya membutuhkan perubahan di Factory, tidak di kelas lain.

### Structural — Decorator Chain (`src/ScoringSystem.cpp`)
`ScoringSystem` mengelola `vector<IModifier*>` dan menerapkannya satu per satu. Output satu modifier menjadi input modifier berikutnya — persis seperti Decorator membungkus Decorator. Interface `IModifier` yang seragam memungkinkan penggabungan modifier apapun dalam urutan apapun.

### Behavioral — Strategy (`src/modifiers/IModifier.h` + semua konkret)
Setiap kelas modifier adalah implementasi "strategi" yang berbeda untuk memodifikasi skor. `ScoringSystem` bekerja melalui interface `IModifier` tanpa mengetahui strategi spesifik yang digunakan. Strategi bisa ditambah, diganti, atau diurutkan ulang di runtime.

---

## Modification Log — Penambahan `SquareScoreModifier`

`SquareScoreModifier` ditambahkan untuk mendemonstrasikan extensibility sistem (analisis A2).

**File yang dibuat:**
- `src/modifiers/SquareScoreModifier.h` — kelas baru mengimplementasikan `IModifier`, method `apply()` mengembalikan `score * score`.

**File yang diubah:**
- `src/ModifierFactory.cpp` — tambah `else if (type == "square") return new SquareScoreModifier();` di fungsi `create()`, dan satu baris di `listAvailable()`.

**File yang TIDAK diubah:**
- `IModifier.h`, `ScoringSystem.h/.cpp`, `ShopSystem.h/.cpp`, `RunSession.h/.cpp`, `main.cpp`

Total perubahan: 1 file baru + 2 baris tambahan di Factory. Ini membuktikan bahwa arsitektur sistem mengikuti **Open/Closed Principle** dengan baik.

---

## AI Usage Disclosure

AI (Claude) digunakan sebagai asisten dalam pengerjaan tugas ini dengan cara berikut:

- **Struktur awal proyek:** AI membantu merancang daftar file dan pembagian tanggung jawab antar kelas.
- **Boilerplate kode:** AI menghasilkan struktur dasar kelas seperti header dan method signature.
- **Analisis pattern:** AI membantu memformulasikan penjelasan konseptual untuk pattern seperti Singleton, Composite, dan Observer.

**Yang dikerjakan sendiri / dipahami secara mandiri:**
- Keputusan desain arsitektur (kenapa Factory, kenapa chain bukan nested wrapper)
- Logika gameplay (perhitungan skor, sistem koin, kondisi menang)
- Semua kode dipahami dan mampu dijelaskan baris per baris

Sesuai kebijakan kelas: AI boleh digunakan sebagai asisten, namun mahasiswa harus memahami dan mampu menjelaskan seluruh kode yang dikumpulkan.
