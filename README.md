# Praktikum Struktur Data C++ — Pertemuan 7

## Binary Tree dan Traversal

Repository ini dibuat otomatis oleh aplikasi praktikum. Setiap kali Anda
melakukan **push**, GitHub Actions akan mengompilasi kode Anda, menjalankan
test, dan mengirim nilainya ke aplikasi.

---

## Tujuan Praktikum

Sampai Pertemuan 6, semua struktur yang Anda buat bentuknya **lurus**: satu node
menyambung ke satu node berikutnya, seperti antrean orang. Linked list, stack,
dan queue semuanya begitu.

Pertemuan ini memperkenalkan bentuk yang berbeda. Satu node boleh menyambung ke
**dua** node sekaligus, sehingga susunannya bercabang seperti pohon terbalik —
akarnya di atas, rantingnya menjulur ke bawah. Struktur seperti ini dipakai
kapan pun datanya punya hubungan **bertingkat**: struktur organisasi, daftar
folder di dalam folder, atau silsilah keluarga.

Karena sebuah cabang bentuknya sama persis dengan pohon utuh, pekerjaan pada
tree paling enak ditulis secara **rekursif**. Itulah keterampilan utama yang
dilatih pertemuan ini.

Setelah pertemuan ini Anda diharapkan mampu:

- menjelaskan perbedaan struktur lurus dan struktur bertingkat;
- mengenali root, child, leaf, dan subtree pada sebuah tree;
- membaca sebuah node yang menyimpan `data`, `left`, dan `right`;
- menjelaskan mengapa rekursi cocok untuk tree, dan apa titik berhentinya;
- menuliskan traversal preorder, inorder, dan postorder;
- memperkirakan urutan hasil traversal **sebelum** menjalankan program;
- menghitung banyaknya node secara rekursif;
- menangani tree kosong, tree satu node, dan tree yang tidak seimbang; dan
- membebaskan seluruh node tree dengan benar.

Setiap soal di bawah ditulis sebagai **tujuan dan hasil yang harus tercapai**,
bukan sebagai langkah pengerjaan. Menentukan cara mencapainya adalah bagian dari
latihan ini.

---

## Materi

| Konsep | Yang perlu Anda kuasai |
|---|---|
| Node | satu satuan penyimpanan: sebuah data + dua alamat cabang |
| `root` | penanda node paling atas; sekaligus penanda tree kosong |
| `left` / `right` | dua cabang sebuah node; `NULL` berarti tidak ada cabang |
| Leaf (daun) | node yang tidak punya cabang sama sekali |
| Subtree | bagian tree yang berakar pada suatu node — bentuknya sama dengan tree utuh |
| Rekursi | mengerjakan node ini, lalu menyerahkan sisanya ke kedua cabang |
| Titik berhenti | `root == NULL`, keadaan paling sederhana |
| Preorder | Root - Left - Right |
| Inorder | Left - Root - Right |
| Postorder | Left - Right - Root |
| Tree kosong | keadaan sah yang harus ditangani setiap fungsi |
| Tree tidak seimbang | keadaan umum; kedua cabang tetap harus dikunjungi |
| Pembebasan | node yang dibuang tidak boleh ditinggalkan di memori |

### Tree yang dipakai di seluruh contoh

Supaya mudah diikuti, semua contoh pada dokumen ini memakai tree yang sama —
yang sama pula dengan tree pada modul:

```
              50          <- root
             /  \
           80    20
          /  \     \
        10    70    90
             /
           30
```

Bacanya: node 50 punya cabang kiri 80 dan cabang kanan 20. Node 70 punya cabang
kiri 30 dan tidak punya cabang kanan. Node 10, 30, dan 90 tidak punya cabang
sama sekali — merekalah **daun**-nya.

> **Ini BUKAN Binary Search Tree.** Perhatikan 80 yang berada di kiri 50
> walaupun 80 lebih besar. Pada pertemuan ini `left` dan `right` cuma menyatakan
> **posisi**, bukan urutan besar-kecil. Aturan pengurutan baru muncul di
> pertemuan berikutnya.

