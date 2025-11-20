#ifndef DEFPARKIR_H
#define DEFPARKIR_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

const int MAX_KENDARAAN = 100;
const int TARIF_JAM_PERTAMA = 3000;
const int TARIF_JAM_BERIKUTNYA = 2000;
const int TARIF_MAKSIMUM = 15000;

struct DataParkirRecord {
    string PlatNomor;
    double JamMasuk;
    double JamKeluar;
    double LamaParkir;
    int BiayaParkir;
};

extern vector<DataParkirRecord> DataParkir;
extern long long TotalPendapatan;

bool platSudahAda(const string &plat);
bool validasiPlat(const string &plat);

double konversi_waktu(double waktu_input); 
void hitungBiaya(int index);
void recalcTotalPendapatan();
void inputData();
void bubbleSort();
void cariKendaraan();
void tampilkanLaporan();

#endif