#include "catat.hpp"
#include "status.hpp"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <iomanip>

bool kosongAtauSpasi(const string& str) {
    if (str.empty()) return true; // <-- Sudah mengecek kosong di sini!
    for (unsigned char c : str) {
        if (!isspace(c)) {
            return false; 
        }
    }
    return true; 
}

string ambilTanggalHariIni() {
    auto now = chrono::system_clock::now();
    time_t waktuSekarang = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&waktuSekarang);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime); 
    return string(buffer);
}

static string keHurufKecil(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

bool validasiString(string input) {
    for (char c : input) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != ' ') {
            return false;
        }
    }
    return true;

}

bool validasiStringHuruf(string input) {
    for (char c : input) {
        if (!isalpha(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

void catatPenyewaan(vector<dataPelanggan> &pelanggan, vector<dataKendaraan> &kendaraan, vector<dataTransaksi> &transaksi){
    dataPelanggan p;
    dataKendaraan k;
    dataTransaksi t;
    
    // Input data pelanggan
    bool idPelangganSudahAda = false;
    do {
        cout << "Masukkan ID Pelanggan        : ";
        getline(cin, p.idPelanggan);
        cout << "Masukkan nama pelanggan      : ";
        getline(cin, p.nama);
        cout << "Masukkan Tanggal Lahir       : ";
        getline(cin, p.tglLahir);
        cout << "Masukkan Alamat              : ";
        getline(cin, p.alamat);
        cout << "Masukkan Jenis Kelamin (P/L) : ";
        getline(cin, p.jenisKelamin);

        auto itPelanggan = find_if(pelanggan.begin(), pelanggan.end(), [&p](const dataPelanggan& existingP) {
            return existingP.idPelanggan == p.idPelanggan;
        });
        idPelangganSudahAda = (itPelanggan != pelanggan.end());

        if (kosongAtauSpasi(p.idPelanggan) || 
            kosongAtauSpasi(p.nama) || 
            kosongAtauSpasi(p.alamat) ||
            kosongAtauSpasi(p.tglLahir) || 
            kosongAtauSpasi(p.jenisKelamin) ||
            !validasiString(p.idPelanggan) ||
            !validasiStringHuruf(p.nama) ||
            !validasiString(p.alamat) ||
            !validasiString(p.tglLahir) ||
            !(p.jenisKelamin == "L" || 
            p.jenisKelamin == "l" || 
            p.jenisKelamin == "P" || 
            p.jenisKelamin == "p")
        ) { 
            cout << "\nData tidak valid" << endl;
        } else if (idPelangganSudahAda) {
            cout << "\nID Pelanggan sudah terdaftar! Harap gunakan ID yang berbeda." << endl;
        }
    } while (
            kosongAtauSpasi(p.idPelanggan) || 
            kosongAtauSpasi(p.nama) || 
            kosongAtauSpasi(p.alamat) ||
            kosongAtauSpasi(p.tglLahir) || 
            kosongAtauSpasi(p.jenisKelamin) ||
            !validasiString(p.idPelanggan) ||
            !validasiStringHuruf(p.nama) ||
            !validasiString(p.alamat) ||
            !validasiString(p.tglLahir) ||
            !(p.jenisKelamin == "L" || 
            p.jenisKelamin == "l" || 
            p.jenisKelamin == "P" || 
            p.jenisKelamin == "p") ||
            idPelangganSudahAda
        );

    // 2. Input jenis, merek, tipe -> cari di data master
    vector<dataKendaraan>::iterator itKendaraan;
    while (true) {
        cout << "\nMasukkan Jenis Kendaraan     : ";
        getline(cin, k.jenis);
        cout << "Masukkan Merek Kendaraan     : ";
        getline(cin, k.merek);
        cout << "Masukkan Tipe Kendaraan      : ";
        getline(cin, k.tipe);

        if (k.jenis.empty() || 
            k.merek.empty() || 
            k.tipe.empty() || 
            !validasiStringHuruf(k.jenis) || 
            !validasiStringHuruf(k.merek) || 
            !validasiString(k.tipe)) {
            cout << "\nData tidak valid" << endl;
            continue;
        }

        itKendaraan = find_if(kendaraan.begin(), kendaraan.end(),
            [&k](const dataKendaraan& veh) {
                return keHurufKecil(veh.jenis) == keHurufKecil(k.jenis) &&
                       keHurufKecil(veh.merek) == keHurufKecil(k.merek) &&
                       keHurufKecil(veh.tipe)  == keHurufKecil(k.tipe)  &&
                       veh.status;
            });

        if (itKendaraan == kendaraan.end()) {
            cout << "\nKendaraan tidak ditemukan atau sedang disewa semua!" << endl;
            continue;
        }
        break;
    }

    // Sinkronisasi plat dan harga dari data master
    k.jenis = itKendaraan->jenis;
    k.merek = itKendaraan->merek;
    k.tipe  = itKendaraan->tipe;
    k.plat  = itKendaraan->plat;
    k.harga = itKendaraan->harga;

    cout << "\nKendaraan ditemukan:" << endl;
    cout << "Plat    : " << k.plat  << endl;
    cout << "Harga   : " << k.harga << endl;

    k.masaSewa = 0;
    while (k.masaSewa < 1) {
        string tempMasaSewa;
        cout << "Masukkan Masa Sewa : ";
        getline(cin, tempMasaSewa);
        try {
            k.masaSewa = stoi(tempMasaSewa);
        } catch (...) {
            k.masaSewa = 0;
        }
        if (k.masaSewa < 1) cout << "\nData tidak valid" << endl;
    }

    // 3. Input Data Transaksi
    string tempTglSewa;
    int tglSewaInt = 0;

    bool idSudahAda = false;
    do {
        cout << "\nMasukkan ID Transaksi : ";
        getline(cin, t.idTransaksi);

        auto itTrans = find_if(transaksi.begin(), transaksi.end(), [&t](const dataTransaksi& existingT) {
            return existingT.idTransaksi == t.idTransaksi;
        });
        idSudahAda = (itTrans != transaksi.end());

        if (kosongAtauSpasi(t.idTransaksi) || !validasiString(t.idTransaksi)) {
            cout << "\nData tidak valid! ID Transaksi tidak boleh kosong atau mengandung simbol terlarang." << endl;
        } else if (idSudahAda) {
            cout << "\nID Transaksi sudah terdaftar! Harap gunakan ID yang berbeda." << endl;
        }
    } while (kosongAtauSpasi(t.idTransaksi) || !validasiString(t.idTransaksi) || idSudahAda);
    

    t.tanggalSewa = ambilTanggalHariIni();
    cout << "Tanggal Sewa : " << t.tanggalSewa << endl;
    t.tanggalPengembalian = "";
    
    t.kendaraanDisewa = k;
    t.penyewa = p;
    t.totalSewa = k.harga * k.masaSewa;
    
    string konfirmasi;
    cout << fixed << setprecision(0);
    cout << "Total Sewa : Rp." << t.totalSewa << endl;
    cout << "Konfirmasi Pembayaran (Y) atau input apapun untuk batal : ";
    getline(cin, konfirmasi);
    
    if (konfirmasi == "y" || konfirmasi == "Y") {
        cout << "Transaksi berhasil!" << endl;
        pelanggan.push_back(p);
        transaksi.push_back(t);

        itKendaraan->status = false;
        k.status = false;
    } else {
        cout << "Transaksi dibatalkan!" << endl;
    }
}

void catatPengembalian(vector<dataKendaraan> &kendaraan, vector<dataTransaksi> &transaksi){
    if (transaksi.empty()) {
        cout << "\nBelum ada transaksi penyewaan yang tercatat!" << endl;
        return; 
    }
    
    string id;
    auto it = transaksi.end();

    // cin.ignore();
    while (true) {
        cout << "Masukkan ID Transaksi : ";
        getline(cin, id);

        if (kosongAtauSpasi(id)) {
            cout << "Inputtan tidak boleh kosong" << endl;
            continue;
        }
        if (!validasiString(id)) {
            cout << "Inputtan hanya boleh berupa huruf dan angka" << endl;
            continue;
        }

        it = find_if(transaksi.begin(), transaksi.end(), [&id](const dataTransaksi& t) {
            return t.idTransaksi == id;
        });

        if (it != transaksi.end()) break;
        cout << "ID Transaksi Tidak Ditemukan" << endl;
    }

    if (!it->tanggalPengembalian.empty()) {
        cout << "Transaksi ini sudah dikembalikan pada " << it->tanggalPengembalian << endl;
        return;
    }

    it->tanggalPengembalian = ambilTanggalHariIni();
    cout << "Tanggal Pengembalian : " << it->tanggalPengembalian << endl;

    // Update status kendaraan
    auto itKendaraan = find_if(kendaraan.begin(), kendaraan.end(), [&it](const dataKendaraan& k) {
        return k.plat  == it->kendaraanDisewa.plat &&
               k.jenis == it->kendaraanDisewa.jenis &&
               k.merek == it->kendaraanDisewa.merek &&
               k.tipe  == it->kendaraanDisewa.tipe;
    });

    if (itKendaraan != kendaraan.end()) {
        itKendaraan->status = true;
    }
    it->kendaraanDisewa.status = true;

    cout << "\nStatus Kendaraan Telah diupdate menjadi Tersedia" << endl;
    cout << "Terima Kasih" << endl;
}

void statusKendaraan(const vector<dataKendaraan>& kendaraan) {
    cout << "\n================================ STATUS KENDARAAN ================================\n";
    cout << left 
         << setw(8)  << "Jenis"  << " | "
         << setw(10) << "Merek"  << " | "
         << setw(10) << "Tipe"   << " | "
         << setw(8)  << "Plat"   << " | "
         << setw(12) << "Harga"  << " | "
         << setw(10) << "Status" << endl;
    cout << "----------------------------------------------------------------------------------\n";
    for (const auto& k : kendaraan) {
        cout << left    
             << setw(8)  << k.jenis << " | " 
             << setw(10) << k.merek << " | " 
             << setw(10) << k.tipe  << " | " 
             << setw(8)  << k.plat  << " | " 
             << setw(12) << k.harga << " | " 
             << setw(10) << (k.status ? "Tersedia" : "Disewa") << endl;
    }
    cout << "==================================================================================\n";
}