---

## Untuk Mahasiswa

### File yang Harus Dikerjakan

**Satu-satunya file yang dinilai adalah:**

```
src/student.cpp
```

Di bagian paling bawah `src/student.cpp` ada `main()`. Bagian itu membuat file
ini menjadi program C++ utuh yang bisa langsung Anda jalankan untuk mencoba
implementasi Anda — dan `main()` tersebut **tidak ikut dinilai**, jadi **bebas
Anda ubah** sesukanya. Checker tidak memakainya sama sekali.

Jangan mengubah file lain. Perubahan pada `src/student.h`, `tests/checker.cpp`,
`tests/report.h`, atau `.github/workflows/` tidak akan membuat nilai Anda naik
dan dapat menyebabkan penilaian gagal.

### Contract / API

Checker memanggil fungsi Anda secara langsung, jadi baris-baris berikut wajib
persis seperti ini (sudah dideklarasikan di `src/student.h`):

```cpp
struct Node {
    int data;
    Node* left;
    Node* right;
};

void preorder(const Node* root);
void inorder(const Node* root);
void postorder(const Node* root);
void clear(Node*& root);
int  jumlahDaun(const Node* root);
```

Yang **wajib sama**: nama `struct Node` beserta ketiga field-nya, nama fungsi,
tipe parameter, dan tipe kembalian.

Yang **bebas Anda tentukan**: seluruh isi fungsi, nama variabel, mau rekursif
atau memakai loop, mau menambah fungsi bantu atau tidak. Penilaian hanya melihat
akibat yang teramati oleh pemanggil.

### Istilah yang Dipakai di Seluruh Soal

| Istilah | Artinya |
|---|---|
| **node** | Satu kotak penyimpanan. Isinya tiga hal: sebuah angka (`data`), alamat cabang kiri (`left`), dan alamat cabang kanan (`right`). |
| **tree** | Rangkaian node yang bercabang-cabang, berbentuk seperti pohon terbalik. |
| `root` | Alamat node **paling atas**. Cukup dengan memegang `root`, seluruh isi tree bisa dicapai. |
| `left` / `right` | Alamat cabang kiri dan cabang kanan sebuah node. Cabang yang tidak ada bernilai `NULL`. |
| **daun** (leaf) | Node yang **tidak punya cabang sama sekali** — `left` dan `right` sama-sama `NULL`. |
| **subtree** | Bagian tree yang berakar pada suatu node. Bentuknya sama persis dengan tree utuh. |
| `NULL` | Alamat kosong. `root` bernilai `NULL` berarti tree sedang **kosong**. |

### Mengapa Rekursi

Perhatikan cabang kiri dari tree contoh:

```
           80
          /  \
        10    70
             /
           30
```

Bentuknya **sama persis** dengan tree utuh: ada node paling atas, ada cabang
kiri, ada cabang kanan. Karena itu pekerjaan pada tree dapat ditulis begini:
kerjakan node yang sedang dipegang, lalu minta cabang kiri dan cabang kanan
mengerjakan pekerjaan yang sama untuk dirinya masing-masing.

Sebuah fungsi yang memanggil dirinya sendiri harus punya **titik berhenti**,
kalau tidak ia akan memanggil dirinya terus-menerus sampai program berhenti
tidak wajar. Pada tree, titik berhentinya adalah keadaan paling sederhana:
subtree yang kosong, yaitu `root == NULL`.

### Kondisi yang Berlaku untuk Semua Soal

- Sebuah tree dinyatakan seluruhnya oleh `root`. `root` bernilai `NULL`
  berarti tree sedang **kosong**, dan itu keadaan yang sah — bukan kesalahan.
- Cabang yang tidak ada bernilai `NULL`. Sebuah node boleh punya dua cabang,
  satu cabang saja, atau tidak sama sekali.
- Tree tidak harus seimbang. Cabang kiri boleh jauh lebih dalam daripada cabang
  kanan, atau sebaliknya. Kedua cabang tetap harus dikerjakan.
