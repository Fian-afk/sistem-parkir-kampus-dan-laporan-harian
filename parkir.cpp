#include "Defparkir.h"

vector<DataParkirRecord> DataParkir;
long long TotalPendapatan = 0;

double konversi_waktu(double waktu_str) {
    if(waktu_str < 0 || waktu_str >= 24.00) return 0.0;
    int jam = static_cast<int>(waktu_str);
    double menit_desimal = (waktu_str - jam) * 100.0 / 60.0;
    return jam + menit_desimal;
}

void hitungBiaya(int index) {
    DataParkirRecord &data = DataParkir[index];

    double jam_masuk_desimal = konversi_waktu(data.JamMasuk);
    double jam_keluar_desimal = konversi_waktu(data.JamKeluar);
    double Durasi = jam_keluar_desimal - jam_masuk_desimal;

    if (Durasi < 0) {
        Durasi += 24.0;
    }

    int LamaDibulatkan = static_cast<int>(ceil(Durasi));
    data.LamaParkir = Durasi;

    int Biaya = 0;
    if (LamaDibulatkan <= 1) {
        Biaya = TARIF_JAM_PERTAMA;
    } else {
        Biaya = TARIF_JAM_PERTAMA + (LamaDibulatkan - 1) * TARIF_JAM_BERIKUTNYA;
    }
    if (Biaya > TARIF_MAKSIMUM) {
            Biaya = TARIF_MAKSIMUM;
    }
    data.BiayaParkir = Biaya;
    TotalPendapatan += Biaya;
}

void inputData() {
    cout << "-- Input Data Parkir Kendaraan --" << endl;
    int N;
    cout << "Masukkan jumlah kendaraan yang parkir hari ini (Max " << MAX_KENDARAAN << "): ";

    if (!(cin >> N)) {
        cout << "Input tidak valid." << endl;
        N = 0;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (N > MAX_KENDARAAN) {
        N = MAX_KENDARAAN;
    }

    for (int i = 0; i < N; ++i) {
        DataParkirRecord data;
        cout << "Kendaraan ke-" << (i + 1) << endl;
        cout << "Masukkan Plat Nomor: ";
        cin >> data.PlatNomor;
        cout << "Masukkan Jam Masuk (format 24 jam, misal 13.30): ";
        cin >> data.JamMasuk;
        cout << "Masukkan Jam Keluar (format 24 jam, misal 15.45): ";
        cin >> data.JamKeluar;

        transform(data.PlatNomor.begin(), data.PlatNomor.end(), data.PlatNomor.begin(), ::toupper);
        hitungBiaya(DataParkir.size()-1);
        DataParkir.push_back(data);
    }
}

void bubbleSort() {
    int n = DataParkir.size();
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
    cout << "-- Cari Kendaraan Berdasarkan Plat Nomor --" << endl;
    string cariPlat;
    cout << "Masukkan Plat Nomor yang dicari: ";
    cin >> cariPlat;
    transform(cariPlat.begin(), cariPlat.end(), cariPlat.begin(), ::toupper);
    bool ditemukan = false;
    for (const auto &data : DataParkir) {
        if (data.PlatNomor == cariPlat) {
            cout << "Kendaraan Ditemukan:" << endl;
            cout << "Plat Nomor: " << data.PlatNomor << endl;
            cout << "Jam Masuk: " << data.JamMasuk << endl;
            cout << "Jam Keluar: " << data.JamKeluar << endl;
            cout << "Lama Parkir: " << data.LamaParkir << " jam" << endl;
            cout << "Biaya Parkir: Rp " << data.BiayaParkir << endl;
            ditemukan = true;
            break;
        }
    }
    if (!ditemukan) {
        cout << "Kendaraan dengan Plat Nomor " << cariPlat << " tidak ditemukan." << endl;
    }
}