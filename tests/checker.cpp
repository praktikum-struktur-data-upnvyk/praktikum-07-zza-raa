// =============================================================================
// checker.cpp — Test Harness Instruktur
// Pertemuan 7: Binary Tree dan Traversal
// =============================================================================
// File ini adalah milik instruktur dan TIDAK boleh diubah mahasiswa.
//
// PRINSIP PENILAIAN (behavior-based):
//   Checker hanya memanggil fungsi mahasiswa lalu memeriksa akibatnya: urutan
//   angka yang dicetak, nilai yang dikembalikan, bentuk tree sesudah
//   pemanggilan, dan apakah node yang dibuang benar-benar dibebaskan. Nama
//   variabel, gaya penulisan pointer, jenis loop, rekursif atau iteratif,
//   urutan kode, komentar, dan formatting tidak pernah diperiksa. Implementasi
//   apa pun yang memenuhi kontrak di student.h akan PASS.
//
// BOBOT:
//   20 test case, bobot rata 100 / 20 = 5 poin per test (lihat report.h).
//   Bobot per soal diatur lewat BANYAKNYA test pada soal tersebut:
//
//     Soal 1   Traversal                                           60 poin
//       1a preorder    4 test = 20
//       1b inorder     4 test = 20
//       1c postorder   4 test = 20
//     Soal 2b  Membersihkan                                        25 poin
//       2b clear       5 test = 25
//     Soal 3   Soal cerita                                         15 poin
//       3  jumlahDaun  3 test = 15
//
// SOAL SALING BEBAS:
//   Tree untuk pengujian dibangun oleh checker sendiri (lihat buatNode dan
//   berkas pembangun tree di bawahnya), bukan lewat fungsi mahasiswa. Dengan
//   begitu preorder yang salah tidak ikut menjatuhkan nilai Soal 2 dan Soal 3.
//
// MENANGKAP KELUARAN TRAVERSAL:
//   Ketiga fungsi traversal bertipe void dan MENCETAK hasilnya, persis seperti
//   pada modul. Karena itu proses anak mengalihkan seluruh keluaran standar ke
//   sebuah berkas sementara selama fungsi mahasiswa berjalan, lalu membacanya
//   kembali. Pengalihan dilakukan pada tingkat file descriptor, bukan sekadar
//   cout.rdbuf(), supaya mahasiswa yang memakai printf() tidak dirugikan.
//
//   Teks yang tertangkap kemudian DIRAPIKAN: dipecah pada setiap spasi,
//   tab, atau pergantian baris, lalu disambung ulang dengan tepat satu spasi.
//   Akibatnya spasi menggantung di akhir baris, spasi ganda, dan endl di antara
//   angka tidak mengurangi nilai. Yang dinilai murni URUTAN ANGKANYA.
//
// ISOLASI PROSES:
//   Rekursi tanpa titik berhenti adalah kesalahan paling umum pada tree, dan
//   akibatnya program habis tumpukan lalu berhenti tidak wajar. Bila fungsi
//   seperti itu dipanggil langsung, checker ikut mati dan seluruh nilai hilang.
//
//   Karena itu setiap test dijalankan di dalam PROSES ANAK hasil fork() dengan
//   batas waktu, batas memori, dan batas ukuran berkas. Anak membangun tree-nya
//   sendiri, memanggil fungsi mahasiswa, lalu menuliskan RINGKASAN HASILNYA
//   sebagai teks; induk hanya membandingkan teks itu dengan teks yang
//   diharapkan. Akibatnya:
//     - rekursi tanpa titik berhenti  -> terdeteksi sebagai FAIL biasa
//     - membuang node yang sama dua kali -> terdeteksi sebagai FAIL biasa
//     - keluaran yang tidak pernah berhenti -> terdeteksi sebagai WAKTU HABIS
//     - ketiga-tiganya TIDAK menghentikan test-test berikutnya
//
//   Isolasi ini murni soal ketahanan checker. Yang dinilai tetap perilaku
//   fungsi mahasiswa, dan mekanisme grading global (report.h, scripts/,
//   workflow, skema result.json) tidak diubah sama sekali.
//
// PEMERIKSAAN PEMBEBASAN MEMORI:
//   Checker mengganti operator new/delete global agar dapat menghitung berapa
//   blok memori dinamis yang masih hidup. Angka itu hanya dibaca sebagai
//   SELISIH pada potongan kode yang sangat pendek (persis sebelum dan sesudah
//   satu pemanggilan clear), sehingga alokasi milik checker sendiri tidak ikut
//   terhitung. Ini tetap pengujian perilaku: yang diuji adalah "node yang
//   dibuang benar-benar dibebaskan", bukan cara penulisan kodenya.
//
// KETAHANAN TERHADAP CRASH:
//   Checker menyimpan snapshot result.json setiap kali satu test selesai,
//   dengan test yang belum sempat berjalan dicatat sebagai FAIL. Nilai parsial
//   yang sudah diperoleh tetap tercatat dan tidak berubah menjadi 0.
//
// CATATAN KEAMANAN:
//   Mahasiswa dapat membaca file ini. Mitigasi:
//   - Setiap kontrak diuji dengan beberapa bentuk tree (kosong, satu node,
//     miring kiri, miring kanan, penuh, dan tree berisi angka berulang serta
//     negatif)
//   - Repository mahasiswa bersifat privat
// =============================================================================