- `left` dan `right` **tidak** menyatakan urutan besar-kecil.
- Data boleh bernilai negatif, boleh bernilai nol, dan boleh muncul lebih dari
  sekali di dalam satu tree.
- Kecuali `clear`, tidak ada fungsi yang boleh mengubah isi tree.

---

## Daftar Soal

Ada **3 soal** dan **5 fungsi**. Penjelasan yang sama juga ada di
`src/student.cpp`, tepat di atas masing-masing fungsi, dengan empat bagian yang
selalu sama:

| Bagian | Isinya |
|---|---|
| **Ceritanya** | Apa yang sedang terjadi, dan apa yang diminta |
| **Parameternya** | Arti setiap parameter, satu per satu |
| **Contoh** | Contoh nyata beserta hasil yang benar |
| **Yang perlu diingat** | Hal khusus yang ikut dinilai |

| Soal | Isi | Fungsi | Bobot |
|---|---|---|---:|
| 1 | Traversal | `preorder`, `inorder`, `postorder` | 60 |
| 2b | Membersihkan | `clear` | 25 |
| 3 | Soal cerita | `jumlahDaun` | 15 |

---

## Soal 1 — Traversal (60 poin)

**Traversal** artinya mengunjungi **setiap** node tree, masing-masing tepat satu
kali, menurut urutan tertentu.

Ketiga fungsi di soal ini mengunjungi node yang **sama persis** dan **sama
banyaknya**. Yang membedakan hanya **kapan** sebuah node dicetak dibandingkan
kedua cabangnya.

### Ketiganya mencetak, bukan mengembalikan nilai

Tipe kembaliannya `void`. Hasilnya ditulis langsung ke layar dengan `cout`,
persis seperti pada modul.

Bentuk keluaran yang berlaku untuk ketiganya:

- Setiap angka dicetak apa adanya dan dipisahkan oleh spasi.
- Tree kosong **tidak mencetak apa pun**.
- Setiap node dicetak tepat sekali.
- Jangan mencetak apa pun selain angka dan spasi — tidak ada judul seperti
  `Preorder:`, tidak ada koma, tidak ada tanda kurung.

> **Spasi tidak dinilai.** Spasi menggantung di akhir baris, spasi ganda, dan
> pergantian baris di antara angka **tidak** mengurangi nilai. Checker hanya
> melihat **urutan angkanya**. Jadi Anda tidak perlu repot menghindari spasi di
> akhir.

### Hasil pada tree contoh

```
              50
             /  \
           80    20
          /  \     \
        10    70    90
             /
           30
```

| Fungsi | Urutan kunjungan | Yang dicetak |
|---|---|---|
| `preorder` | Root - Left - Right | `50 80 10 70 30 20 90` |
| `inorder` | Left - Root - Right | `10 80 30 70 50 20 90` |
| `postorder` | Left - Right - Root | `10 30 70 80 90 20 50` |

Perhatikan angka **50** (root): muncul di **awal** pada preorder, di **tengah**
pada inorder, dan di **akhir** pada postorder. Itulah inti perbedaan ketiganya.

### Soal 1a — `preorder` (20 poin)

**Ceritanya.** Sebuah node dicetak **lebih dulu**, baru sesudah itu kedua
cabangnya dikerjakan. Aturan itu berlaku di **setiap** node, bukan cuma di
`root`.

**Parameternya.**

| Parameter | Artinya |
|---|---|
| `root` | Alamat node paling atas tree yang mau dicetak. Boleh bernilai `NULL`, artinya tree sedang kosong |
| *kembalian* | Tidak ada (`void`). Hasilnya dicetak ke layar |

**Contoh.** Runut pelan-pelan supaya urutannya masuk akal:

