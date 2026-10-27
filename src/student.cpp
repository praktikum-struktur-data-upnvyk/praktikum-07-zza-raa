// =============================================================================
// student.cpp — Implementasi Mahasiswa
// Pertemuan 7: Binary Tree dan Traversal
// =============================================================================
// FILE YANG BOLEH DIEDIT      : src/student.cpp  ← HANYA FILE INI
// FILE YANG TIDAK BOLEH DIEDIT: src/student.h, tests/checker.cpp, tests/report.h
//
// -----------------------------------------------------------------------------
// CARA MEMBACA SOAL DI FILE INI
// -----------------------------------------------------------------------------
// Setiap soal ditulis tepat di atas fungsinya, dengan empat bagian yang sama:
//
//     Ceritanya          — apa yang sedang terjadi, dan apa yang diminta
//     Parameternya       — arti setiap parameter, satu per satu
//     Contoh             — contoh nyata beserta hasil yang benar
//     Yang perlu diingat — hal khusus yang ikut dinilai
//
// Seluruhnya ada 3 soal dan 5 fungsi:
//
//     Soal 1   Traversal      preorder, inorder, postorder    60 poin
//     Soal 2b  Membersihkan   clear                           25 poin
//     Soal 3   Soal cerita    jumlahDaun                      15 poin
//
// Nama parameter dan nama field selalu ditulis di antara tanda petik miring,
// misalnya `root` atau `left`, supaya mudah dibedakan dari kata biasa.
//
// Soal hanya menjelaskan HASIL yang harus tercapai, bukan langkah-langkah
// pengerjaannya. Menentukan caranya adalah bagian dari latihan ini.
// =============================================================================

#include "student.h"

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// =============================================================================
// SOAL 1a — preorder                                                   20 poin
// =============================================================================
// Contoh:
//
//               50
//              ╱  ╲
//            80    20            preorder(root) mencetak:
//           ╱  ╲     ╲
//         10    70    90             50 80 10 70 30 20 90
//              ╱
//            30
//
//   Coba runut pelan-pelan supaya urutannya masuk akal:
//     50 dicetak dulu (dia root), lalu giliran cabang kirinya;
//     80 dicetak, lalu giliran cabang kirinya;
//     10 dicetak, tidak punya cabang, selesai;
//     70 dicetak, lalu 30 (cabang kirinya), cabang kanannya tidak ada;
//     cabang kiri 50 sudah habis, sekarang cabang kanannya;
//     20 dicetak, cabang kirinya tidak ada, lalu 90.
//
//   Tree kosong tidak mencetak apa pun:
//
//     preorder(NULL);    // layar tidak bertambah apa-apa
//
//   Tree berisi satu node saja mencetak satu angka:
//
//     Node satu{42, NULL, NULL};
//     preorder(&satu);      // mencetak:  42
// =============================================================================
void preorder(const Node* root) {
}

// =============================================================================
// SOAL 1b — inorder                                                    20 poin
// =============================================================================
// Contoh:
//
//               50
//              ╱  ╲
//            80    20            inorder(root) mencetak:
//           ╱  ╲     ╲
//         10    70    90             10 80 30 70 50 20 90
//              ╱
//            30
//
//   Bandingkan dengan preorder yang mencetak  50 80 10 70 30 20 90.
//   Angkanya sama, hanya urutannya yang berpindah. Yang paling kentara: 50
//   tadinya di paling depan, sekarang berada di TENGAH — karena seluruh cabang
//   kirinya harus selesai lebih dulu.
//
//     inorder(NULL);     // tidak mencetak apa pun
// =============================================================================
void inorder(const Node* root) {
}

// =============================================================================
// SOAL 1c — postorder                                                  20 poin
// =============================================================================
// Contoh:
//
//               50
//              ╱  ╲
//            80    20            postorder(root) mencetak:
//           ╱  ╲     ╲
//         10    70    90             10 30 70 80 90 20 50
//              ╱
//            30
//
//   Ketiganya berdampingan supaya bedanya terlihat:
//
//     preorder  : 50 80 10 70 30 20 90     <- 50 di depan
//     inorder   : 10 80 30 70 50 20 90     <- 50 di tengah
//     postorder : 10 30 70 80 90 20 50     <- 50 di belakang
//
//     postorder(NULL);   // tidak mencetak apa pun
// =============================================================================
void postorder(const Node* root) {
}

// =============================================================================
// SOAL 2b — clear                                                      25 poin
// =============================================================================
// Contoh:
//
//               50
//              ╱  ╲
//            80    20            clear(root);
//           ╱  ╲     ╲
//         10    70    90         sesudahnya:
//              ╱                   root == NULL
//            30                     ketujuh node sudah dibuang dari memori
//
//   Memanggilnya pada tree yang sudah kosong adalah sah:
//
//     Node* root = NULL;
//     clear(root);              // aman, tidak terjadi apa-apa
//
//   Memanggilnya dua kali berturut-turut juga harus aman:
//
//     clear(root);              // tree menjadi kosong
//     clear(root);              // aman, tidak terjadi apa-apa
// =============================================================================
void clear(Node*& root) {
}