#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include "report.h"
#include "../src/student.h"

using namespace std;

// =============================================================================
// Penghitung blok memori dinamis yang masih hidup
// =============================================================================
// Mengganti operator new/delete global adalah cara yang sah dalam C++ dan
// berlaku untuk seluruh program, termasuk `new Node` di dalam student.cpp.

static long long g_blokHidup = 0;

void* operator new(size_t ukuran) {
    if (ukuran == 0) ukuran = 1;
    void* blok = malloc(ukuran);
    if (blok == NULL) throw bad_alloc();
    ++g_blokHidup;
    return blok;
}

void* operator new[](size_t ukuran) {
    return ::operator new(ukuran);
}

void operator delete(void* blok) noexcept {
    if (blok != NULL) {
        --g_blokHidup;
        free(blok);
    }
}

void operator delete[](void* blok) noexcept {
    ::operator delete(blok);
}

void operator delete(void* blok, size_t) noexcept {
    ::operator delete(blok);
}

void operator delete[](void* blok, size_t) noexcept {
    ::operator delete(blok);
}

static long long blokHidup() {
    return g_blokHidup;
}

// =============================================================================
// Test Framework (sederhana, tanpa dependency eksternal)
// =============================================================================

// ANSI color codes untuk output terminal yang jelas
#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN  "\033[36m"
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD  "\033[1m"

static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Banyaknya test yang direncanakan pada pertemuan ini. Dipakai untuk menghitung
// skor snapshot supaya test yang belum berjalan tetap terhitung sebagai gagal.
static const int TOTAL_TEST_DIRENCANAKAN = 20;

// Menulis result.json versi sementara setelah setiap test selesai.
// Test yang belum dijalankan ditambahkan sebagai FAIL, lalu dilepas kembali,
// sehingga isi rekaman sebenarnya tidak terpengaruh.
static void simpanHasilSementara() {
    vector<TestRecord>& rekaman = test_records();
    const size_t jumlahAsli = rekaman.size();

    for (size_t i = jumlahAsli;
         i < static_cast<size_t>(TOTAL_TEST_DIRENCANAKAN); ++i) {
        rekaman.push_back(TestRecord{
            "(test #" + to_string(i + 1) + " belum dijalankan)",
            "FAIL",
            "Checker berhenti sebelum test ini sempat dijalankan."});
    }

    const int skor = passed_tests * 100 / TOTAL_TEST_DIRENCANAKAN;
    write_result_json("result.json", skor);

    rekaman.resize(jumlahAsli);
}

// Mencatat satu hasil test. Dipakai oleh makro UJI di bawah.
static void catatHasil(const string& nama, bool lulus,
                       const string& keterangan) {
    total_tests++;
    if (lulus) {
        passed_tests++;
        cout << COLOR_GREEN << "  [PASS]" << COLOR_RESET << " " << nama
             << endl;
    } else {
        failed_tests++;
        cout << COLOR_RED << "  [FAIL]" << COLOR_RESET << " " << nama
             << endl;
        cout << "         Keterangan: " << keterangan << endl;
    }
    record_test(nama, lulus, lulus ? "" : keterangan);
    simpanHasilSementara();
}

// =============================================================================
// Menjalankan satu test di dalam proses anak dengan batas waktu
// =============================================================================

// Batas waktu satu test. Implementasi yang benar selesai dalam hitungan
// milidetik; batas ini semata-mata menangkap keluaran yang tidak berhenti.
static const int BATAS_DETIK = 5;

// Batas pemakaian memori proses anak (512 MB). Rekursi tanpa titik berhenti
// biasanya sudah lebih dulu kehabisan tumpukan, tetapi batas ini menjaga
// implementasi yang menumpuk data di memori.
static const rlim_t BATAS_MEMORI = static_cast<rlim_t>(512) * 1024 * 1024;

// Batas ukuran berkas yang boleh ditulis proses anak (8 MB). Melindungi runner
// dari fungsi traversal yang mencetak tanpa henti ke berkas penampung.
static const rlim_t BATAS_BERKAS = static_cast<rlim_t>(8) * 1024 * 1024;

// Batas panjang teks hasil, jauh di bawah kapasitas pipe (64 KB) sehingga anak
// tidak pernah terhalang saat menulis.
static const size_t BATAS_KELUARAN = 8000;

enum KeadaanAnak { ANAK_SELESAI, ANAK_WAKTU_HABIS, ANAK_BERHENTI };

struct HasilAnak {
    KeadaanAnak keadaan;
    string teks;
    int penyebab;   // nomor sinyal atau kode keluar, sesuai keadaan
};

typedef string (*FungsiUji)();

