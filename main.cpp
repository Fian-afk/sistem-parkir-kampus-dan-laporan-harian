#include "DefParkir.h"

void tampilkanMenu() {
    cout << "\n=== MENU UTAMA ===" << endl;
    cout << "1. Input Data Kendaraan" << endl;
    cout << "2. Tampilkan Laporan" << endl;
    cout << "3. Cari Kendaraan" << endl;
    cout << "4. Urutkan berdasarkan Biaya Tertinggi" << endl;
    cout << "5. Keluar" << endl;
    cout << "Pilih opsi (1-5): ";
}

int main() {
    bool selesai = false;
    while (!selesai) {
        tampilkanMenu();
        int pilihan;
        if (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka 1-5." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (pilihan) {
            case 1:
                inputData();
                break;
            case 2:
                tampilkanLaporan();
                break;
            case 3:
                cariKendaraan();
                break;
            case 4:
                bubbleSort();
                break;
            case 5:
                cout << "Keluar. Sampai jumpa." << endl;
                selesai = true;
                break;
            default:
                cout << "Pilihan tidak dikenal. Masukkan 1-5." << endl;
                break;
        }
    }

    return 0;
}
