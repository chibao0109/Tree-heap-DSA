/*
 * So sánh thời gian chạy của 4 cấu trúc dữ liệu:
 *   - BT   : Binary Tree (cây nhị phân thường, chèn theo tầng - không có thứ tự)
 *   - BST  : Binary Search Tree (không cân bằng)
 *   - AVL  : AVL Tree (BST tự cân bằng)
 *   - Heap : Max-heap + Heapsort
 *
 * Các phép đo cho mỗi cấu trúc:
 *   1. insert : chèn lần lượt n phần tử
 *   2. search : tìm m khóa (một nửa có trong cây, một nửa không có)
 *   3. sort   : xuất dãy tăng dần (BST/AVL: duyệt inorder, Heap: heapsort,
 *               BT: gom phần tử rồi sắp xếp vì BT không có thứ tự)
 *
 * Hai kiểu dữ liệu đầu vào:
 *   - random : hoán vị ngẫu nhiên
 *   - sorted : dãy tăng dần (trường hợp xấu nhất của BST -> suy biến thành danh sách)
 *
 * Biên dịch:  g++ -O2 -std=c++17 ds_benchmark.cpp -o ds_benchmark
 * Chạy:       ./ds_benchmark
 */

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <queue>
#include <random>
#include <stack>
#include <string>
#include <vector>

using namespace std;

// ====================== CẤU HÌNH ======================
const vector<int> SIZES = {5000, 10000, 20000, 40000};  // các kích thước n
const int NUM_SEARCH = 1000;                            // số khóa tìm kiếm m
const int REPEAT = 3;                                   // số lần lặp, lấy trung bình
const unsigned SEED = 42;
const char* CSV_FILE = "results.csv";

struct Node {
    int key;
    Node* left = nullptr;
    Node* right = nullptr;
    int height = 1;  // chỉ dùng cho AVL
    explicit Node(int k) : key(k) {}
};

// Duyệt inorder bằng stack (tránh tràn đệ quy khi BST suy biến)
void inorder(Node* root, vector<int>& out) {
    stack<Node*> st;
    Node* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        out.push_back(cur->key);
        cur = cur->right;
    }
}

// Giải phóng cây không dùng đệ quy
void freeTree(Node* root) {
    if (!root) return;
    stack<Node*> st;
    st.push(root);
    while (!st.empty()) {
        Node* n = st.top(); st.pop();
        if (n->left) st.push(n->left);
        if (n->right) st.push(n->right);
        delete n;
    }
}

// ====================== 1. BINARY TREE ======================
class BinaryTree {
    Node* root = nullptr;
    queue<Node*> q;  // các nút còn thiếu con
public:
    ~BinaryTree() { freeTree(root); }

    void insert(int key) {                       // O(1)
        Node* node = new Node(key);
        if (!root) {
            root = node;
        } else {
            Node* parent = q.front();
            if (!parent->left) {
                parent->left = node;
            } else {
                parent->right = node;
                q.pop();
            }
        }
        q.push(node);
    }

    bool search(int key) const {                 // O(n) - duyệt BFS toàn cây
        if (!root) return false;
        queue<Node*> bfs;
        bfs.push(root);
        while (!bfs.empty()) {
            Node* n = bfs.front(); bfs.pop();
            if (n->key == key) return true;
            if (n->left) bfs.push(n->left);
            if (n->right) bfs.push(n->right);
        }
        return false;
    }

    vector<int> sortedList() const {             // O(n log n)
        vector<int> out;
        inorder(root, out);
        sort(out.begin(), out.end());
        return out;
    }
};

// ====================== 2. BST ======================
class BST {
    Node* root = nullptr;
public:
    ~BST() { freeTree(root); }

    void insert(int key) {                       // O(h): log n -> n
        if (!root) { root = new Node(key); return; }
        Node* cur = root;
        while (true) {
            if (key < cur->key) {
                if (!cur->left) { cur->left = new Node(key); return; }
                cur = cur->left;
            } else if (key > cur->key) {
                if (!cur->right) { cur->right = new Node(key); return; }
                cur = cur->right;
            } else {
                return;  // bỏ qua khóa trùng
            }
        }
    }