static HasilAnak jalankanTerisolasi(FungsiUji uji) {
    HasilAnak hasil;
    hasil.keadaan = ANAK_BERHENTI;
    hasil.penyebab = 0;

    int pipa[2];
    if (pipe(pipa) != 0) {
        hasil.teks = "(checker gagal menyiapkan pipe)";
        return hasil;
    }

    cout.flush();
    cerr.flush();
    fflush(stdout);

    pid_t anak = fork();
    if (anak < 0) {
        close(pipa[0]);
        close(pipa[1]);
        hasil.teks = "(checker gagal membuat proses anak)";
        return hasil;
    }

    if (anak == 0) {
        // ---------------------------------------------------------------
        // Proses anak: di sinilah fungsi mahasiswa benar-benar dipanggil.
        // ---------------------------------------------------------------
        close(pipa[0]);

        struct rlimit batas;
        batas.rlim_cur = BATAS_MEMORI;
        batas.rlim_max = BATAS_MEMORI;
        setrlimit(RLIMIT_AS, &batas);

        struct rlimit batasBerkas;
        batasBerkas.rlim_cur = BATAS_BERKAS;
        batasBerkas.rlim_max = BATAS_BERKAS;
        setrlimit(RLIMIT_FSIZE, &batasBerkas);

        string keluaran;
        try {
            keluaran = uji();
        } catch (const exception& e) {
            keluaran = string("(program melempar exception: ") + e.what() + ")";
        } catch (...) {
            keluaran = "(program melempar exception)";
        }
        if (keluaran.size() > BATAS_KELUARAN) {
            keluaran.resize(BATAS_KELUARAN);
            keluaran += "...(dipotong)";
        }

        const char* data = keluaran.c_str();
        size_t sisa = keluaran.size();
        while (sisa > 0) {
            ssize_t ditulis = write(pipa[1], data, sisa);
            if (ditulis <= 0) break;
            data += ditulis;
            sisa -= static_cast<size_t>(ditulis);
        }
        close(pipa[1]);
        _exit(0);
    }

    // -------------------------------------------------------------------
    // Proses induk: menunggu anak, dengan batas waktu.
    // -------------------------------------------------------------------
    close(pipa[1]);

    int status = 0;
    bool berakhir = false;
    for (int i = 0; i < BATAS_DETIK * 100; ++i) {
        pid_t hasilTunggu = waitpid(anak, &status, WNOHANG);
        if (hasilTunggu == anak) { berakhir = true; break; }
        if (hasilTunggu < 0) { berakhir = true; break; }
        struct timespec jeda;
        jeda.tv_sec = 0;
        jeda.tv_nsec = 10L * 1000L * 1000L;   // 10 ms
        nanosleep(&jeda, NULL);
    }

    if (!berakhir) {
        kill(anak, SIGKILL);
        waitpid(anak, &status, 0);
    }

    string teks;
    char penyangga[4096];
    ssize_t dibaca;
    while ((dibaca = read(pipa[0], penyangga, sizeof(penyangga))) > 0) {
        teks.append(penyangga, static_cast<size_t>(dibaca));
    }
    close(pipa[0]);

    if (!berakhir) {
        hasil.keadaan = ANAK_WAKTU_HABIS;
        hasil.penyebab = BATAS_DETIK;
    } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        hasil.keadaan = ANAK_SELESAI;
    } else if (WIFSIGNALED(status)) {
        hasil.keadaan = ANAK_BERHENTI;
        hasil.penyebab = WTERMSIG(status);
    } else {
        hasil.keadaan = ANAK_BERHENTI;
        hasil.penyebab = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
    hasil.teks = teks;
    return hasil;
}

// Menerjemahkan hasil proses anak menjadi teks "Got" yang dibaca manusia.
static string bacaHasil(const HasilAnak& hasil) {
    switch (hasil.keadaan) {
        case ANAK_SELESAI:
            return hasil.teks;
        case ANAK_WAKTU_HABIS: {
            ostringstream out;
            out << "(operasi tidak pernah berhenti — dihentikan setelah "
                << hasil.penyebab << " detik; kemungkinan rekursi yang tidak "
                   "punya titik berhenti)";
            return out.str();
        }
        default: {
            ostringstream out;
            out << "(program berhenti tidak wajar";
            if (hasil.penyebab == SIGSEGV) {
                out << ": mengakses memori yang tidak sah (SIGSEGV), mis. "
                       "rekursi tanpa titik berhenti sehingga tumpukan habis, "
                       "atau membaca node yang sudah di-delete";
            } else if (hasil.penyebab == SIGABRT) {
                out << ": program dihentikan paksa (SIGABRT), mis. membuang "
                       "node yang sama dua kali";
            } else if (hasil.penyebab == SIGXFSZ) {
                out << ": mencetak tanpa henti sampai melewati batas ukuran "
                       "berkas";
            } else if (hasil.penyebab != 0) {
                out << ": sinyal/kode " << hasil.penyebab;
            }
            out << ")";
            if (!hasil.teks.empty()) out << " keluaran sebagian: " << hasil.teks;
            return out.str();
        }
    }
}

