// =============================================================================
// student.h — Interface Praktikum
// Pertemuan 7: Binary Tree dan Traversal
// =============================================================================
// INSTRUKSI UNTUK MAHASISWA:
//   - JANGAN mengubah file ini.
//   - Implementasikan seluruh fungsi di dalam src/student.cpp.
//   - Nama struct, nama field, nama fungsi, tipe parameter, dan tipe kembalian
//     adalah KONTRAK: checker memanggilnya langsung, jadi tidak boleh diubah.
//   - Cara Anda memenuhi kontrak sepenuhnya bebas. Penilaian hanya melihat
//     perilaku yang teramati oleh pemanggil.
//
// -----------------------------------------------------------------------------
// APA ITU BINARY TREE
// -----------------------------------------------------------------------------
// Sampai pertemuan kemarin, data selalu tersusun LURUS: satu node menyambung ke
// satu node berikutnya, seperti antrean orang. Pertemuan ini berbeda. Satu node
// boleh menyambung ke DUA node sekaligus, sehingga bentuknya bercabang seperti
// pohon terbalik — akarnya di atas, rantingnya menjulur ke bawah.
//
//               50          <- root, node paling atas
//              ╱  ╲
//            80    20
//           ╱  ╲     ╲
//         10    70    90
//              ╱
//            30
//
// Setiap kotak di gambar itu adalah sebuah `Node`. Isinya tiga hal: sebuah
// angka (`data`), alamat cabang KIRI (`left`), dan alamat cabang KANAN
// (`right`). Cabang yang tidak ada ditandai dengan `NULL`.
//
// -----------------------------------------------------------------------------
// ISTILAH YANG DIPAKAI
// -----------------------------------------------------------------------------
//   root      Node paling atas. Seluruh isi tree bisa dicapai dari sini.
//             `root` bernilai `NULL` berarti tree sedang KOSONG.
//
//   child     Node yang berada tepat di bawah sebuah node. Setiap node punya
//             paling banyak dua: `left` dan `right`.
//
//   parent    Kebalikannya: node yang berada tepat di atas sebuah node.
//
//   leaf      Node yang TIDAK punya child sama sekali — `left` dan `right`
//             kedua-duanya `NULL`. Pada gambar di atas: 10, 30, dan 90.
//             Kata Indonesianya "daun", dipakai pada Soal 3.
//
//   subtree   Bagian tree yang berakar pada suatu node. Yang penting: sebuah
//             subtree bentuknya SAMA PERSIS dengan tree utuh — ia juga punya
//             root, punya cabang kiri, punya cabang kanan. Karena itulah
//             pekerjaan pada tree hampir selalu ditulis secara REKURSIF:
//             kerjakan node ini, lalu minta cabang kiri dan cabang kanan
//             mengerjakan hal yang sama untuk dirinya masing-masing.
//
// CATATAN PENTING: `left` dan `right` di sini hanya menyatakan POSISI, bukan
// aturan besar-kecil. Tree pada pertemuan ini BUKAN Binary Search Tree. Lihat
// saja gambar di atas: 80 berada di kiri 50 walaupun 80 lebih besar. Itu wajar
// dan memang disengaja. Aturan pengurutan baru muncul di pertemuan berikutnya.
//
// -----------------------------------------------------------------------------
// TREE KOSONG ADALAH KEADAAN YANG SAH
// -----------------------------------------------------------------------------
// `root == NULL` bukan kesalahan, melainkan tree yang sedang tidak berisi
// apa-apa. Setiap fungsi di bawah harus menanganinya dengan wajar, tanpa
// membuat program berhenti tidak wajar. Pada penulisan rekursif, keadaan inilah
// yang menjadi titik berhenti — tanpa titik berhenti, rekursi tidak akan pernah
// selesai.
// =============================================================================

#ifndef STUDENT_H
#define STUDENT_H

/**
 * Satu kotak penyimpanan di dalam tree.
 *
 *   `data`    angka yang disimpan node ini
 *   `left`    alamat cabang kiri, atau `NULL` bila tidak ada
 *   `right`   alamat cabang kanan, atau `NULL` bila tidak ada
 */
struct Node {
    int data;
    Node* left;
    Node* right;
};