// =============================================================================
// SOAL 3 — jumlahDaun                          SOAL CERITA             15 poin
// =============================================================================
// Contoh:
//
//               50                jumlahDaun(root)  ->  3
//              ╱  ╲
//            80    20             Yang termasuk daun: 10, 30, dan 90.
//           ╱  ╲     ╲
//         10    70    90          Yang BUKAN daun:
//              ╱                    50  punya dua cabang
//            30                      80  punya dua cabang
//                                    20  punya satu cabang (90)
//                                    70  punya satu cabang (30)
//
//   Perhatikan 20 dan 70 baik-baik. Keduanya cuma punya SATU cabang, dan itu
//   tetap membuat mereka BUKAN daun. Daun harus benar-benar tidak punya cabang
//   sama sekali.
//
//   Beberapa keadaan lain:
//
//     Node satu{42, NULL, NULL};
//     jumlahDaun(&satu);        // 1  — dia root sekaligus daun
//     jumlahDaun(NULL);      // 0  — tidak ada orang sama sekali
//
//   Tim yang menjulur lurus ke bawah hanya punya satu daun, yaitu yang paling
//   bawah:
//
//         5
//        ╱
//       4                        jumlahDaun(root)  ->  1
//      ╱                         (hanya node 3; node 5 dan 4 masing-masing
//     3                           masih punya satu cabang)
// =============================================================================
int jumlahDaun(const Node* root) {
    return 0;
}

// =============================================================================
// MAIN() — program Anda sendiri. TIDAK dinilai, bebas diubah.
// =============================================================================
// Di bawah ini file ini menjadi program C++ biasa. Tekan Run di VS Code, atau
// jalankan lewat terminal:
//
//     g++ -std=c++17 src/student.cpp -o latihan
//     ./latihan
//
// Isinya boleh Anda ganti total: tambah cin, ubah bentuk tree-nya, buat tree
// yang menjulur lurus ke bawah, tambah cabang baru, tulis fungsi tampilan
// buatan Anda sendiri. Tidak ada satu pun di bawah sini yang mempengaruhi nilai
// Anda.
//
// main() ini sengaja hanya MEMANGGIL fungsi-fungsi Anda, lalu menampilkan apa
// yang dilaporkan fungsi-fungsi itu. Selama sebuah fungsi belum Anda isi,
// bagian yang bersangkutan akan tampak kosong atau bernilai 0 — itu wajar.
// Kerjakan satu soal, jalankan lagi, lalu perhatikan bagian mana yang mulai
// berubah.
//
// SATU ATURAN YANG TIDAK BOLEH DILANGGAR
// --------------------------------------
// cin hanya boleh dipakai DI DALAM main() ini. JANGAN menaruh cin di dalam
// keenam fungsi di atas. Saat menilai, checker memanggil fungsi-fungsi itu
// tanpa memberi masukan apa pun, jadi cin di sana akan membaca sampah — dan
// nilai Anda berubah-ubah setiap kali dinilai, dari kode yang sama persis.
//
// Keenam fungsi di atas menerima datanya lewat PARAMETER. Itulah gunanya
// parameter: satu fungsi yang sama bisa dipakai berkali-kali dengan tree yang
// berbeda-beda.
// =============================================================================

#ifndef ADA_MAIN_LAIN

// -----------------------------------------------------------------------------
// Pembantu di bawah ini hanya membangun tree contoh dan merapikan tampilan.
// Boleh Anda ubah atau hapus.
// -----------------------------------------------------------------------------

// Membangun tree contoh yang dipakai di seluruh soal:
//
//               50
//              ╱  ╲
//            80    20
//           ╱  ╲     ╲
//         10    70    90
//              ╱
//            30
//
// Sengaja dibangun manual dengan `new`, persis seperti pada modul. Perhatikan
// bahwa fungsi-fungsi Anda TIDAK dipakai di sini — tree-nya dibentuk sendiri,
// supaya program ini tetap bisa dijalankan walaupun soal belum dikerjakan.
static Node* buatTreeContoh() {
    Node* root              = new Node{50, NULL, NULL};
    root->left              = new Node{80, NULL, NULL};
    root->right             = new Node{20, NULL, NULL};
    root->left->left        = new Node{10, NULL, NULL};
    root->left->right       = new Node{70, NULL, NULL};
    root->right->right      = new Node{90, NULL, NULL};
    root->left->right->left = new Node{30, NULL, NULL};
    return root;
}

// Membangun tree yang menjulur lurus ke bawah:  5 -> 4 -> 3 (semua ke kiri).
static Node* buatTreeMiring() {
    Node* root       = new Node{5, NULL, NULL};
    root->left       = new Node{4, NULL, NULL};
    root->left->left = new Node{3, NULL, NULL};
    return root;
}