// Makro utama: jalankan `fungsi` secara terisolasi, bandingkan teks hasilnya
// dengan `harapan`.
#define UJI(nama, fungsi, harapan) do { \
    HasilAnak _h = jalankanTerisolasi(fungsi); \
    string _dapat = bacaHasil(_h); \
    string _harap = (harapan); \
    bool _ok = (_h.keadaan == ANAK_SELESAI) && (_dapat == _harap); \
    ostringstream _pesan; \
    _pesan << "Expected: " << _harap << " | Got: " << _dapat; \
    catatHasil((nama), _ok, _pesan.str()); \
} while (0)

// =============================================================================
// Utilitas tree milik checker (dipakai di dalam proses anak)
// =============================================================================

static const char* bo(bool nilai) { return nilai ? "true" : "false"; }

// Membuat satu node tanpa memakai fungsi mahasiswa.
static Node* buatNode(int data, Node* kiri = NULL, Node* kanan = NULL) {
    Node* baru = new Node;
    baru->data = data;
    baru->left = kiri;
    baru->right = kanan;
    return baru;
}

// --- Tree uji ----------------------------------------------------------------
// Seluruhnya dibangun sendiri oleh checker, supaya setiap soal dapat dinilai
// walaupun soal yang lain belum benar.

// Tree utama pada modul dan pada seluruh contoh soal:
//
//           50
//          ╱  ╲
//        80    20
//       ╱  ╲     ╲
//     10    70    90
//          ╱
//        30
//
//   preorder  : 50 80 10 70 30 20 90
//   inorder   : 10 80 30 70 50 20 90
//   postorder : 10 30 70 80 90 20 50
//   jumlahDaun: 3   (10, 30, 90)
static Node* treeModul() {
    return buatNode(50,
                    buatNode(80,
                             buatNode(10),
                             buatNode(70, buatNode(30), NULL)),
                    buatNode(20, NULL, buatNode(90)));
}

// Tree penuh, setiap node dalamnya punya dua cabang:
//
//         1
//       ╱   ╲
//      2     3
//     ╱ ╲   ╱ ╲
//    4   5 6   7
//
//   preorder  : 1 2 4 5 3 6 7
//   inorder   : 4 2 5 1 6 3 7
//   postorder : 4 5 2 6 7 3 1
//   jumlahDaun: 4   (4, 5, 6, 7)
static Node* treePenuh() {
    return buatNode(1,
                    buatNode(2, buatNode(4), buatNode(5)),
                    buatNode(3, buatNode(6), buatNode(7)));
}

// Tree berisi satu node. Node itu root sekaligus daun.
static Node* treeSatu() {
    return buatNode(42);
}

// Tree yang menjulur lurus ke KIRI: 5 -> 4 -> 3 -> 2 -> 1
//
//   preorder  : 5 4 3 2 1
//   inorder   : 1 2 3 4 5
//   postorder : 1 2 3 4 5
//   jumlahDaun: 1
static Node* treeMiringKiri() {
    return buatNode(5, buatNode(4, buatNode(3, buatNode(2, buatNode(1)))));
}

// Tree yang menjulur lurus ke KANAN: 1 -> 2 -> 3 -> 4
//
//   preorder  : 1 2 3 4
//   inorder   : 1 2 3 4
//   postorder : 4 3 2 1
//   jumlahDaun: 1
static Node* treeMiringKanan() {
    return buatNode(1, NULL,
                    buatNode(2, NULL,
                             buatNode(3, NULL, buatNode(4))));
}

// Tree berisi angka NEGATIF, NOL, dan BERULANG, dengan cabang tunggal di dua
// tempat:
//
//          0
//        ╱   ╲
//      -7     7
//        ╲   ╱
//         7 0
//
//   preorder  : 0 -7 7 7 0
//   inorder   : -7 7 0 0 7
//   postorder : 7 -7 0 7 0
//   jumlahDaun: 2   (7 di bawah -7, dan 0 di bawah 7)
static Node* treeAneh() {
    return buatNode(0,
                    buatNode(-7, NULL, buatNode(7)),
                    buatNode(7, buatNode(0), NULL));
}

// Node 1 HANYA punya cabang kiri, dan cabang itu sendiri bercabang dua:
//
//        1
//       ╱
//      2
//     ╱ ╲
//    3   4
//
// Kedalaman ini disengaja. Pada tree "satu cabang" yang cuma dua tingkat,
// implementasi yang keliru menuliskan `left == NULL ATAU right == NULL`
// kebetulan memberi angka yang sama dengan implementasi yang benar. Dengan
// bentuk di atas, keliru itu menghasilkan 1 sedangkan jawaban benarnya 2.
//
//   jumlahDaun: 2   (3 dan 4)
static Node* treeSatuCabangKiri() {
    return buatNode(1, buatNode(2, buatNode(3), buatNode(4)), NULL);
}

// Cerminan dari treeSatuCabangKiri, cabang tunggalnya di sebelah kanan:
//
//    1
//     ╲
//      2
//     ╱ ╲
//    3   4
//
//   jumlahDaun: 2   (3 dan 4)
static Node* treeSatuCabangKanan() {
    return buatNode(1, NULL, buatNode(2, buatNode(3), buatNode(4)));
}

