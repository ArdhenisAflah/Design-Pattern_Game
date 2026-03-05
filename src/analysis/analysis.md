PART A - Creational Pattern Analysis
A1 - Factory
Where are modifier objects created?
Objek modifier dan consumable tidak saya buat langsung di dalam class yang menggunakanya (seperti ShopSystem), tetapi melalui class Factory khusus.

Class yang membuat: Untuk script modifier dibuat oleh ModifierFactory.cpp, sedangkan untuk item consumable dibuat oleh HackerFactory.cpp.

Kenapa disentralisasi: Ini membuat ShopSystem tidak perlu tau detail cara inisialisasi FlatBonusModifier atau GlassInjection.. Ia hanya perlu meminta object tersebut ke Factory. Jika kedepanya saya ingin menambah script baru, saya cukup mengedit Factory nya saja tanpa merubah logika ShopSystem.cpp.

Referensi file: src/ModifierFactory.cpp pada fungsi CreateModifier() dan src/HackerFactory.cpp pada fungsi CreateRandomHackerCard().

A2 - Extensibility
Explain how you added SquareScoreModifier (Substituted with MemoryExpansionModifier)
Sebagai bukti dari extensibility sistem, saya menambahkan modifier baru yaitu MemoryExpansionModifier untuk menambah kapasitas RAM (hand size).

File yang diubah:

Membuat file src/modifiers/MemoryExpansionModifier.h dan .cpp baru.

Menambahkan virtual int GetHandSizeBonus() { return 0; } ke dalam src/modifiers/IModifier.h sebagai defaut behavior.

Mendaftarkan tipe "memory" di ModifierFactory.cpp.

Menambahkan probabilitas drop nya di ShopSystem::GenerateStock().

Apakah mudah di extend? Sangat mudah.. Karena menggunakan interface IModifier dan factory, penambahan script baru sama sekali tidak mengganggu logic pertempuran utama di RunSession.cpp.

A3 - Factory Method (Conceptual)
If you had multiple shop types, how would Factory Method help?
Jika terdapat banyak jenis shop, saya akan membuat interface IShopFactory yang memiliki fungsi virtual IShop* CreateShop() = 0;. Turunan class nya seperti BlackMarketFactory tinggal mengimplementasikan fungsi tersebut untuk menghasilkan shop dengan inventory yg spesifik,

A4 - Prototype (Conceptual)
Explain how Clone() could help duplicate modifiers.
Jika ada fitur untuk "Menduplikasi Script" di dalam game, pattern Prototype ini akan sangat berguna. Saya bisa menambahkan virtual IModifier* Clone() = 0; di IModifier.h. Nanti class turunanya seperti FlatBonusModifier tinggal membuat implementasi misal return new FlatBonusModifier(this->bonusAmount);. Jadi tidak perlu memanggil factory dari awal jika object nya sudah dimiliki pemain.

A5 - Singleton (Conceptual)
Did you use global objects? Explain advantages and risks.
Tidak, saya tidak menggunakan global object atau pola Singleton secara murni di project ini. RunSession, ScoringSystem, dll saya inisialisasi secara biasa.

Keuntungan Singleton: Memang mudah diakses dari mana saja.

Resiko: Tetapi dapat menciptakan hidden dependency, sulit untuk di testing, dan akan merepotkan jika nantinya saya ingin menambahkan fitur Save/Load, karena datanya menempel terus di memori global.

PART B - Structural Pattern Analysis
B1 - Decorator Concept
Explain how your modifier system behaves like a Decorator pattern.
Sistem modifer saya berkerja mirip seperti Decorator yang menumpuk numpuk nilai. Yang "didekorasi" atau diubah nilainya adalah base damage.

Referensi kode: Di src/RunSession.cpp pada fungsi PlayBlind():

C++
int damage = scoringSystem->PlayHand(...);
for (IModifier* mod : playerModifiers) {
    mod->Apply(damage); 
}
Jadi setiap script yg aktif akan mengambil nilai damage sebelumya, diubah (ditambah/dikali) , lalu diteruskan ke script selanjutnya.