// Menulis satu baris keterangan dengan lebar label yang seragam.
static ostream& baris(const string& label) {
    return cout << "    " << left << setw(12) << label << ": ";
}

// Menjalankan ketiga traversal pada satu tree, berurutan supaya mudah
// dibandingkan.
static void tampilkanTraversal(const Node* root) {
    baris("preorder");   preorder(root);   cout << "\n";
    baris("inorder");    inorder(root);    cout << "\n";
    baris("postorder");  postorder(root);  cout << "\n";
}

// Menampilkan hitungan daun pada satu tree.
static void tampilkanHitungan(const Node* root) {
    baris("jumlahDaun") << jumlahDaun(root) << "\n";
}

int main() {
    cout << "==========================================\n";
    cout << " Latihan — Binary Tree dan Traversal\n";
    cout << " (bagian ini tidak ikut dinilai)\n";
    cout << "==========================================\n";

    // -------------------------------------------------------------------------
    // Tree contoh, sama dengan yang dipakai di seluruh soal.
    // -------------------------------------------------------------------------
    Node* root = buatTreeContoh();

    cout << "\n[1] Tree contoh\n";
    cout << "\n";
    cout << "              50\n";
    cout << "             /  \\\n";
    cout << "           80    20\n";
    cout << "          /  \\     \\\n";
    cout << "        10    70    90\n";
    cout << "             /\n";
    cout << "           30\n";
    cout << "\n";

    // -------------------------------------------------------------------------
    // Soal 1 — ketiga traversal. Tulis dulu tebakan Anda sebelum melihat ini.
    // -------------------------------------------------------------------------
    cout << "[2] Soal 1 — ketiga traversal\n";
    tampilkanTraversal(root);
    cout << "\n";
    cout << "    Jawaban yang benar menurut modul:\n";
    cout << "      preorder    : 50 80 10 70 30 20 90\n";
    cout << "      inorder     : 10 80 30 70 50 20 90\n";
    cout << "      postorder   : 10 30 70 80 90 20 50\n";

    // -------------------------------------------------------------------------
    // Soal 3 — menghitung jumlah daun.
    // -------------------------------------------------------------------------
    cout << "\n[3] Soal 3 — menghitung jumlah daun\n";
    tampilkanHitungan(root);
    cout << "\n";
    cout << "    Jawaban yang benar: jumlahDaun = 3\n";

    // -------------------------------------------------------------------------
    // Keadaan batas: tree kosong dan tree berisi satu node.
    // -------------------------------------------------------------------------
    cout << "\n[4] Tree kosong (root = NULL)\n";
    Node* kosong = NULL;
    tampilkanTraversal(kosong);
    tampilkanHitungan(kosong);
    cout << "\n    Yang benar: ketiga traversal tidak mencetak apa pun,\n";
    cout << "                jumlahDaun = 0\n";

    cout << "\n[5] Tree berisi satu node (42)\n";
    Node* satu = new Node{42, NULL, NULL};
    tampilkanTraversal(satu);
    tampilkanHitungan(satu);
    cout << "\n    Yang benar: ketiganya mencetak 42,\n";
    cout << "                jumlahDaun = 1\n";

    cout << "\n[6] Tree yang menjulur lurus ke bawah (5 -> 4 -> 3)\n";
    Node* miring = buatTreeMiring();
    tampilkanTraversal(miring);
    tampilkanHitungan(miring);
    cout << "\n    Yang benar: preorder 5 4 3, inorder 3 4 5, postorder 3 4 5,\n";
    cout << "                jumlahDaun = 1\n";

    // -------------------------------------------------------------------------
    // Soal 2b — membuang seluruh isi tree.
    // -------------------------------------------------------------------------
    cout << "\n[7] Soal 2b — clear(root)\n";
    clear(root);
    baris("root") << (root == NULL ? "NULL" : "masih terisi") << "\n";
    cout << "\n    Yang benar: root = NULL\n";

    // -------------------------------------------------------------------------
    // Mau mencoba dengan angka yang Anda ketik sendiri? Hapus tanda // di
    // bawah ini, lalu jalankan lagi.
    // -------------------------------------------------------------------------
    // int angka;
    // cout << "\nKetik satu angka untuk dijadikan root: ";
    // cin >> angka;
    // Node* punyaAnda = new Node{angka, NULL, NULL};
    // punyaAnda->left = new Node{angka * 2, NULL, NULL};
    // cout << "preorder: ";
    // preorder(punyaAnda);
    // cout << "\njumlahDaun = " << jumlahDaun(punyaAnda) << endl;
    // clear(punyaAnda);

    cout << "\n==========================================\n";
    cout << " Selesai. Silakan ubah bagian ini untuk\n";
    cout << " mencoba percobaan Anda sendiri.\n";
    cout << "==========================================\n";

    // Seluruh tree dikosongkan sebelum program berakhir. Bila clear() sudah
    // benar, pemanggilan ini membebaskan sisanya.
    clear(satu);
    clear(miring);
    clear(root);
    return 0;
}
#endif