// --- Pembacaan tree oleh checker ---------------------------------------------

// Batas banyaknya node yang ditelusuri checker saat MENGGAMBARKAN sebuah tree.
// Tree yang benar pada pertemuan ini tidak pernah melebihi belasan node, jadi
// batas ini hanya berlaku untuk tree yang rusak atau melingkar.
static const int BATAS_NODE = 400;

static void tulisBentuk(const Node* n, ostringstream& out, int& sisa) {
    if (sisa <= 0) { out << "..."; return; }
    if (n == NULL) { out << "."; return; }
    --sisa;
    out << n->data << "(";
    tulisBentuk(n->left, out, sisa);
    out << ",";
    tulisBentuk(n->right, out, sisa);
    out << ")";
}

// Gambaran bentuk tree sebagai teks, dipakai untuk memastikan sebuah fungsi
// tidak mengubah tree. Bentuknya: data(kiri,kanan), dengan "." untuk NULL.
static string bentukTree(const Node* root) {
    ostringstream out;
    int sisa = BATAS_NODE;
    tulisBentuk(root, out, sisa);
    return out.str();
}

// Banyaknya node menurut checker sendiri, tanpa memakai fungsi mahasiswa.
static int hitungSendiri(const Node* n, int sisa = BATAS_NODE) {
    if (n == NULL || sisa <= 0) return 0;
    return 1 + hitungSendiri(n->left, sisa - 1)
             + hitungSendiri(n->right, sisa - 1);
}

// Membuang tree milik checker sendiri, tanpa memakai clear() mahasiswa.
static void buangSendiri(Node* n) {
    if (n == NULL) return;
    buangSendiri(n->left);
    buangSendiri(n->right);
    delete n;
}

// --- Menangkap keluaran fungsi traversal -------------------------------------

// Memecah teks pada setiap spasi/tab/baris baru, lalu menyambungnya kembali
// dengan tepat satu spasi. Inilah yang membuat spasi menggantung, spasi ganda,
// dan endl di antara angka tidak mengurangi nilai.
static string rapikan(const string& mentah) {
    istringstream in(mentah);
    ostringstream out;
    string kata;
    bool pertama = true;
    while (in >> kata) {
        if (!pertama) out << " ";
        out << kata;
        pertama = false;
    }
    return out.str();
}

typedef void (*FungsiTraversal)(const Node*);

// Menjalankan satu fungsi traversal dan mengembalikan angka-angka yang
// dicetaknya, sudah dirapikan.
//
// Pengalihan dilakukan pada file descriptor 1, bukan cout.rdbuf(), supaya
// keluaran lewat printf() ikut tertangkap. Hanya dipanggil di dalam proses
// anak, jadi pengalihan ini tidak pernah mengganggu keluaran checker sendiri.
static string tangkapKeluaran(FungsiTraversal fungsi, const Node* root) {
    char nama[] = "/tmp/checker_p07_XXXXXX";
    int berkas = mkstemp(nama);
    if (berkas < 0) return "(checker gagal menyiapkan penampung keluaran)";
    unlink(nama);   // berkasnya hilang sendiri begitu ditutup

    cout.flush();
    fflush(stdout);

    int stdoutLama = dup(STDOUT_FILENO);
    if (stdoutLama < 0) { close(berkas); return "(checker gagal menggandakan stdout)"; }
    dup2(berkas, STDOUT_FILENO);

    fungsi(root);

    cout.flush();
    fflush(stdout);
    dup2(stdoutLama, STDOUT_FILENO);
    close(stdoutLama);

    lseek(berkas, 0, SEEK_SET);
    string mentah;
    char penyangga[4096];
    ssize_t dibaca;
    while ((dibaca = read(berkas, penyangga, sizeof(penyangga))) > 0) {
        mentah.append(penyangga, static_cast<size_t>(dibaca));
        if (mentah.size() > BATAS_KELUARAN) break;
    }
    close(berkas);

    return rapikan(mentah);
}

// Pola yang dipakai berulang: jalankan satu traversal pada beberapa tree,
// masing-masing diberi label.
static string traversalPada(FungsiTraversal fungsi, const string& label,
                            Node* root) {
    ostringstream out;
    out << label << "=[" << tangkapKeluaran(fungsi, root) << "]";
    buangSendiri(root);
    return out.str();
}

// Pola pemeriksaan "tidak mengubah tree": jalankan traversal dua kali pada tree
// yang sama, lalu bandingkan hasilnya dan bentuk tree sebelum/sesudah.
static string traversalTidakMerusak(FungsiTraversal fungsi) {
    Node* t = treeAneh();
    string sebelum = bentukTree(t);
    string pertama = tangkapKeluaran(fungsi, t);
    string kedua = tangkapKeluaran(fungsi, t);
    string sesudah = bentukTree(t);

    ostringstream out;
    out << "aneh=[" << pertama << "]"
        << " samaDuaKali=" << bo(pertama == kedua)
        << " treeUtuh=" << bo(sebelum == sesudah);
    buangSendiri(t);
    return out.str();
}