B2 - Modifier Order
Does modifier order affect score?
Sangat berpengaruh. Di dalam kode saya, modifier dieksekusi secara berurutan sesuai dengan saat pemain membeli item tersebut (berdasarkan posisinya di vector playerModifiers).

Contoh: Misal Base Dmg = 1000.
Jika membeli Firewall (+300) dulu kemudian Overclock (x2.4) hasilnya: (1000 + 300) * 2.4 = 3120 DMG.
Tetapi jika Overclock dulu baru Firewall: (1000 * 2.4) + 300 = 2700 DMG.
Oleh karena itu, terdapat elemen strategi bagi pemain untuk menyusun urutan pembelian modifiernya.

B3 - Composite (Conceptual)
Explain how a modifier group could be implemented.
Jika ingin membuat grup modifier, misalnya terdapat mekanik "Zip Folder" yg menggabungkan 3 script menjadi 1 slot.. Saya akan membuat ModifierGroup : public IModifier yang menyimpan vector std::vector<IModifier*>. Ketika fungsi Apply() dipanggil pada grup ini, ia akan melakukan iterasi dan memanggil Apply() ke semua script anak di dalamnya.

B4 - Adapter (Conceptual)
Explain how you would integrate an external scoring system using Adapter.
Jika menggunakan sistem skor eksternal misal terdapat library LegacyScoreCalc::GetPoints(int arr[]), saya tidak bisa langsung memasukkan vector<Card> milik saya kesana. Jadi saya akan membuat class Adapter yang meng-inherit ScoringSystem. Di dalamnya dia akan melakukan convert dari vector<Card> saya menjadi array primitif yang dibutuhkan library tersebut, baru kemudian mengembalikan hasilnya.

B5 - Facade (Conceptual)
Design a GameFacade class for your system.
Jika membuat GameFacade, class ini akan membungkus kompleksitas interaksi antara RunSession dan ShopSystem. Sehingga di main.cpp saya hanya perlu memanggil fungsi sederhana seperti InitGame(), SubmitHand(), atau EnterShop(). Hal ini agar client tidak perlu mengatur transisi memori antar sistem nya secara manual.

PART C - Behavioral Pattern Analysis
C1 - Strategy
Explain where scoring logic exists. How would you support multiple scoring rules?
Logika itungan skor dasar ada di ScoringSystem.cpp, namun untuk logika efek enhancement kartu (seperti Corrupted, Firewall) saya pisahkan menggunakan Strategy Pattern ke dalam class turunan IEnhancementEffect.

Mendukung banyak rule skor: Di ScoringSystem.cpp saya menggunakan std::map<CardEnhancement, IEnhancementEffect*> effectMap. Jadi jika ingin menambah aturan skor baru, saya tinggal membuat class turunannya, lalu mendaftarkanya ke map tersebut.. Tidak perlu menambah block if-else yang panjang lagi di dalam sistem utama.

C2 - Observer (Conceptual)
Identify events in your system. Explain how Observer could improve design.

Event di sistem saya adalah: VirusKilled, CardExploded.

Peningkatan menggunakan Observer: Saat ini, untuk menampilkan teks peringatan merah saat kartu pecah masih di hardcode di ScoringSystem. Jika menggunakan Observer, sistem intinya hanya perlu memberi tahu UI dengan memancarkan event OnCardExploded, lalu class UI lah yang akan menangani untuk mencetak peringatannya.

C3 - Command (Conceptual)
Identify actions that could be represented as Command objects.
Aksi pemain seperti saat memainkan kartu (PlayHandCommand) atau membeli item di toko (BuyShopItemCommand) dapat direpresentasikan sebagai object Command. Ini akan sangat berguna jika nantinya saya ingin mengimplementasikan fitur Undo/Redo, atau merekam action log untuk replay permainan.

C4 - State (Conceptual)
Explain how State pattern could organize run phases.
Saat ini fase permainanya hanya diatur menggunakan loop while prosedural di RunSession::StartGame(). Jika menggunakan State pattern, saya bisa mendefinisikan class class baru seperti BattleState atau ShopState. Jadi perpindahan fasenya akan lebih terstruktur menggunakan currentState->TransitionTo(), sehingga mengurangi resiko bug jika dibandingkan dengan menggunakan statment kondisi if yang panjang.