    bool search(int key) const {                 // O(h)
        Node* cur = root;
        while (cur) {
            if (key == cur->key) return true;
            cur = (key < cur->key) ? cur->left : cur->right;
        }
        return false;
    }

    vector<int> sortedList() const {             // O(n)
        vector<int> out;
        inorder(root, out);
        return out;
    }
};

// ====================== 3. AVL ======================
class AVL {
    Node* root = nullptr;

    static int h(Node* n) { return n ? n->height : 0; }
    static void update(Node* n) { n->height = 1 + max(h(n->left), h(n->right)); }
    static int bal(Node* n) { return h(n->left) - h(n->right); }

    static Node* rotRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        update(y); update(x);
        return x;
    }

    static Node* rotLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        update(x); update(y);
        return y;
    }

    static Node* insertRec(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->key)      node->left = insertRec(node->left, key);
        else if (key > node->key) node->right = insertRec(node->right, key);
        else return node;

        update(node);
        int b = bal(node);
        if (b > 1) {                                   // lệch trái
            if (key > node->left->key)                 // LR
                node->left = rotLeft(node->left);
            return rotRight(node);                     // LL
        }
        if (b < -1) {                                  // lệch phải
            if (key < node->right->key)                // RL
                node->right = rotRight(node->right);
            return rotLeft(node);                      // RR
        }
        return node;
    }
public:
    ~AVL() { freeTree(root); }

    void insert(int key) { root = insertRec(root, key); }   // O(log n)

    bool search(int key) const {                 // O(log n)
        Node* cur = root;
        while (cur) {
            if (key == cur->key) return true;
            cur = (key < cur->key) ? cur->left : cur->right;
        }
        return false;
    }

    vector<int> sortedList() const {             // O(n)
        vector<int> out;
        inorder(root, out);
        return out;
    }
};

// ====================== 4. HEAP / HEAPSORT ======================
class Heap {  // Max-heap lưu trên mảng
    vector<int> a;

    static void siftDown(vector<int>& v, int i, int end) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, big = i;
            if (l < end && v[l] > v[big]) big = l;
            if (r < end && v[r] > v[big]) big = r;
            if (big == i) return;
            swap(v[i], v[big]);
            i = big;
        }
    }
public:
    void insert(int key) {                       // O(log n) - sift up
        a.push_back(key);
        int i = (int)a.size() - 1;
        while (i > 0) {
            int p = (i - 1) / 2;
            if (a[p] >= a[i]) break;
            swap(a[p], a[i]);
            i = p;
        }
    }

    bool search(int key) const {                 // O(n) - heap không hỗ trợ tìm nhanh
        for (int x : a)
            if (x == key) return true;
        return false;
    }

    vector<int> sortedList() const {             // Heapsort O(n log n)
        vector<int> v = a;                       // đã là max-heap
        for (int end = (int)v.size() - 1; end > 0; --end) {
            swap(v[0], v[end]);
            siftDown(v, 0, end);
        }
        return v;
    }
};

// ====================== BENCHMARK ======================
using Clock = chrono::high_resolution_clock;

static double ms(Clock::time_point a, Clock::time_point b) {
    return chrono::duration<double, milli>(b - a).count();
}

vector<int> makeData(int n, const string& kind, mt19937& rng) {
    vector<int> d;
    if (kind == "random") {
        vector<int> pool(n * 10);
        for (int i = 0; i < n * 10; ++i) pool[i] = i;
        shuffle(pool.begin(), pool.end(), rng);
        d.assign(pool.begin(), pool.begin() + n);
    } else {  // sorted
        d.resize(n);
        for (int i = 0; i < n; ++i) d[i] = i;
    }
    return d;
}

vector<int> makeQueries(const vector<int>& data, int m, mt19937& rng) {
    vector<int> tmp = data, q;
    shuffle(tmp.begin(), tmp.end(), rng);
    for (int i = 0; i < m / 2; ++i) q.push_back(tmp[i]);       // có trong cây
    for (int i = 0; i < m - m / 2; ++i) q.push_back(-(i + 1)); // chắc chắn không có
    shuffle(q.begin(), q.end(), rng);
    return q;
}

