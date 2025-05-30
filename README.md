# Tugas Pemrograman B Komputasi Numerik – Analisis Diode Circuit dengan Metode Secant

## Kelompok 6
- Adhi Rajasa Rafif        2306266943      
- Javana Muhammad Dzaki    2306161826
- Nugroho Ulil Abshar      2306229310
- Rivi Yasha Hafizhan      2306250535

## Deskripsi
Proyek ini merupakan implementasi analisis rangkaian dioda menggunakan Metode Secant sesuai dengan buku Chapman (Bab 6). Program membaca data permasalahan dari file CSV, menyelesaikan persamaan nonlinear untuk rangkaian dioda dan dioda dengan resistor, lalu menuliskan hasilnya ke file CSV

Implementasi ini mencakup analisis komprehensif terhadap 31 kasus uji dengan tingkat keberhasilan konvergensi mencapai **93,5%**, menunjukkan efektivitas metode Secant dalam menyelesaikan persamaan nonlinear rangkaian dioda

## Struktur File
- `kode_program.c`         : Source code utama program
- `data-io/data.csv`       : File input parameter masalah 
- `data-io/results.csv`    : File output hasil simulasi
- `secant_solver.exe`      : File executable hasil kompilasi
- `visualisasi/`           : Folder berisi script dan hasil visualisasi
- `referensi/`             : Folder berisi referensi dan contoh implementasi
- `TugasPemrogramanB_Kelompok_6.pdf` : Laporan lengkap dalam format PDF

## Cara Menjalankan
1. **Edit file `data-io/data.csv`** sesuai format yang diharapkan (lihat contoh di bawah)
2. **Kompilasi program** 
   ```bash
   gcc kode_program.c -o secant_solver -lm
   ```
3. **Jalankan program:**
   ```bash
   ./secant_solver
   ```
   atau di Windows:
   ```bash
   ./secant_solver.exe
   ```
4. **Lihat hasil di `data-io/results.csv`** dan output terminal

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
- **paramX_name/paramX_value**: Nama dan nilai parameter 
- **x0, x1**: Tebakan awal metode secant
- **tolerance**: Toleransi error
- **max_iterations**: Iterasi maksimum

## Alur Program dan Metodologi

![Flowchart Program](https://i.imgur.com/tnSf04W.png)

Analisis rangkaian dioda dilakukan dengan metode numerik Secant yang diawali dengan membaca data input dari file `data.csv` dan memberikan output analisis ke `results.csv`. Alur program dimulai dari fungsi `main()` yang memanggil `readCsvFile()` untuk mem parsing data yang perlu diinput (tipe masalah, parameter seperti Is, n, Vt, tebakan awal, toleransi, dan iterasi maksimum)

Selanjutnya, program menganalisis setiap kasus yang terdapat pada data untuk dipilih proses yang sesuai antara:

1. **`diodeFunction()`** yang memiliki formula:
   ```
   f(v) = Is * (e^(v/(n*Vt)) - 1) - I
   ```

2. **`diodeResistorFunction()`** yang menghitung berdasarkan formula:
   ```
   f(v) = Is * (e^(v/(n*Vt)) - 1) - (Vs - v)/R
   ```

Kemudian, program mencatat periode eksekusi dengan menggunakan fungsi `clock()`. Selanjutnya, data hasil dikirim ke `secantMethod()` yang mengimplementasikan metode Secant dengan strategi stabilitas seperti damping factor dan limit osilasi, sembari menampilkan setiap iterasi hingga mencapai toleransi atau batas iterasi

Akhirnya, seluruh hasil analisis (detail parameter, solusi, status konvergensi, error, dan waktu eksekusi) disimpan ke `results.csv` melalui fungsi `writeCsvFile()`

## Hasil Eksperimen dan Analisis

### Analisis Konvergensi Metode Secant

![Analisis Konvergensi](https://i.imgur.com/Hdi4Vwd.png)

Implementasi metode Secant pada 31 kasus uji rangkaian dioda menunjukkan performa konvergensi yang sangat baik dengan tingkat keberhasilan mencapai **93,5%**. Terdapat perbedaan signifikan antara rangkaian dioda tunggal yang mencapai konvergensi **83,3%** (10/12 kasus) dibandingkan dengan rangkaian dioda-resistor yang mencapai konvergensi sempurna **100%** (19/19 kasus) 

Keunggulan rangkaian dioda-resistor dalam konvergensi ini dapat dijelaskan melalui adanya efek pembatasan natural dari resistor yang menstabilkan proses iteratif

### Analisis Tegangan Dioda

![Analisis Tegangan Dioda](https://i.imgur.com/vl2JOPb.png)

Analisis tegangan dioda menunjukkan distribusi yang bervariasi tergantung pada parameter fisika komponen dan konfigurasi rangkaian:

#### Rangkaian Dioda Tunggal
- Tegangan forward berkisar antara **0,589V hingga 0,983V** (mengabaikan dua kasus yang gagal konvergen)
- Mayoritas nilai berkisar **0,7V-0,8V** sesuai dengan karakteristik praktis dioda silikon

#### Rangkaian Dioda-Resistor
- Rentang tegangan yang lebih luas dari **0,627V hingga 1,036V**
- Dua outlier pada **2,339V dan 3,019V** untuk kasus dengan faktor idealitas tinggi (n = 2,0 dan n = 2,1)

#### Pengaruh Faktor Idealitas
Pengaruh faktor idealitas (n) terhadap tegangan dioda menunjukkan **korelasi positif** yang jelas, di mana peningkatan nilai n menyebabkan tegangan forward yang lebih tinggi untuk arus yang sama. Hal ini sesuai dengan teori dioda di mana n menggambarkan deviasi dari karakteristik ideal, dengan n > 1 mengindikasikan ada kombinais tambahan di junction diodanya


## Kesimpulan

Implementasi metode Secant untuk analisis rangkaian dioda terbukti efektif dengan tingkat keberhasilan **93,5%**. Rangkaian dioda-resistor menunjukkan stabilitas konvergensi yang lebih baik jika dibandingkan rangkaian dioda tunggal, sehingga penggunaan Metode Secant bisa dibilang sudah tepat
