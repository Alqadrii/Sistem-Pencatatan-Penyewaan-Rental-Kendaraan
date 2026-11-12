#include <iostream>
#include "catat.hpp"
#include "status.hpp"


int main(){
    string inputPilihan;
    int pilihan;
    vector<dataPelanggan> pelanggan;
    vector<dataTransaksi> transaksi;
    vector<dataKendaraan> kendaraan = {
        {"Motor", "Yamaha", "Mio", "1234", 0, 100000.0, true},
        {"Motor", "Yamaha", "Mio 125", "2341", 0, 100000.0, true},
        {"Motor", "Yamaha", "NMax", "3412", 0, 150000.0, true},
        {"Motor", "Honda", "Beat", "9999", 0, 100000.0, true},
        {"Motor", "Honda", "Vario", "8888", 0, 150000.0, true},
        {"Mobil", "Toyota", "Avanza", "9988", 0, 500000.0, true},
        {"Mobil", "Toyota", "Sigra", "6767", 0, 500000.0, true},
        {"Mobil", "Toyota", "Fortuner", "6666", 0, 500000.0, true}
    };

    do {
        cout << "\n ===== MENU UTAMA RENTAL UTAMA MOBIL DAN MOTOR =====" << endl;
        cout << "1. Catat Penyewaan" << endl;
        cout << "2. Lihat Status Kendaraan" << endl;
        cout << "3. Catat Pengembalian" << endl;
        cout << "4. Keluar" << endl;

        cout << "Input Pilihan : ";
        getline(cin, inputPilihan);
        
        if (!validasiString(inputPilihan)) {
            cout << "Inputtan tidak valid!" << endl;
            pilihan = 0;
            continue;
        } else {
            try {
                pilihan = stoi(inputPilihan);
            } catch (...) {
                pilihan = 0;
            }
            
            switch (pilihan) {
            case 1:
                cout << "\n========= Menu Catat Penyewaan ========" << endl;
                catatPenyewaan(pelanggan, kendaraan, transaksi);
                break;
            case 2:
                cout << "\n===== Menu Lihat Status Kendaraan =====" << endl;
                statusKendaraan(kendaraan);
                break;
            case 3:
                cout << "\n======= Menu Catat Pengembalian =======" << endl;
                catatPengembalian(kendaraan, transaksi);
                break;
            case 4:
                break;
            default:
                cout << "Input tidak valid!" << endl;
            }
        }
        
    } while (pilihan != 4 );
    cout << "Terima Kasih." << endl;
    return 0;
}