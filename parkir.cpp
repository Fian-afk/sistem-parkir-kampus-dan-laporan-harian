#include "DefParkir.h"

vector<DataParkirRecord> DataParkir;
long long TotalPendapatan = 0;

// Konversi format HH.MM (misal 13.30) -> jam desimal (13 + 30/60 = 13.5).
// Jika input invalid (menit >= 60 atau jam di luar 0-23), kembalikan -1.0
double konversi_waktu(double waktu_input) {
    if (!(waktu_input >= 0.0)) return -1.0;
    int jam = static_cast<int>(waktu_input);
    int menit = static_cast<int>(round((waktu_input - jam) * 100));

    if (jam < 0 || jam > 23) return -1.0;
    if (menit < 0 || menit >= 60) return -1.0;

    return jam + (menit / 60.0);
}

// Overload untuk menerima input string seperti "07.30", "7.30", "13:05" atau "7"
double konversi_waktu(const string &waktu_str) {
    // salin dan trim
    string s = waktu_str;
    while (!s.empty() && isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
    while (!s.empty() && isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
    if (s.empty()) return -1.0;

    size_t pos = s.find_first_of(".:");
    int jam = 0;
    int menit = 0;

    try {
        if (pos == string::npos) {
            // tidak ada pemisah, anggap hanya jam
            jam = stoi(s);
            menit = 0;
        } else {
            string jamStr = s.substr(0, pos);
            string menitStr = s.substr(pos + 1);

            if (jamStr.empty()) return -1.0;
            jam = stoi(jamStr);

            if (menitStr.empty()) {
                menit = 0;
            } else {
                // ambil hanya digit dari bagian menit
                string digits;
                for (char c : menitStr) {
                    if (isdigit(static_cast<unsigned char>(c))) digits.push_back(c);
                }
                if (digits.empty()) return -1.0;
                menit = stoi(digits);
                // jika pengguna memasukkan "7.3" kemungkinan maksudnya 7:30, bukan 7:03
                if (digits.length() == 1) menit *= 10;
            }
        }
    } catch (...) {
        return -1.0;
    }

    if (jam < 0 || jam > 23) return -1.0;
    if (menit < 0 || menit >= 60) return -1.0;

    return jam + (menit / 60.0);
}

void hitungBiaya(int index) {
    if (index < 0 || index >= static_cast<int>(DataParkir.size())) return;

    DataParkirRecord &data = DataParkir[index];

    double masuk = konversi_waktu(data.JamMasuk);
    double keluar = konversi_waktu(data.JamKeluar);

    if (masuk < 0.0 || keluar < 0.0) {
        // Tanda bahwa input waktu invalid — atur nilai default
        data.LamaParkir = 0.0;
        data.BiayaParkir = 0;
        return;
    }

    double durasi = keluar - masuk;
    if (durasi < 0.0) durasi += 24.0; // lewat tengah malam

    data.LamaParkir = durasi;
    int jam_bulat = static_cast<int>(ceil(durasi));

    int biaya = TARIF_JAM_PERTAMA;
    if (jam_bulat > 1) biaya += (jam_bulat - 1) * TARIF_JAM_BERIKUTNYA;
    if (biaya > TARIF_MAKSIMUM) biaya = TARIF_MAKSIMUM;

    data.BiayaParkir = biaya;
}

// Hitung ulang total pendapatan dari seluruh DataParkir
void recalcTotalPendapatan() {
    long long total = 0;
    for (const auto &d : DataParkir) total += d.BiayaParkir;
    TotalPendapatan = total;
}

bool platSudahAda(const string &plat) {
    for (const auto &d : DataParkir) {
        if (d.PlatNomor == plat) return true;
    }
    return false;
}

bool validasiPlat(const string& plat) {
    bool adaHuruf = false;
    bool adaAngka = false;

    if (plat.empty()) return false;

    for (char c : plat) {
        if (!isalnum(c)) return false; 
        if (isalpha(c)) adaHuruf = true;
        if (isdigit(c)) adaAngka = true;
    }

    return adaHuruf && adaAngka;
}

void inputData() {
    cout << "\n-- Input Data Parkir Kendaraan --" << endl;
    int N;
    cout << "Masukkan jumlah kendaraan yang ingin ditambahkan (Max " << MAX_KENDARAAN << "): ";
    if (!(cin >> N)) {
        cout << "Input tidak valid. Kembali ke menu." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (N <= 0) {
        cout << "Tidak ada data yang ditambahkan." << endl;
        return;
    }

    // batasi jumlah total agar tidak melebihi MAX_KENDARAAN
    if (static_cast<int>(DataParkir.size()) + N > MAX_KENDARAAN) {
        int allowed = MAX_KENDARAAN - static_cast<int>(DataParkir.size());
        cout << "Hanya tersisa slot untuk " << allowed << " kendaraan. Akan ditambahkan sebanyak itu." << endl;
        if (allowed <= 0) {
            cout << "Kapasitas penuh. Hapus data dulu jika ingin menambah." << endl;
            return;
        }
        N = allowed;
    }

    for (int i = 0; i < N; ++i) {
        DataParkirRecord data;
        cout << "\nKendaraan ke-" << (DataParkir.size() + 1) << endl;
        // Validasi plat nomor unik
        while (true) {
            cout << "Masukkan Plat Nomor: ";
            cin >> data.PlatNomor;
            transform(data.PlatNomor.begin(), data.PlatNomor.end(), data.PlatNomor.begin(), ::toupper);

            if (!validasiPlat(data.PlatNomor)) {
                cout << "Format plat nomor tidak valid. Harus mengandung huruf dan angka, tanpa simbol.\n";
                continue;
            }

            if (!platSudahAda(data.PlatNomor)) {
                break;
            } else {
                cout << "Plat nomor sudah terdaftar. Masukkan plat nomor lain.\n";
            }
        }

        // Baca sebagai string lalu validasi & konversi
        string sMasuk, sKeluar;
        double convertedMasuk = -1.0, convertedKeluar = -1.0;

        while (true) {
            cout << "Masukkan Jam Masuk (format HH.MM atau HH:MM, contoh 7.30 atau 13:05): ";
            cin >> sMasuk;
            convertedMasuk = konversi_waktu(sMasuk); // akan kita tambahkan overload
            if (convertedMasuk >= 0.0) break;
            cout << "Format waktu tidak valid. Coba lagi.\n";
        }

        while (true) {
            cout << "Masukkan Jam Keluar (format HH.MM atau HH:MM, contoh 10.50 atau 15:45): ";
            cin >> sKeluar;
            convertedKeluar = konversi_waktu(sKeluar);
            if (convertedKeluar >= 0.0) break;
            cout << "Format waktu tidak valid. Coba lagi.\n";
        }

        data.JamMasuk = convertedMasuk;
        data.JamKeluar = convertedKeluar;


        // push lalu hitung biaya untuk index valid
        DataParkir.push_back(data);
        int idx = static_cast<int>(DataParkir.size()) - 1;
        hitungBiaya(idx);
    }

    // setelah semua input, hitung ulang total pendapatan
    recalcTotalPendapatan();
    cout << "\nInput selesai. " << N << " record ditambahkan." << endl;
}

void bubbleSort() {
    int n = static_cast<int>(DataParkir.size());
    if (n <= 1) {
        cout << "Data kurang dari 2, tidak perlu diurutkan." << endl;
        return;
    }

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (DataParkir[j].BiayaParkir < DataParkir[j + 1].BiayaParkir) {
                swap(DataParkir[j], DataParkir[j + 1]);
            }
        }
    }
    cout << "Data telah diurutkan berdasarkan Biaya Parkir (tertinggi ke terendah)." << endl;
}

void cariKendaraan() {
    if (DataParkir.empty()) {
        cout << "Belum ada data. Silakan input data terlebih dahulu." << endl;
        return;
    }

    cout << "\n-- Cari Kendaraan Berdasarkan Plat Nomor --" << endl;
    string cariPlat;
    cout << "Masukkan Plat Nomor yang dicari: ";
    cin >> cariPlat;
    transform(cariPlat.begin(), cariPlat.end(), cariPlat.begin(), ::toupper);

    bool ditemukan = false;
    for (const auto &data : DataParkir) {
        if (data.PlatNomor == cariPlat) {
            int dJam = static_cast<int>(data.LamaParkir);
            int dMen = static_cast<int>(round((data.LamaParkir - dJam) * 60));
            if (dMen == 60) { dJam += 1; dMen = 0; }

            cout << "\nKendaraan Ditemukan:" << endl;
            cout << "Plat Nomor  : " << data.PlatNomor << endl;
            cout << "Jam Masuk   : " << fixed << setprecision(2) << data.JamMasuk << endl;
            cout << "Jam Keluar  : " << fixed << setprecision(2) << data.JamKeluar << endl;
            cout << "Lama Parkir : " << dJam << " jam " << dMen << " menit" << endl;
            cout << "Biaya Parkir: Rp " << data.BiayaParkir << endl;
            ditemukan = true;
            break;
        }
    }
    if (!ditemukan) {
        cout << "Kendaraan dengan Plat Nomor " << cariPlat << " tidak ditemukan." << endl;
    }
}

void tampilkanLaporan() {
    cout << "\n" << string(80, '=') << endl;
    cout << "LAPORAN PARKIR KENDARAAN" << endl;
    cout << string(80, '=') << endl;

    cout << left << setw(4) << "No" << " | "
         << setw(12) << "Plat Nomor" << " | "
         << setw(10) << "Jam Masuk" << " | "
         << setw(10) << "Jam Keluar" << " | "
         << setw(12) << "Durasi" << " | "
         << setw(12) << "Biaya (Rp)" << endl;

    cout << string(80, '-') << endl;

    for (size_t i = 0; i < DataParkir.size(); ++i) {
        const auto &d = DataParkir[i];
        int dJam = static_cast<int>(d.LamaParkir);
        int dMen = static_cast<int>(round((d.LamaParkir - dJam) * 60));
        if (dMen == 60) { dJam += 1; dMen = 0; }

        cout << left << setw(4) << (i + 1) << " | "
             << setw(12) << d.PlatNomor << " | "
             << setw(10) << fixed << setprecision(2) << d.JamMasuk << " | "
             << setw(10) << fixed << setprecision(2) << d.JamKeluar << " | "
             << setw(12) << (to_string(dJam) + " jam " + to_string(dMen) + " menit") << " | "
             << setw(12) << d.BiayaParkir << endl;
    }

    cout << string(80, '=') << endl;
    recalcTotalPendapatan(); // pastikan konsisten
    cout << "Total Kendaraan Parkir Hari Ini: " << DataParkir.size() << endl;
    cout << "Total Pendapatan Parkir Hari Ini: Rp " << TotalPendapatan << endl;
    cout << string(80, '=') << endl;
}