// =============================================================================
// SOAL 1a — preorder  (4 test = 20 poin)
// =============================================================================

// Tree utama pada modul.
static string uji01() {
    return traversalPada(preorder, "modul", treeModul());
}

// Tree penuh supaya kedua cabang benar-benar terpakai di setiap tingkat.
static string uji02() {
    return traversalPada(preorder, "penuh", treePenuh());
}

// Keadaan batas: tree kosong, tree satu node, dan tree yang menjulur lurus ke
// satu arah. Tree miring menangkap implementasi yang lupa menelusuri salah satu
// cabang.
static string uji03() {
    ostringstream out;
    out << "kosong=[" << tangkapKeluaran(preorder, NULL) << "]";
    out << " " << traversalPada(preorder, "satu", treeSatu());
    out << " " << traversalPada(preorder, "miringKiri", treeMiringKiri());
    out << " " << traversalPada(preorder, "miringKanan", treeMiringKanan());
    return out.str();
}

// Angka negatif, nol, dan berulang; sekaligus memastikan mencetak tidak
// mengubah tree dan hasilnya tetap sama saat dipanggil dua kali.
static string uji04() {
    return traversalTidakMerusak(preorder);
}

// =============================================================================
// SOAL 1b — inorder  (4 test = 20 poin)
// =============================================================================

static string uji05() {
    return traversalPada(inorder, "modul", treeModul());
}

static string uji06() {
    return traversalPada(inorder, "penuh", treePenuh());
}

static string uji07() {
    ostringstream out;
    out << "kosong=[" << tangkapKeluaran(inorder, NULL) << "]";
    out << " " << traversalPada(inorder, "satu", treeSatu());
    out << " " << traversalPada(inorder, "miringKiri", treeMiringKiri());
    out << " " << traversalPada(inorder, "miringKanan", treeMiringKanan());
    return out.str();
}

static string uji08() {
    return traversalTidakMerusak(inorder);
}

// =============================================================================
// SOAL 1c — postorder  (4 test = 20 poin)
// =============================================================================

static string uji09() {
    return traversalPada(postorder, "modul", treeModul());
}

static string uji10() {
    return traversalPada(postorder, "penuh", treePenuh());
}

static string uji11() {
    ostringstream out;
    out << "kosong=[" << tangkapKeluaran(postorder, NULL) << "]";
    out << " " << traversalPada(postorder, "satu", treeSatu());
    out << " " << traversalPada(postorder, "miringKiri", treeMiringKiri());
    out << " " << traversalPada(postorder, "miringKanan", treeMiringKanan());
    return out.str();
}

static string uji12() {
    return traversalTidakMerusak(postorder);
}

// =============================================================================
// SOAL 2b — clear  (5 test = 25 poin)
// =============================================================================

// Tree utama dikosongkan beserta pembebasan seluruh node-nya.
static string uji13() {
    Node* t = treeModul();
    const int banyak = hitungSendiri(t);

    long long sebelum = blokHidup();
    clear(t);
    long long sesudah = blokHidup();

    ostringstream out;
    out << "root=" << (t == NULL ? "NULL" : "masihTerisi")
        << " dibebaskan=" << (sebelum - sesudah) << "/" << banyak;
    return out.str();
}

// Keadaan batas: tree yang memang sudah kosong.
static string uji14() {
    Node* kosong = NULL;
    clear(kosong);                      // harus aman, tidak boleh crash

    ostringstream out;
    out << "kosong: root=" << (kosong == NULL ? "NULL" : "berubah")
        << " aman=ya";
    return out.str();
}

// Keadaan batas: tree berisi satu node.
static string uji15() {
    Node* satu = treeSatu();
    long long sebelum = blokHidup();
    clear(satu);
    long long sesudah = blokHidup();

    ostringstream out;
    out << "satu: root=" << (satu == NULL ? "NULL" : "masihTerisi")
        << " dibebaskan=" << (sesudah < sebelum ? sebelum - sesudah : 0) << "/1";
    return out.str();
}

// Pemanggilan berulang. Bila clear() tidak mengosongkan `root`, pemanggilan
// kedua akan membuang node yang sama untuk kedua kalinya dan program berhenti
// tidak wajar — itulah yang ditangkap test ini.
static string uji16() {
    Node* t = treeModul();
    clear(t);
    clear(t);                           // pemanggilan kedua harus aman
    clear(t);                           // dan ketiga kalinya juga

    ostringstream out;
    out << "root=" << (t == NULL ? "NULL" : "masihTerisi")
        << " ulangAman=ya";
    return out.str();
}

// Tree yang menjulur dalam ke satu arah, kedua-duanya. Menangkap implementasi
// yang hanya membuang satu sisi cabang.
static string uji17() {
    Node* kiri = treeMiringKiri();
    const int banyakKiri = hitungSendiri(kiri);
    long long a1 = blokHidup();
    clear(kiri);
    long long a2 = blokHidup();

    Node* kanan = treeMiringKanan();
    const int banyakKanan = hitungSendiri(kanan);
    long long b1 = blokHidup();
    clear(kanan);
    long long b2 = blokHidup();

    ostringstream out;
    out << "miringKiri: root=" << (kiri == NULL ? "NULL" : "masihTerisi")
        << " dibebaskan=" << (a1 - a2) << "/" << banyakKiri
        << " | miringKanan: root="
        << (kanan == NULL ? "NULL" : "masihTerisi")
        << " dibebaskan=" << (b1 - b2) << "/" << banyakKanan;
    return out.str();
}