```
50 dicetak dulu (dia root), lalu giliran cabang kirinya;
80 dicetak, lalu giliran cabang kirinya;
10 dicetak, tidak punya cabang, selesai;
70 dicetak, lalu 30 (cabang kirinya), cabang kanannya tidak ada;
cabang kiri 50 sudah habis, sekarang cabang kanannya;
20 dicetak, cabang kirinya tidak ada, lalu 90.

hasilnya:  50 80 10 70 30 20 90
```

Keadaan lain:

```cpp
preorder(NULL);          // tidak mencetak apa pun

Node satu{42, NULL, NULL};
preorder(&satu);            // mencetak:  42
```

**Yang perlu diingat.**
- Setiap node dicetak **tepat satu kali**.
- Tree kosong tidak mencetak apa pun, dan program tidak boleh berhenti tidak
  wajar.
- Mencetak tidak boleh mengubah tree. Memanggilnya dua kali berturut-turut harus
  mencetak hal yang sama persis.

---

### Soal 1b — `inorder` (20 poin)

**Ceritanya.** Cabang **kiri** dikerjakan sampai habis lebih dulu, **baru** node
itu sendiri dicetak, baru kemudian cabang kanannya dikerjakan.

**Parameternya.**

| Parameter | Artinya |
|---|---|
| `root` | Alamat node paling atas tree yang mau dicetak. Boleh bernilai `NULL` |
| *kembalian* | Tidak ada (`void`). Hasilnya dicetak ke layar |

**Contoh.** Pada tree contoh menghasilkan `10 80 30 70 50 20 90`.

Bandingkan dengan preorder yang menghasilkan `50 80 10 70 30 20 90`. Angkanya
sama, hanya urutannya yang berpindah. Yang paling kentara: 50 tadinya di paling
depan, sekarang berada di **tengah** — karena seluruh cabang kirinya harus
selesai lebih dulu.

**Yang perlu diingat.**
- Setiap node dicetak tepat satu kali.
- Tree kosong tidak mencetak apa pun.
- Mencetak tidak boleh mengubah tree.
- Hati-hati jangan sampai tertukar dengan postorder. Bedanya cuma satu baris
  posisinya, tetapi hasilnya berbeda jauh.

---

### Soal 1c — `postorder` (20 poin)

**Ceritanya.** **Kedua** cabang dikerjakan sampai habis lebih dulu, dan node itu
sendiri dicetak **paling terakhir**. Akibatnya `root` selalu menjadi angka yang
paling belakang.

Sifat ini nanti dipakai lagi di Soal 2b: saat membuang tree, sebuah node baru
boleh dibuang setelah kedua cabangnya selesai dibuang.

**Parameternya.**

| Parameter | Artinya |
|---|---|
| `root` | Alamat node paling atas tree yang mau dicetak. Boleh bernilai `NULL` |
| *kembalian* | Tidak ada (`void`). Hasilnya dicetak ke layar |

**Contoh.** Pada tree contoh menghasilkan `10 30 70 80 90 20 50`.

Ketiganya berdampingan supaya bedanya terlihat:

```
preorder  : 50 80 10 70 30 20 90     <- 50 di depan
inorder   : 10 80 30 70 50 20 90     <- 50 di tengah
postorder : 10 30 70 80 90 20 50     <- 50 di belakang
```

**Yang perlu diingat.**
- Setiap node dicetak tepat satu kali.
- Tree kosong tidak mencetak apa pun.
- Mencetak tidak boleh mengubah tree.
- `root` harus menjadi angka **terakhir** yang dicetak. Kalau angka root Anda
  muncul di tengah, yang Anda tulis sebenarnya inorder.

---

## Soal 2 — Membersihkan Tree (25 poin)

### Soal 2b — `clear` (25 poin)

**Ceritanya.** Seluruh isi tree dibuang sekaligus, sampai tree benar-benar
kosong. Bukan hanya diputus sambungannya — setiap node-nya harus dibuang dari
memori satu per satu dengan `delete`.

Ada satu urutan yang tidak boleh dilanggar: sebuah node baru boleh dibuang
**setelah** kedua cabangnya selesai dibuang. Kalau sebuah node dibuang lebih
dulu, alamat kedua cabangnya ikut hilang bersama node itu — dan cabang-cabang
tadi tertinggal di memori tanpa bisa dicapai lagi oleh siapa pun.

