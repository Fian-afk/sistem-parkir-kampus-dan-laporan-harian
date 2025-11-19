# sistem-parkir-kampus-dan-laporan-harian
Program ini mencatat data kendaraan masuk dan keluar area parkir kampus, menghitung durasi parkir serta biaya berdasarkan waktu. Tujuannya untuk membantu petugas parkir membuat laporan jumlah kendaraan dan total pendapatan harian.

Perubahan pada struktur program dan alur

1. Validasi Plat Nomor

Menambahkan pemeriksaan plat nomor unik (platSudahAda()).

Menghindari duplikasi data kendaraan.

Memaksa input ulang jika plat sudah terdaftar.

2. Perbaikan Konversi Waktu

konversi_waktu() kini melakukan validasi menit ≥ 60 dan jam di luar rentang 0–23.

Input HH.MM (misal 7.30, 13.05, 21.45) sekarang dikonversi secara aman ke jam desimal.

3. Perbaikan Perhitungan Durasi

Durasi tetap dihitung dalam jam desimal untuk keperluan tarif.

Output tampilan tidak lagi memakai desimal (3.33 jam).

Durasi kini ditampilkan sebagai:
X jam Y menit.

4. Pembaruan Output Laporan

Format tabel diperbaiki untuk konsistensi.

Mengganti kolom durasi menjadi format jam-menit.

Header diperbarui agar tidak menampilkan label “Lama (jam)” yang tidak relevan.

5. Pembaruan Output Pencarian Kendaraan

Detail kendaraan ditemukan sekarang menampilkan durasi dalam jam-menit juga.

Output lebih manusiawi dan konsisten dengan laporan utama.

6. Konsistensi Total Pendapatan

Menambahkan recalcTotalPendapatan() dan memanggilnya setelah input untuk memastikan sinkronisasi total biaya.

7. Pembersihan Struktur

Penanganan input invalid lebih aman.

Mengurangi potensi data rusak akibat jam/menit tidak valid.

Memperbaiki flow input data sehingga lebih robust.