// =============================================================================
// SOAL 3 — jumlahDaun  (3 test = 15 poin)
// =============================================================================

// Tree utama pada modul, ditambah tree penuh yang seluruh daunnya berada di
// tingkat paling bawah.
static string uji18() {
    Node* a = treeModul();
    Node* b = treePenuh();

    ostringstream out;
    out << "modul=" << jumlahDaun(a)
        << " penuh=" << jumlahDaun(b);

    buangSendiri(a);
    buangSendiri(b);
    return out.str();
}

// Keadaan batas: tree kosong, tree satu node (root sekaligus daun), dan tree
// menjulur lurus yang hanya punya satu daun di ujung.
static string uji19() {
    Node* a = treeSatu();
    Node* b = treeMiringKiri();
    Node* c = treeMiringKanan();

    ostringstream out;
    out << "kosong=" << jumlahDaun(NULL)
        << " satu=" << jumlahDaun(a)
        << " miringKiri=" << jumlahDaun(b)
        << " miringKanan=" << jumlahDaun(c);

    buangSendiri(a);
    buangSendiri(b);
    buangSendiri(c);
    return out.str();
}

// Kesalahan paling umum: menganggap node yang punya SATU cabang sebagai daun.
// Sekaligus memastikan menghitung tidak mengubah tree dan hasilnya sama saat
// dipanggil dua kali.
static string uji20() {
    Node* a = treeSatuCabangKiri();
    Node* b = treeSatuCabangKanan();
    Node* c = treeAneh();

    string sebelum = bentukTree(b);
    int pertama = jumlahDaun(b);
    int kedua = jumlahDaun(b);
    string sesudah = bentukTree(b);

    ostringstream out;
    out << "satuCabangKiri=" << jumlahDaun(a)
        << " satuCabangKanan=" << jumlahDaun(b)
        << " campuran=" << jumlahDaun(c)
        << " samaDuaKali=" << bo(pertama == kedua)
        << " treeUtuh=" << bo(sebelum == sesudah);

    buangSendiri(a);
    buangSendiri(b);
    buangSendiri(c);
    return out.str();
}

// =============================================================================
// Test Suites
// =============================================================================

static void judulSuite(const string& teks) {
    cout << COLOR_CYAN << COLOR_BOLD << "\n[TEST SUITE] " << teks
         << COLOR_RESET << endl;
}

static void suiteSoal01a() {
    judulSuite("Soal 1a — preorder()");

    UJI("preorder pada tree modul",
        uji01,
        "modul=[50 80 10 70 30 20 90]");

    UJI("preorder pada tree penuh",
        uji02,
        "penuh=[1 2 4 5 3 6 7]");

    UJI("preorder pada tree kosong, tree satu node, dan tree yang menjulur "
        "lurus ke kiri maupun ke kanan",
        uji03,
        "kosong=[] satu=[42] miringKiri=[5 4 3 2 1] miringKanan=[1 2 3 4]");

    UJI("preorder pada angka negatif/nol/berulang, tidak mengubah tree, dan "
        "sama saat dipanggil dua kali",
        uji04,
        "aneh=[0 -7 7 7 0] samaDuaKali=true treeUtuh=true");
}

static void suiteSoal01b() {
    judulSuite("Soal 1b — inorder()");

    UJI("inorder pada tree modul",
        uji05,
        "modul=[10 80 30 70 50 20 90]");

    UJI("inorder pada tree penuh",
        uji06,
        "penuh=[4 2 5 1 6 3 7]");

    UJI("inorder pada tree kosong, tree satu node, dan tree yang menjulur "
        "lurus ke kiri maupun ke kanan",
        uji07,
        "kosong=[] satu=[42] miringKiri=[1 2 3 4 5] miringKanan=[1 2 3 4]");

    UJI("inorder pada angka negatif/nol/berulang, tidak mengubah tree, dan "
        "sama saat dipanggil dua kali",
        uji08,
        "aneh=[-7 7 0 0 7] samaDuaKali=true treeUtuh=true");
}

static void suiteSoal01c() {
    judulSuite("Soal 1c — postorder()");

    UJI("postorder pada tree modul",
        uji09,
        "modul=[10 30 70 80 90 20 50]");

    UJI("postorder pada tree penuh",
        uji10,
        "penuh=[4 5 2 6 7 3 1]");

    UJI("postorder pada tree kosong, tree satu node, dan tree yang menjulur "
        "lurus ke kiri maupun ke kanan",
        uji11,
        "kosong=[] satu=[42] miringKiri=[1 2 3 4 5] miringKanan=[4 3 2 1]");

    UJI("postorder pada angka negatif/nol/berulang, tidak mengubah tree, dan "
        "sama saat dipanggil dua kali",
        uji12,
        "aneh=[7 -7 0 7 0] samaDuaKali=true treeUtuh=true");
}

