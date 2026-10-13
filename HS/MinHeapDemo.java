// ============================================================
// MIN-HEAP + TEST & DO THOI GIAN TUNG HAM (Java)
// Cha i -> con 2i+1, 2i+2 (goc = nho nhat)
//   add O(log n) | poll O(log n) | peek O(1) | heapify O(n)
//   removeAt O(log n) | heapSort O(n log n)
// Chay: javac MinHeapDemo.java && java MinHeapDemo < input.txt
// ============================================================
import java.io.*;
import java.util.*;

public class MinHeapDemo {

    static class MinHeap {
        private final int[] arr;
        private final int maxSize;
        private int size;

        MinHeap(int maxSize) {
            this.maxSize = maxSize;
            this.arr = new int[maxSize];
        }

        private void swap(int i, int j) { int t = arr[i]; arr[i] = arr[j]; arr[j] = t; }

        private void heapifyUp(int cur) {
            while (cur > 0) {
                int parent = (cur - 1) / 2;
                if (arr[parent] <= arr[cur]) break;
                swap(parent, cur);
                cur = parent;
            }
        }

        private void heapifyDown(int cur) {
            while (true) {
                int left = 2 * cur + 1, right = left + 1;
                if (left >= size) break;
                int smallest = (right < size && arr[right] < arr[left]) ? right : left;
                if (arr[cur] <= arr[smallest]) break;
                swap(cur, smallest);
                cur = smallest;
            }
        }

        boolean isEmpty() { return size <= 0; }
        int getSize() { return size; }
        int peek() { return isEmpty() ? -1 : arr[0]; }

        void add(int v) {
            if (size >= maxSize) { System.out.println("Heap da day!"); return; }
            arr[size++] = v;
            heapifyUp(size - 1);
        }

        int poll() {
            if (isEmpty()) return -1;
            int root = arr[0];
            arr[0] = arr[--size];
            if (size > 0) heapifyDown(0);
            return root;
        }

        // Floyd build-heap, O(n)
        void heapify(int[] input, int n) {
            if (n > maxSize) { System.out.println("So luong phan tu vuot qua suc chua cua heap!"); return; }
            System.arraycopy(input, 0, arr, 0, n);
            size = n;
            for (int i = size / 2 - 1; i >= 0; i--) heapifyDown(i);
        }

        boolean removeAt(int index) {
            if (index < 0 || index >= size) return false;
            arr[index] = arr[--size];
            if (index < size) {
                if (index > 0 && arr[index] < arr[(index - 1) / 2]) heapifyUp(index);
                else heapifyDown(index);
            }
            return true;
        }

        static void heapSort(int[] a, int n) {
            MinHeap h = new MinHeap(n);
            h.heapify(a, n);
            for (int i = 0; i < n; i++) a[i] = h.poll();
        }

        boolean isValid() {
            for (int i = 1; i < size; i++)
                if (arr[(i - 1) / 2] > arr[i]) return false;
            return true;
        }

        void print(String label) {
            StringBuilder sb = new StringBuilder(label).append(": ");
            for (int i = 0; i < size; i++) sb.append(arr[i]).append(' ');
            System.out.println(sb);
        }
    }

    // ---------- tien ich do thoi gian ----------
    static double timeMs(Runnable f) {
        long s = System.nanoTime();
        f.run();
        return (System.nanoTime() - s) / 1e6;
    }

    static void printRow(String name, double ms, long ops, String note) {
        System.out.print(String.format(Locale.US, "%-24s %10.4f ms | %10.2f ns/lan | %s%n",
                name, ms, ms * 1e6 / ops, note));
    }

    static volatile long sink = 0; // tranh JIT xoa code
    static boolean sortedFlag;     // dung trong lambda

    static boolean isSorted(int[] a) {
        for (int i = 1; i < a.length; i++) if (a[i - 1] > a[i]) return false;
        return true;
    }

    public static void main(String[] args) throws IOException {
        final int MAXN = 100000;
        System.out.println("=== TEST MIN HEAP VOI DATASET CUA BAN (toi da " + MAXN + " phan tu) ===");
        System.out.print("Nhap N: ");
        System.out.flush();

        StreamTokenizer in = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));
        if (in.nextToken() == StreamTokenizer.TT_EOF || (int) in.nval <= 0 || (int) in.nval > MAXN) {
            System.out.println("N phai nam trong khoang 1.." + MAXN);
            return;
        }
        final int n = (int) in.nval;
        System.out.println("Nhap " + n + " so nguyen (cach nhau boi khoang trang hoac xuong dong):");
        final int[] data = new int[n];
        for (int i = 0; i < n; i++) {
            if (in.nextToken() != StreamTokenizer.TT_NUMBER) { System.out.println("Thieu du lieu!"); return; }
            data[i] = (int) in.nval;
        }
        System.out.println("\n--------------------------------------------------");

        Random rng = new Random(42);
        MinHeap h = new MinHeap(n);

        // 1. add
        double t = timeMs(() -> { for (int x : data) h.add(x); });
        printRow("add x N", t, n, h.isValid() ? "heap hop le: OK" : "LOI");
        if (n <= 20) h.print("   heap sau add");

        // 2. peek (lap 1e6 lan)
        int minTrue = Arrays.stream(data).min().getAsInt();
        final int R = 1_000_000;
        t = timeMs(() -> { long s = 0; for (int i = 0; i < R; i++) s += h.peek(); sink = s; });
        printRow("peek x 1e6", t, R, "peek = " + h.peek() + (h.peek() == minTrue ? " (dung min)" : " (SAI)"));

        // 3. removeAt ngau nhien (N/10 phan tu, it nhat 1)
        final int K = Math.max(1, n / 10);
        t = timeMs(() -> { for (int k = 0; k < K; k++) h.removeAt(rng.nextInt(h.getSize())); });
        printRow("removeAt x (N/10)", t, K, h.isValid() ? "heap hop le: OK" : "LOI");

        // 4. poll toan bo phan tu con lai, kiem tra tang dan
        final int remain = h.getSize();
        sortedFlag = true;
        t = timeMs(() -> {
            int prev = Integer.MIN_VALUE;
            while (!h.isEmpty()) {
                int v = h.poll();
                if (v < prev) sortedFlag = false;
                prev = v;
            }
        });
        printRow("poll x (con lai)", t, remain, sortedFlag ? "thu tu tang: OK" : "LOI");

        // 5. heapify (Floyd, O(n))
        MinHeap h2 = new MinHeap(n);
        t = timeMs(() -> h2.heapify(data, n));
        printRow("heapify (build heap)", t, n, h2.isValid() ? "heap hop le: OK" : "LOI");
        if (n <= 20) h2.print("   heap sau heapify");

        // 6. heapSort so voi Arrays.sort
        int[] s1 = data.clone(), s2 = data.clone();
        double th = timeMs(() -> MinHeap.heapSort(s1, n));
        double ts = timeMs(() -> Arrays.sort(s2));
        printRow("heapSort", th, n, isSorted(s1) ? "da sap xep: OK" : "LOI");
        printRow("Arrays.sort (so sanh)", ts, n, "");

        System.out.println("--------------------------------------------------");
        System.out.println("(ns/lan = thoi gian trung binh moi lan goi; heapify/heapSort chia deu cho N)");
    }
}