Urutan "cabang dulu, baru node ini" itu persis urutan **postorder** pada Soal 1c.

**Parameternya.**

| Parameter | Artinya |
|---|---|
| `root` | Alamat node paling atas tree milik pemanggil. Bertanda `&`, sehingga tree yang menjadi kosong ikut terasa oleh pemanggil. Boleh bernilai `NULL` |
| *kembalian* | Tidak ada (fungsi bertipe `void`) |

**Contoh.**

| Sebelum | Operasi | Sesudah |
|---|---|---|
| tree contoh berisi 7 node | `clear(root)` | `root` bernilai `NULL`, dan ketujuh node sudah dibuang dari memori |

```cpp
Node* root = NULL;
clear(root);       // aman, tidak terjadi apa-apa

clear(root);       // tree menjadi kosong
clear(root);       // aman, tidak terjadi apa-apa
```

**Yang perlu diingat.**
- Setelah selesai, `root` milik pemanggil harus bernilai `NULL`.
- **Seluruh** node harus dibuang dengan `delete`, masing-masing **tepat satu
  kali**. Banyaknya node yang dibuang ikut dihitung checker.
- Node yang cuma dilepas dari tree tetapi tidak di-`delete` akan tertinggal di
  memori, dan itu ikut dinilai.
- Membuang node yang **sama** dua kali adalah kesalahan berat: program biasanya
  langsung berhenti tidak wajar.
- Hati-hati: alamat kedua cabang harus sudah dipakai **sebelum** sebuah node
  di-`delete`. Node yang sudah di-`delete` tidak boleh dibaca lagi.
- Memanggil `clear` pada tree yang **sudah** kosong adalah sah, dan memanggilnya
  dua kali berturut-turut juga harus aman.
- Fungsi ini tidak mencetak apa pun.

---

## Soal 3 — Soal Cerita: `jumlahDaun` (15 poin)

**Ceritanya.** Sebuah tim kerja disusun berjenjang. Setiap orang membawahi
paling banyak dua orang, dan setiap orang dicatat sebagai satu node dengan nomor
pegawainya sebagai `data`. Orang yang dibawahi seseorang disimpan sebagai cabang
`left` dan `right` miliknya.

Ketua tim ingin tahu satu hal: ada **berapa orang yang tidak membawahi
siapa-siapa**. Merekalah yang bekerja langsung di lapangan, sementara yang lain
sibuk mengurus bawahannya.

Orang yang tidak membawahi siapa pun berarti node yang tidak punya cabang sama
sekali — `left` **dan** `right`-nya sama-sama `NULL`. Node seperti itu disebut
**daun**.

Yang harus Anda putuskan sendiri: bagaimana mengenali sebuah daun, dan bagaimana
menjumlahkan daun yang tersebar di kedua cabang.

**Parameternya.**

| Parameter | Artinya |
|---|---|
| `root` | Alamat node paling atas tim yang mau dihitung. Boleh bernilai `NULL`, artinya timnya sedang kosong |
| *kembalian* | Banyaknya daun di dalam tree |

**Contoh.** Pada tree contoh, `jumlahDaun(root)` bernilai **3**:

```
              50                Yang termasuk daun: 10, 30, dan 90.
             /  \
           80    20             Yang BUKAN daun:
          /  \     \              50  punya dua cabang
        10    70    90            80  punya dua cabang
             /                    20  punya satu cabang (90)
           30                     70  punya satu cabang (30)
```

Perhatikan **20** dan **70** baik-baik. Keduanya cuma punya **satu** cabang, dan
itu tetap membuat mereka **bukan** daun. Daun harus benar-benar tidak punya
cabang sama sekali.

Keadaan lain:

