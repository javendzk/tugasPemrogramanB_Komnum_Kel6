# Tugas Pemrograman B Komputasi Numerik – Analisis Diode Circuit dengan Metode Secant

## Kelompok 6
- Adhi Rajasa Rafif
- Javana Muhammad Dzaki
- Nugroho Ulil Abshar
- Rivi Yasha

## Deskripsi
Proyek ini merupakan implementasi analisis rangkaian dioda menggunakan **Metode Secant** sesuai dengan buku Chapman (Bab 6). Program membaca data permasalahan dari file CSV, menyelesaikan persamaan nonlinear untuk rangkaian dioda dan dioda dengan resistor, lalu menuliskan hasilnya ke file CSV.

## Fitur Utama
- Membaca parameter masalah dari file `data.csv`.
- Menyelesaikan persamaan nonlinear untuk dua tipe masalah:
  - Dioda saja (`diode`)
  - Dioda dengan resistor (`diode_resistor`)
- Menggunakan metode secant untuk mencari solusi.
- Menyimpan hasil simulasi ke file `results.csv`.
- Menampilkan proses iterasi, error, dan waktu eksekusi di terminal.

## Struktur File
- `kode_program.c`  : Source code utama program.
- `data.csv`        : File input parameter masalah (edit sesuai kebutuhan).
- `results.csv`     : File output hasil simulasi.
- `secant_solver.exe`: (Jika ada) File executable hasil kompilasi.

## Cara Menjalankan
1. **Edit file `data.csv`** sesuai format yang diharapkan (lihat contoh di bawah).
2. **Kompilasi program** (gunakan GCC atau compiler C lain):
   ```sh
   gcc kode_program.c -o secant_solver -lm
   ```
3. **Jalankan program:**
   ```sh
   ./secant_solver
   ```
   atau di Windows:
   ```sh
   secant_solver.exe
   ```
4. **Lihat hasil di `results.csv`** dan output terminal.

## Format File `data.csv`
Baris pertama adalah header. Setiap baris berikutnya berisi satu masalah, dengan format:
```
problem_type,param1_name,param1_value,param2_name,param2_value,...,x0,x1,tolerance,max_iterations
```
Contoh:
```
diode,Is,1e-12,n,1,Vt,0.025,I,0.002,,,,,0.5,0.7,1e-6,100
```

## Penjelasan Parameter
- **problem_type**: `diode` atau `diode_resistor`
- **paramX_name/paramX_value**: Nama dan nilai parameter (lihat kode untuk urutan parameter)
- **x0, x1**: Tebakan awal metode secant
- **tolerance**: Toleransi error
- **max_iterations**: Iterasi maksimum

## Catatan
- Pastikan urutan dan jumlah parameter sesuai dengan tipe masalah.
- Program akan menampilkan peringatan jika metode gagal konvergen.

## Lisensi
Proyek ini untuk keperluan pembelajaran Komputasi Numerik.