#ifndef CATAT_HPP
#define CATAT_HPP

#include <string>
#include <vector>
#include "status.hpp"
using namespace std;

struct dataPelanggan{
    string idPelanggan;
    string nama;
    string tglLahir;
    string alamat;
    string jenisKelamin;
};

struct dataTransaksi{
    string idTransaksi;
    dataPelanggan penyewa;
    dataKendaraan kendaraanDisewa;
    double totalSewa;
    string tanggalSewa;
    string tanggalPengembalian;
};

void catatPenyewaan(vector<dataPelanggan> &pelanggan, vector<dataKendaraan> &kendaraan, vector<dataTransaksi> &transaksi);
void catatPengembalian(vector<dataKendaraan> &kendaraan, vector<dataTransaksi> &transaksi);
bool validasiString(string input);
bool kosongAtauSpasi(const string& str);
bool validasiStringHuruf(string input);
static string keHurufKecil(string s);

#endif