// =============================================================================
// SOAL 1 — TRAVERSAL
// =============================================================================
// Traversal artinya mengunjungi SETIAP node tree, masing-masing tepat satu
// kali, menurut urutan tertentu. Ketiga fungsi di bawah mengunjungi node yang
// sama persis dan sama banyaknya — yang membedakan hanya KAPAN sebuah node
// dicetak dibandingkan kedua cabangnya.
//
// KETIGANYA MENCETAK, BUKAN MENGEMBALIKAN NILAI.
// Tipe kembaliannya `void`: hasilnya ditulis langsung ke layar dengan `cout`.
//
// BENTUK KELUARAN (berlaku untuk ketiganya):
//   - Setiap angka dicetak apa adanya dan dipisahkan oleh spasi.
//   - Tree kosong tidak mencetak apa pun.
//   - Setiap node dicetak tepat sekali.
//   - Jangan mencetak apa pun selain angka dan spasi — tidak ada judul, tidak
//     ada koma, tidak ada tanda kurung.
//   - Isi tree tidak boleh berubah oleh pemanggilan ini. Memanggil fungsi yang
//     sama dua kali berturut-turut harus mencetak hal yang sama persis.
//
// Spasi berlebih di akhir baris, atau pergantian baris di antara angka, TIDAK
// mengurangi nilai — penilaian hanya melihat urutan angkanya.
//
// Untuk tree pada gambar di atas, ketiganya mencetak:
//
//     preorder   : 50 80 10 70 30 20 90
//     inorder    : 10 80 30 70 50 20 90
//     postorder  : 10 30 70 80 90 20 50
//
// Perhatikan bahwa angka 50 (root) muncul di AWAL pada preorder, di TENGAH pada
// inorder, dan di AKHIR pada postorder. Itulah inti perbedaan ketiganya.
// =============================================================================

/**
 * Traversal PREORDER, dengan urutan kunjungan: Root - Left - Right.
 *
 * Sebuah node dicetak LEBIH DULU, baru sesudah itu kedua cabangnya dikerjakan.
 *
 * Contoh: pada tree gambar di atas mencetak  50 80 10 70 30 20 90
 */
void preorder(const Node* root);

/**
 * Traversal INORDER, dengan urutan kunjungan: Left - Root - Right.
 *
 * Cabang kiri dikerjakan sampai habis lebih dulu, baru node ini dicetak, baru
 * kemudian cabang kanan dikerjakan.
 *
 * Contoh: pada tree gambar di atas mencetak  10 80 30 70 50 20 90
 */
void inorder(const Node* root);

/**
 * Traversal POSTORDER, dengan urutan kunjungan: Left - Right - Root.
 *
 * Kedua cabang dikerjakan sampai habis lebih dulu, dan node ini dicetak paling
 * terakhir.
 *
 * Contoh: pada tree gambar di atas mencetak  10 30 70 80 90 20 50
 */
void postorder(const Node* root);

// =============================================================================
// SOAL 2 — MEMBERSIHKAN TREE
// =============================================================================

/**
 * Seluruh isi tree dibuang sekaligus, sampai tree benar-benar kosong.
 *
 * Bukan hanya diputus sambungannya — setiap node-nya harus dibuang dari memori
 * satu per satu dengan `delete`.
 *
 * Kontrak:
 *   - Setelah selesai, `root` milik pemanggil bernilai `NULL`. Karena itulah
 *     parameternya bertanda `&`: tree yang menjadi kosong ikut terasa oleh
 *     pemanggil.
 *   - SELURUH node harus dibuang dengan `delete`, masing-masing TEPAT SATU
 *     KALI. Node yang cuma dilepas tetapi tidak di-`delete` akan tertinggal di
 *     memori, dan membuang node yang sama dua kali adalah kesalahan berat.
 *     Kedua hal itu ikut diuji.
 *   - Sebuah node hanya boleh dibuang SETELAH kedua cabangnya selesai dibuang.
 *     Node yang sudah di-`delete` tidak boleh dibaca lagi, termasuk `left` dan
 *     `right`-nya.
 *   - Memanggilnya pada tree yang SUDAH kosong adalah sah: tidak terjadi
 *     apa-apa, dan program tidak boleh berhenti tidak wajar.
 *   - Memanggilnya dua kali berturut-turut juga harus aman.
 *   - Fungsi ini tidak mencetak apa pun.
 */
void clear(Node*& root);

// =============================================================================
// SOAL 3 — SOAL CERITA
// =============================================================================

/**
 * Banyaknya DAUN di dalam tree, yaitu node yang tidak punya cabang sama sekali.
 *
 * Sebuah node disebut daun bila `left` DAN `right`-nya sama-sama `NULL`.
 *
 * Kontrak:
 *   - Tree kosong menghasilkan 0.
 *   - Tree berisi satu node menghasilkan 1 — node itu sekaligus root dan daun.
 *   - Node yang punya satu cabang saja BUKAN daun.
 *   - Menghitung tidak boleh mengubah tree. Memanggilnya dua kali berturut-turut
 *     harus memberi angka yang sama persis.
 *   - Fungsi ini tidak mencetak apa pun.
 *
 * Contoh: pada tree gambar di atas menghasilkan 3, yaitu node 10, 30, dan 90.
 */
int jumlahDaun(const Node* root);

#endif // STUDENT_H