| Tree | `jumlahDaun(root)` | Keterangan |
|---|---|---|
| `(kosong)` | `0` | tidak ada orang sama sekali |
| satu node saja | `1` | dia root sekaligus daun |
| `5` → `4` → `3`, semuanya ke kiri | `1` | hanya node 3; node 5 dan 4 masih punya satu cabang |

**Yang perlu diingat.**
- Tree kosong hasilnya 0.
- Tree berisi satu node hasilnya 1. Node itu root sekaligus daun — kedua hal itu
  tidak saling meniadakan.
- Node yang punya **satu cabang saja bukan daun**. Ini kesalahan yang paling
  sering terjadi pada soal ini.
- Daun bisa berada di kedalaman yang berbeda-beda. Semuanya tetap dihitung.
- Menghitung tidak boleh mengubah tree.
- Fungsi ini tidak mencetak apa pun.

Kondisi lain yang disebut di bagian **Kondisi** dan **Batasan** juga diuji. Hasil
yang seharusnya untuk kondisi-kondisi itu Anda tentukan sendiri.

---

## Batasan

- Hanya keenam fungsi di `src/student.cpp` yang dinilai. `main()` di bagian
  paling bawah file itu bebas Anda ubah untuk latihan dan tidak ikut dinilai.
- `cin` hanya boleh dipakai di dalam `main()` tersebut, tidak di dalam
  fungsi-fungsi yang dinilai.
- Jangan mengubah `struct Node` maupun signature fungsi di `src/student.h`.
- Materi pertemuan ini terbatas pada **binary tree biasa dan traversal-nya**.
  Tidak diperlukan Binary Search Tree, penyisipan, penghapusan node tertentu,
  penyeimbangan, pohon AVL, maupun graf.
- Tidak perlu memakai container pustaka standar (`vector`, `stack`, `queue`, dan
  sejenisnya) sebagai penyimpanan tree — node harus benar-benar dibuat sendiri
  di memori dinamis.
- Anda boleh menambahkan fungsi bantu sendiri sebanyak yang Anda perlukan.

---

## Penilaian Otomatis

Penilaian sepenuhnya berdasarkan **perilaku** program: checker memanggil fungsi
Anda, lalu memeriksa urutan angka yang dicetak, nilai yang dikembalikan, bentuk
tree sesudahnya, dan apakah node yang dibuang benar-benar dibebaskan.

Yang **tidak** pernah diperiksa: nama variabel, gaya penulisan pointer,
formatting, indentasi, komentar, rekursif atau memakai loop, urutan penulisan
fungsi, dan ada tidaknya fungsi bantu. Dua implementasi yang berbeda jauh tetapi
sama-sama memenuhi kontrak akan sama-sama lolos.

Tree yang dipakai untuk pengujian dibangun sendiri oleh checker, bukan lewat
fungsi Anda. Karena itu `preorder` yang belum benar **tidak** ikut menjatuhkan
nilai Soal 2 dan Soal 3.

Total **20 test case**, masing-masing bernilai **5 poin**:

| Soal | Fungsi | Test | Bobot |
|---|---|---:|---:|
| 1a | `preorder` | 4 | 20 |
| 1b | `inorder` | 4 | 20 |
| 1c | `postorder` | 4 | 20 |
| 2b | `clear` | 5 | 25 |
| 3 | `jumlahDaun` | 3 | 15 |
| | **Total** | **20** | **100** |

| Kondisi | Score |
|---|---|
| Gagal compile | 0 |
| Sebagian test lolos | jumlah test lolos × 5 |
| Semua test lolos | 100 |

Setiap fungsi diuji dengan tree kosong, tree satu node, tree contoh pada modul,
tree penuh, tree yang menjulur lurus ke kiri, tree yang menjulur lurus ke kanan,
dan tree berisi angka negatif, nol, serta angka berulang. Mengerjakan sebagian
soal tetap mendapat nilai.

Program yang berhenti tidak wajar di tengah penilaian — hal yang mudah terjadi
saat rekursi lupa titik berhentinya — tidak menghanguskan nilai yang sudah
terkumpul: test yang sudah lolos sebelum berhenti tetap dihitung, sedangkan
sisanya dianggap gagal.