struct Times { double ins = 0, sea = 0, srt = 0; };

template <class DS>
Times benchOne(const vector<int>& data, const vector<int>& queries, const vector<int>& expected) {
    Times t;
    DS ds;
    auto t0 = Clock::now();
    for (int x : data) ds.insert(x);
    auto t1 = Clock::now();
    int found = 0;
    for (int k : queries) found += ds.search(k);
    auto t2 = Clock::now();
    vector<int> out = ds.sortedList();
    auto t3 = Clock::now();

    if (out != expected || found != (int)queries.size() / 2) {
        fprintf(stderr, "Lỗi: kết quả sai!\n");
        exit(1);
    }
    t.ins = ms(t0, t1); t.sea = ms(t1, t2); t.srt = ms(t2, t3);
    return t;
}

template <class DS>
Times benchAvg(const vector<int>& data, const vector<int>& queries, const vector<int>& expected) {
    Times sum;
    for (int r = 0; r < REPEAT; ++r) {
        Times t = benchOne<DS>(data, queries, expected);
        sum.ins += t.ins; sum.sea += t.sea; sum.srt += t.srt;
    }
    sum.ins /= REPEAT; sum.sea /= REPEAT; sum.srt /= REPEAT;
    return sum;
}

// Đọc file dữ liệu: dòng đầu là n, sau đó n số nguyên
vector<int> loadFile(const string& path) {
    ifstream in(path);
    if (!in) { fprintf(stderr, "Không mở được file: %s\n", path.c_str()); exit(1); }
    int n;
    in >> n;
    vector<int> d(n);
    for (int i = 0; i < n; ++i)
        if (!(in >> d[i])) { fprintf(stderr, "File %s thiếu phần tử\n", path.c_str()); exit(1); }
    return d;
}

// Chạy 4 cấu trúc trên một tập dữ liệu, in bảng và ghi CSV
void runOne(const string& label, const vector<int>& data, mt19937& rng, ofstream& csv) {
    int n = (int)data.size();
    vector<int> queries = makeQueries(data, NUM_SEARCH, rng);
    vector<int> expected = data;
    sort(expected.begin(), expected.end());

    vector<pair<string, Times>> res = {
        {"BT",   benchAvg<BinaryTree>(data, queries, expected)},
        {"BST",  benchAvg<BST>(data, queries, expected)},
        {"AVL",  benchAvg<AVL>(data, queries, expected)},
        {"Heap", benchAvg<Heap>(data, queries, expected)},
    };
    for (auto& [name, t] : res) {
        printf("%7d %8s %10.3f %10.3f %10.3f %10.3f\n",
               n, name.c_str(), t.ins, t.sea, t.srt, t.ins + t.sea + t.srt);
        csv << label << "," << n << "," << name << ","
            << t.ins << "," << t.sea << "," << t.srt << "\n";
    }
    printf("------------------------------------------------------------\n");
}

void printHeader(const string& label) {
    printf("\n=== Dữ liệu: %s  (m = %d lần tìm, TB %d lần, đơn vị: ms) ===\n",
           label.c_str(), NUM_SEARCH, REPEAT);
    printf("%7s %8s %10s %10s %10s %10s\n", "n", "DS", "insert", "search", "sort", "total");
}

/*
 * Cách dùng:
 *   ./ds_benchmark                         -> tự sinh dữ liệu (random + sorted, theo SIZES)
 *   ./ds_benchmark f1.txt f2.txt ...       -> chạy trên các file dữ liệu cho trước
 */
int main(int argc, char* argv[]) {
    mt19937 rng(SEED);
    ofstream csv(CSV_FILE);
    csv << "data,n,structure,insert_ms,search_ms,sort_ms\n";

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            string path = argv[i];
            vector<int> data = loadFile(path);
            printHeader(path);
            runOne(path, data, rng, csv);
        }
    } else {
        for (string kind : {"random", "sorted"}) {
            printHeader(kind);
            for (int n : SIZES) runOne(kind, makeData(n, kind, rng), rng, csv);
        }
    }
    printf("\nĐã lưu kết quả vào %s\n", CSV_FILE);
    return 0;
}