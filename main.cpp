#include "DefParkir.h"

int main() {
    inputData();
    tampilkanLaporan();
    bubbleSort();
    tampilkanLaporan();
    cariKendaraan();
    
    cout << "Tekan ENTER untuk keluar...";
    cin.ignore();
    cin.get();

    return 0;
}