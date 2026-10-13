// ============================================================
// MIN-HEAP + TEST & DO THOI GIAN TUNG HAM
// Y tuong: heap luu bang mang, cha i -> con 2i+1, 2i+2 (goc = nho nhat)
//   heapifyUp   : day phan tu moi len            O(log n)
//   heapifyDown : day phan tu xuong              O(log n)
//   add         : them cuoi + heapifyUp          O(log n)
//   poll        : dua cuoi len goc + heapifyDown O(log n)
//   peek        : xem goc                        O(1)
//   heapify     : dung heap tu mang (Floyd)      O(n)
//   removeAt    : xoa vi tri bat ky              O(log n)
//   heapSort    : heapify + n lan poll           O(n log n)
// Bien dich: g++ -O2 -std=c++17 MinHeapDemo_test.cpp -o minheap
// ============================================================
#include <bits/stdc++.h>
using namespace std;

class MinHeap {
private:
    int* arr;
    int maxSize;
    int size;

    void heapifyUp(int curIndex) {
        int parentIndex = (curIndex - 1) / 2;
        while (curIndex > 0 && arr[parentIndex] > arr[curIndex]) {
            swap(arr[parentIndex], arr[curIndex]);
            curIndex = parentIndex;
            parentIndex = (curIndex - 1) / 2;
        }
    }

    void heapifyDown(int curIndex) {
        while (true) {
            int leftChild = 2 * curIndex + 1;
            int rightChild = 2 * curIndex + 2;
            if (leftChild >= size) break;

            int smallestIndex = leftChild;
            if (rightChild < size && arr[rightChild] < arr[leftChild]) {
                smallestIndex = rightChild;
            }

            if (arr[curIndex] > arr[smallestIndex]) {
                swap(arr[curIndex], arr[smallestIndex]);
                curIndex = smallestIndex;
            } else {
                break;
            }
        }
    }

public:
    MinHeap(int maxSize) : maxSize(maxSize), size(0) {
        arr = new int[maxSize];
    }

    ~MinHeap() {
        delete[] arr;
    }

    bool isEmpty() {
        return size <= 0;
    }

    int getSize() const { return size; }  // [THEM]

    int peek() {
        if (isEmpty()) return -1;
        return arr[0];
    }

    void add(int v) {
        if (size >= maxSize) {
            cout << "Heap da day!" << endl;
            return;
        }
        arr[size] = v;
        size++;
        heapifyUp(size - 1);
    }

    int poll() {
        if (isEmpty()) return -1;
        int rootValue = arr[0];
        arr[0] = arr[size - 1];
        size--;
        if (size > 0) heapifyDown(0);
        return rootValue;
    }