static void suiteSoal02b() {
    judulSuite("Soal 2b — clear()");

    UJI("clear mengosongkan root dan membebaskan seluruh node tree modul",
        uji13,
        "root=NULL dibebaskan=7/7");

    UJI("clear aman pada tree yang sudah kosong",
        uji14,
        "kosong: root=NULL aman=ya");

    UJI("clear membebaskan node satu-satunya pada tree satu node",
        uji15,
        "satu: root=NULL dibebaskan=1/1");

    UJI("clear aman dipanggil berulang kali tanpa membuang node yang sama "
        "dua kali",
        uji16,
        "root=NULL ulangAman=ya");

    UJI("clear membebaskan seluruh node pada tree yang menjulur lurus ke kiri "
        "maupun ke kanan",
        uji17,
        "miringKiri: root=NULL dibebaskan=5/5 | "
        "miringKanan: root=NULL dibebaskan=4/4");
}

static void suiteSoal03() {
    judulSuite("Soal 3 — jumlahDaun()");

    UJI("jumlahDaun pada tree modul dan pada tree penuh",
        uji18,
        "modul=3 penuh=4");

    UJI("jumlahDaun pada tree kosong, tree satu node, dan tree yang menjulur "
        "lurus ke kiri maupun ke kanan",
        uji19,
        "kosong=0 satu=1 miringKiri=1 miringKanan=1");

    UJI("jumlahDaun tidak menganggap node bercabang satu sebagai daun, "
        "tidak mengubah tree, dan sama saat dipanggil dua kali",
        uji20,
        "satuCabangKiri=2 satuCabangKanan=2 campuran=2 samaDuaKali=true treeUtuh=true");
}

// =============================================================================
// main
// =============================================================================

int main() {
    cout << COLOR_BOLD
         << "============================================" << endl;
    cout << " Praktikum Struktur Data C++ — Auto Checker" << endl;
    cout << " Pertemuan 7: Binary Tree dan Traversal" << endl;
    cout << "============================================"
         << COLOR_RESET << endl;

    suiteSoal01a();   // preorder      4 test = 20 poin
    suiteSoal01b();   // inorder       4 test = 20 poin
    suiteSoal01c();   // postorder     4 test = 20 poin
    suiteSoal02b();   // clear         5 test = 25 poin
    suiteSoal03();    // jumlahDaun    3 test = 15 poin

    // Pengaman untuk instruktur: bobot per test dihitung dari angka rencana,
    // jadi jumlah test yang benar-benar berjalan harus sama dengan rencana.
    if (total_tests != TOTAL_TEST_DIRENCANAKAN) {
        cerr << "PERINGATAN (instruktur): jumlah test berjalan ("
             << total_tests << ") tidak sama dengan rencana ("
             << TOTAL_TEST_DIRENCANAKAN << ")." << endl;
    }

    // -----------------------------------------------------------------------
    // Scoring Summary
    // -----------------------------------------------------------------------
    int score = (total_tests > 0) ? (passed_tests * 100 / total_tests) : 0;

    cout << "\n" << COLOR_BOLD
         << "============================================\n"
         << " SCORING SUMMARY\n"
         << "============================================\n"
         << COLOR_RESET;

    cout << " Tests Berhasil : " << COLOR_GREEN << COLOR_BOLD
         << passed_tests << COLOR_RESET << " / " << total_tests << "\n";
    cout << " Tests Gagal    : " << COLOR_RED << COLOR_BOLD
         << failed_tests << COLOR_RESET << " / " << total_tests << "\n";

    // Score line — warna hijau jika sempurna, kuning jika sebagian, merah jika 0
    string score_color = (score == 100) ? COLOR_GREEN
                            : (score > 0)    ? COLOR_YELLOW
                                             : COLOR_RED;
    cout << " Score          : " << score_color << COLOR_BOLD
         << score << " / 100" << COLOR_RESET << "\n";

    cout << COLOR_BOLD
         << "============================================\n"
         << COLOR_RESET;

    // -----------------------------------------------------------------------
    // Hasil yang dapat dibaca mesin.
    // Berkas inilah yang diunggah sebagai artifact dan dibaca aplikasi web.
    // -----------------------------------------------------------------------
    if (!write_result_json("result.json", score)) {
        cerr << "PERINGATAN: gagal menulis result.json" << endl;
    }

    if (failed_tests == 0) {
        cout << COLOR_GREEN << COLOR_BOLD
             << " STATUS: SEMUA TEST BERHASIL ✓\n"
             << COLOR_RESET;
        return 0; // exit code 0 = GitHub Actions SUCCESS
    } else {
        cout << COLOR_RED << COLOR_BOLD
             << " STATUS: " << failed_tests << " TEST GAGAL ✗\n"
             << COLOR_RESET;
        return 1; // exit code non-zero = GitHub Actions FAIL
    }
}
