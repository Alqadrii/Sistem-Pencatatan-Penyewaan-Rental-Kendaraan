#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>
#include <vector>
using namespace std;

struct dataKendaraan{
    string jenis;
    string merek;
    string tipe;
    string plat;
    int masaSewa = 0;
    double harga = 0;
    bool status = true;
};

void statusKendaraan(const vector<dataKendaraan> &kendaraan);

#endif