    // Xay dung heap tu mot mang co san (Floyd's build-heap), do phuc tap O(n)
    void heapify(int inputArr[], int n) {
        if (n > maxSize) {
            cout << "So luong phan tu vuot qua suc chua cua heap!" << endl;
            return;
        }
        for (int i = 0; i < n; i++) {
            arr[i] = inputArr[i];
        }
        size = n;
        for (int i = size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    // Xoa phan tu tai vi tri bat ky trong heap
    bool removeAt(int index) {
        if (index < 0 || index >= size) {
            return false;
        }
        int lastIndex = size - 1;
        arr[index] = arr[lastIndex];
        size--;

        if (index < size) {
            int parentIndex = (index - 1) / 2;
            if (index > 0 && arr[index] < arr[parentIndex]) {
                heapifyUp(index);
            } else {
                heapifyDown(index);
            }
        }
        return true;
    }

    // [THEM] Sap xep tang dan: heapify + n lan poll, O(n log n)
    static void heapSort(int a[], int n) {
        MinHeap h(n);
        h.heapify(a, n);
        for (int i = 0; i < n; i++) a[i] = h.poll();
    }

    // [THEM] Kiem tra tinh chat min-heap: cha <= con
    bool isValid() const {
        for (int i = 1; i < size; i++)
            if (arr[(i - 1) / 2] > arr[i]) return false;
        return true;
    }

    // [THEM] In mang heap (dung cho demo nho)
    void print(const string& label) const {
        cout << label << ": ";
        for (int i = 0; i < size; i++) cout << arr[i] << ' ';
        cout << '\n';
    }
};

// ---------- tien ich do thoi gian ----------
template <class F>
double timeMs(F f) {
    auto s = chrono::high_resolution_clock::now();
    f();
    return chrono::duration<double, milli>(chrono::high_resolution_clock::now() - s).count();
}

void printRow(const string& name, double ms, long long ops, const string& note = "") {
    printf("%-24s %10.4f ms | %10.2f ns/lan | %s\n", name.c_str(), ms, ms * 1e6 / ops, note.c_str());
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int MAXN = 100000;
    cout << "=== TEST MIN HEAP VOI DATASET CUA BAN (toi da " << MAXN << " phan tu) ===\n";
    cout << "Nhap N: ";
    int n;
    if (!(cin >> n) || n <= 0 || n > MAXN) {
        cout << "N phai nam trong khoang 1.." << MAXN << "\n";
        return 1;
    }
    cout << "Nhap " << n << " so nguyen (cach nhau boi khoang trang hoac xuong dong):\n";
    vector<int> data(n);
    for (auto& x : data) {
        if (!(cin >> x)) {
            cout << "Thieu du lieu!\n";
            return 1;
        }
    }
    cout << "\n--------------------------------------------------\n";

    volatile long long sink = 0;  // tranh compiler xoa code
    mt19937 rng(42);
    MinHeap h(n);

    // 1. add
    double t = timeMs([&] { for (int x : data) h.add(x); });
    printRow("add x N", t, n, h.isValid() ? "heap hop le: OK" : "LOI");
    if (n <= 20) h.print("   heap sau add");

    // 2. peek (lap 1e6 lan vi 1 lan qua nho de do)
    int minTrue = *min_element(data.begin(), data.end());
    const int R = 1000000;
    t = timeMs([&] { for (int i = 0; i < R; i++) sink += h.peek(); });
    printRow("peek x 1e6", t, R,
             "peek = " + to_string(h.peek()) + (h.peek() == minTrue ? " (dung min)" : " (SAI)"));

    // 3. removeAt tai vi tri ngau nhien (xoa N/10 phan tu, it nhat 1)
    int K = max(1, n / 10);
    t = timeMs([&] { for (int k = 0; k < K; k++) h.removeAt(rng() % h.getSize()); });
    printRow("removeAt x (N/10)", t, K, h.isValid() ? "heap hop le: OK" : "LOI");

    // 4. poll toan bo phan tu con lai, kiem tra thu tu tang dan
    int remain = h.getSize();
    bool sorted = true;
    t = timeMs([&] {
        int prev = INT_MIN;
        while (!h.isEmpty()) {
            int v = h.poll();
            if (v < prev) sorted = false;
            prev = v;
        }
    });
    printRow("poll x (con lai)", t, remain, sorted ? "thu tu tang: OK" : "LOI");

    // 5. heapify (Floyd, O(n)) tu mang du lieu
    MinHeap h2(n);
    t = timeMs([&] { h2.heapify(data.data(), n); });
    printRow("heapify (build heap)", t, n, h2.isValid() ? "heap hop le: OK" : "LOI");
    if (n <= 20) h2.print("   heap sau heapify");

    // 6. heapSort so voi std::sort
    vector<int> s1 = data, s2 = data;
    double th = timeMs([&] { MinHeap::heapSort(s1.data(), n); });
    double ts = timeMs([&] { sort(s2.begin(), s2.end()); });
    printRow("heapSort", th, n, is_sorted(s1.begin(), s1.end()) ? "da sap xep: OK" : "LOI");
    printRow("std::sort (so sanh)", ts, n, "");

    cout << "--------------------------------------------------\n";
    cout << "(ns/lan = thoi gian trung binh moi lan goi; heapify/heapSort chia deu cho N)\n";
    return 0;
}