### Membaca Hasil

| Status | Artinya |
|---|---|
| ✅ hijau | Semua test berhasil |
| ❌ merah | Ada test yang gagal, compile error, atau program berhenti tidak wajar |

Klik run tersebut, lalu baca **Summary** — ada tabel nilai dan rincian setiap
test beserta keterangan `Expected` vs `Got`. Contohnya:

```
Expected: modul=[50 80 10 70 30 20 90] penuh=[1 2 4 5 3 6 7]
Got     : modul=[50 80 10 70 30] penuh=[1 2 4 5]
```

Contoh di atas berarti cabang kanan tidak pernah ditelusuri.

| Step yang gagal | Penyebab |
|---|---|
| `Periksa penggunaan cin` | Ada `cin`/`scanf` di dalam fungsi yang dinilai |
| `Compile student.cpp` | Ada syntax/compile error di `student.cpp` |
| `Compile checker` | Nama atau signature fungsi tidak sesuai `student.h`, atau `main()` Anda keluar dari blok `#ifndef ADA_MAIN_LAIN` |
| `Jalankan test & hitung score` | Kode berhasil dikompilasi tetapi perilakunya belum sesuai |

---

## Cara Menjalankan Program Anda

`src/student.cpp` adalah program C++ utuh. Ada `main()` di bagian paling
bawahnya, jadi Anda bisa menjalankannya seperti tugas C++ biasa.

**Lewat VS Code (paling gampang).** Buka `src/student.cpp`, lalu tekan **F5**
(Run and Debug) atau **Ctrl+F5** (jalankan tanpa debug). Repo ini sudah membawa
setelannya, jadi tidak ada yang perlu Anda konfigurasi. Program muncul di
terminal terintegrasi, dan `cin` bisa Anda ketik langsung di situ.

**Lewat terminal.**

```bash
g++ -std=c++17 src/student.cpp -o latihan
./latihan
```

`main()` bawaan membangun tree contoh, lalu memanggil kelima fungsi Anda dan
menampilkan hasilnya berdampingan dengan jawaban yang benar menurut modul —
sehingga Anda bisa langsung membandingkan.

> **Saran cara mengerjakan.** Sebelum menjalankan program, **tulis dulu tebakan
> Anda** untuk ketiga traversal di selembar kertas, baru jalankan dan bandingkan.
> Modul menyebut langkah ini "prediksi sebelum menjalankan program", dan inilah
> cara tercepat memahami bedanya ketiga urutan itu.

`main()` itu **tidak ikut dinilai** dan boleh Anda ubah sebebasnya untuk mencoba
bentuk tree lain, menambah cabang, atau menulis fungsi tampilan buatan Anda
sendiri. Di dalamnya juga sudah ada contoh pemakaian `cin` yang tinggal Anda
hapus tanda komentarnya.

Selama sebuah fungsi belum Anda isi, bagian yang bersangkutan akan tampak kosong
atau bernilai 0 — itu wajar. Kerjakan satu soal, jalankan lagi, lalu perhatikan
bagian mana yang mulai berubah.

> **Penting: `cin` hanya di dalam `main()`.**
> Jangan pernah menaruh `cin` di dalam kelima fungsi yang dinilai. Saat menilai,
> checker memanggil fungsi-fungsi itu tanpa memberi masukan apa pun, sehingga
> `cin` di sana membaca sampah — dan nilai Anda berubah-ubah setiap kali dinilai,
> dari kode yang sama persis. Kelima fungsi itu menerima datanya lewat
> **parameter**, bukan lewat `cin`. Penilaian menolak lebih awal kode yang
> melanggar aturan ini.

> **Kalau program berhenti mendadak** dengan pesan seperti `Segmentation fault`,
> artinya kode Anda menyentuh memori yang bukan haknya. Di pertemuan ini paling
> sering karena rekursi **lupa titik berhentinya**, atau karena membaca node yang
> sudah di-`delete`. Perintah ini menunjukkan baris persisnya:
>
> ```bash
> g++ -std=c++17 -g -fsanitize=address src/student.cpp -o latihan && ./latihan
> ```

