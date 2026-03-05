# Judul Prototype Game: Cyber Survival: Data Defense

**Cyber Survival: Data Defense** adalah game prototipe *roguelike deckbuilder* berbasis teks yang dibangun dengan C++, terinspirasi oleh mekanisme permainan Balatro. Proyek ini merupakan tugas penerapan *Design Patterns* untuk menciptakan arsitektur perangkat lunak yang *extensible* dan modular.

---

## Architecture Overview
Berikut adalah peran dari class-class utama  dalam arsitektur game ini:

**1. Core Systems**
* **`RunSession`**: Bertindak sebagai *Game Manager* utama. Mengelola *Game Loop* (fase *Battle* dan *Shop*), menjaga persistensi *Master Deck* pemain antar ronde, dan menangani nyawa (Integrity) serta batas *Wave* musuh.
* **`ScoringSystem`**: Modul cerdas yang mengevaluasi kartu yang dimainkan pemain. Sistem ini mendeteksi kombinasi tangan (*Flush*, *Pair*, dll.) dan menghitung total *damage* berdasarkan *Base Code*, *Multiplier* (Amp), serta efek *enhancement* spesifik dari setiap kartu.
* **`ShopSystem`**: Mengelola transaksi di *Dark Web Vendor*. Memungkinkan pemain menggunakan *Integrity* mereka untuk meng-install *Defense Scripts*  atau mengeksekusi *Hacker Protocols* (Consumable item).

**2. Item Generators & Factories**
* **`ModifierFactory`**: Merupakan class yang memproduksi class lain (*Factory*) yang mensentralisasi pembuatan objek-objek *Defense Script*. `ShopSystem` memanggil kelas ini untuk menghasilkan modifier secara dinamis tanpa perlu mengetahui detail implementasi kelas aslinya.
* **`HackerFactory`**: Berfungsi serupa dengan `ModifierFactory`, namun dikhususkan untuk memproduksi kartu *Hacker Protocols*  yang diacak menggunakan sistem *RNG* (*Random Number Generator*).

**3. Item Directories (Modifiers & Consumables)**
* **`modifiers/` (Defense Scripts)**: Direktori ini berisi turunan dari antarmuka `IModifier`. Berfungsi sebagai *buff* pasif yang aktif selama pertempuran. Terdiri dari:
  * `DoubleScoreModifier`: Melipatgandakan *damage* akhir (Overclock Daemon).
  * `FlatBonusModifier`: Menambahkan *damage* dasar secara konstan (Firewall Script).
  * `MemoryExpansionModifier`: Memperluas kapasitas maksimum kartu di tangan pemain (RAM Upgrade).
* **`consumables/` (Hacker Protocols)**: Direktori ini berisi turunan dari antarmuka `IHackerCard`. Berfungsi sebagai item sekali pakai (langsung eksekusi). Terdiri dari:
  * Mekanik Pemulihan: `Antivirus` untuk menyembuhkan *Integrity* sistem.
  * Mekanik *Upgrade*: `LevelPatch` untuk menaikkan level algoritma *hand* dasar (misal: Pair v1 -> v2).
  * Injeksi Status Kartu (*Enhancements*): `GlassInjection` (Corrupted), `ShieldProtocol` (Firewall), dan `SignalBooster` (Optimized/Overclocked) yang memodifikasi atribut kartu di dalam *master deck* secara permanen.
---

## Pattern Usage
Sistem permainan ini mengimplementasikan beberapa pola desain untuk memenuhi standar  *Object-Oriented Programming*:

* **Factory Pattern (Creational)**: 
  Pola ini diterapkan pada `ModifierFactory` dan `HackerFactory`. Kelas Factory bertanggung jawab penuh untuk menginisialisasi (*instantiate*) objek modifier atau item. Hal ini membuat `ShopSystem` tidak perlu mengetahui detail rumit dari pembuatan masing-masing item, sehingga mematuhi prinsip *Open/Closed*.
* **Decorator Concept (Structural)**: 
  Pola *Decorator* diimplementasikan pada antarmuka `IModifier`. Objek modifier yang dibeli pemain disimpan dalam *list* dan akan dipanggil secara berurutan di `RunSession` untuk mendekorasi/memodifikasi nilai *damage* akhir (*Base Damage*) secara dinamis sebelum menyerang Virus.
* **Strategy Pattern (Behavioral)**: 
  Digunakan pada `IEnhancementEffect` dan kelas turunannya (seperti `CorruptedEffect` dan `FirewallEffect`). Alih-alih menumpuk kondisi `if-else` yang panjang di `ScoringSystem`, setiap tipe efek kartu (*enhancement*) memiliki kelas strateginya sendiri yang menangani perubahan skor serta efek sampingnya secara mandiri.

---

## Modification Log

1. **Core Structure & Scoring Logic:** Menginisialisasi struktur dasar *class* dan mengimplementasikan algoritma pendeteksi serta kalkulasi skor kombinasi kartu dasar.
2. **System Integration:** Menghubungkan modul `ScoringSystem` dan purwarupa `ShopSystem` ke dalam *game loop* utama di `RunSession`.
3. **Consumables & Enhancements:** Mengembangkan mekanik *Hacker Protocols* dan *Card Enhancements* (seperti efek *Corrupted*, *Firewall*, dll.) menggunakan pola arsitektur *Strategy*.
4. **Theming & Balancing:** Melakukan *rebranding* pada *modifier* dasar (menjadi tema *Tech Hacker*) serta menyesuaikan kalkulasi *damage* dan harga item agar permainan lebih seimbang.
5. **Final Integration & Extensibility:** Menyelesaikan integrasi mekanik toko, menerapkan perlindungan input, dan mendemonstrasikan kemudahan penambahan fitur baru melalui `MemoryExpansionModifier`.
Berkat penerapan pola *Factory* dan *Decorator*, fitur-fitur baru ini dapat langsung berfungsi dan terintegrasi ke dalam toko (*ShopSystem*) serta arena pertempuran (*RunSession*) tanpa merusak atau mengubah satupun baris kode logika inti yang sudah ada.

---

## AI Usage Disclosure
Dalam pengerjaan proyek ini, AI (Google Gemini) digunakan sebagai asisten diskusi teknis untuk beberapa hal berikut:
* **Debugging**: Membantu mengidentifikasi dan memperbaiki *error* kompilasi spesifik pada C++ (seperti masalah *include guards*, *scope resolution*, dan kompatibilitas *compiler* `g++`).
* **Specific-C++ Syntax OOP** : AI membantu dalam memberikan saran teknis penulisan kode. Seperti, *foreach* versi C++ serta library-library C++ yang penting dan vital.
* **Syntax Boilerplate**: Mempercepat penulisan sintaks dasar kelas turunan di C++.

Sementara perancangan *Game Loop*, aturan *scoring*, mekanik permainan, serta keputusan penerapan *Design Pattern* dirancang, dipahami, dan dikerjakan secara mandiri.