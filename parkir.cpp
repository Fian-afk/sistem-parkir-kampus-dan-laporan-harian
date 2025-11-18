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