**Contoh keluaran ketika seluruh fungsi sudah benar (dipendekkan):**

```
[2] Soal 1 — ketiga traversal
    preorder    : 50 80 10 70 30 20 90
    inorder     : 10 80 30 70 50 20 90
    postorder   : 10 30 70 80 90 20 50

[3] Soal 3 — menghitung jumlah daun
    jumlahDaun  : 3

[4] Tree kosong (root = NULL)
    preorder    :
    inorder     :
    postorder   :
    jumlahDaun  : 0

[6] Tree yang menjulur lurus ke bawah (5 -> 4 -> 3)
    preorder    : 5 4 3
    inorder     : 3 4 5
    postorder   : 3 4 5
    jumlahDaun  : 1

[7] Soal 2b — clear(root)
    root        : NULL
```

Keluaran di atas hanya sebagian kecil dari apa yang diperiksa saat penilaian.
Lolos di sini **belum tentu** 100 — jalankan juga test lengkapnya di bawah.

### Memeriksa Kebocoran Memori (opsional, sangat disarankan)

Karena pertemuan ini memakai `new` dan `delete`, ada satu kesalahan yang tidak
terlihat dari keluaran program: **node yang tidak pernah dilepas**. Compiler
dapat membantu memeriksanya:

```bash
g++ -std=c++17 -fsanitize=address,leak -g src/student.cpp -o latihan_periksa
./latihan_periksa
```

Bila ada node yang bocor, program melaporkannya di akhir dengan keterangan
`LeakSanitizer: detected memory leaks`. Implementasi yang benar tidak
menghasilkan laporan apa pun.

---

## Cara Menjalankan Test di Komputer Sendiri

Butuh `g++` yang mendukung C++17, dan sistem berbasis Linux/macOS (atau WSL di
Windows) karena checker menjalankan setiap test sebagai proses terpisah:

```bash
chmod +x scripts/run_tests.sh
./scripts/run_tests.sh
```

Alurnya sama persis dengan yang dijalankan GitHub Actions, termasuk cara
menghitung score.

Pada starter code yang belum diisi, compiler memunculkan peringatan
*unused parameter*. Itu wajar, hilang setelah parameter benar-benar Anda pakai,
dan **tidak** mengurangi nilai.

---

## Cara Mengumpulkan

Tidak ada tombol "submit". **Push adalah pengumpulan.**

```bash
git clone https://github.com/<ORG>/praktikum-07-<username>.git
cd praktikum-07-<username>
# edit src/student.cpp
git add src/student.cpp
git commit -m "Kerjakan pertemuan 7"
git push
```

Lalu buka tab **Actions** di GitHub, atau lihat nilainya di aplikasi praktikum.

Anda boleh push berkali-kali. **Setiap percobaan tersimpan**, misalnya
45 → 80 → 100. Bergantung pada pengaturan tugas, yang dipakai adalah nilai
terbaik atau nilai terakhir.

---

## Struktur Repository

```
.
├── .github/workflows/test.yml     ← workflow penilaian (jangan diubah)
├── .vscode/                       ← setelan tombol Run (jangan diubah)
├── src/
│   ├── student.h                  ← kontrak/interface (jangan diubah)
│   └── student.cpp                ← KERJAKAN DI SINI ← (main() ada di bawahnya)
├── tests/
│   ├── checker.cpp                ← test instruktur (jangan diubah)
│   └── report.h                   ← penulis result.json (jangan diubah)
├── scripts/
│   ├── run_tests.sh               ← uji lokal
│   ├── periksa_masukan.py         ← menolak cin di dalam fungsi yang dinilai
│   ├── anotasi_gcc.py             ← terjemahan error compiler ke bahasa Indonesia
│   ├── job_summary.py             ← Job Summary dari result.json
│   └── write_error_result.sh      ← result.json saat compile error
└── README.md
```
