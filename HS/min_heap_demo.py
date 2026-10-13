# ============================================================
# MIN-HEAP + TEST & DO THOI GIAN TUNG HAM (Python 3)
# Cha i -> con 2i+1, 2i+2 (goc = nho nhat)
#   add O(log n) | poll O(log n) | peek O(1) | heapify O(n)
#   remove_at O(log n) | heap_sort O(n log n)
# Chay: python min_heap_demo.py < input.txt
# ============================================================
import random
import sys
import time


class MinHeap:
    __slots__ = ("_arr", "_max_size", "_size")

    def __init__(self, max_size: int):
        self._max_size = max_size
        self._arr = [0] * max_size
        self._size = 0

    def _heapify_up(self, cur: int) -> None:
        arr = self._arr
        while cur > 0:
            parent = (cur - 1) >> 1
            if arr[parent] <= arr[cur]:
                break
            arr[parent], arr[cur] = arr[cur], arr[parent]
            cur = parent

    def _heapify_down(self, cur: int) -> None:
        arr, size = self._arr, self._size
        while True:
            left = 2 * cur + 1
            if left >= size:
                break
            right = left + 1
            smallest = right if right < size and arr[right] < arr[left] else left
            if arr[cur] <= arr[smallest]:
                break
            arr[cur], arr[smallest] = arr[smallest], arr[cur]
            cur = smallest

    def is_empty(self) -> bool:
        return self._size <= 0

    def get_size(self) -> int:
        return self._size

    def peek(self) -> int:
        return -1 if self._size == 0 else self._arr[0]

    def add(self, v: int) -> None:
        if self._size >= self._max_size:
            print("Heap da day!")
            return
        self._arr[self._size] = v
        self._size += 1
        self._heapify_up(self._size - 1)

    def poll(self) -> int:
        if self._size == 0:
            return -1
        root = self._arr[0]
        self._size -= 1
        self._arr[0] = self._arr[self._size]
        if self._size > 0:
            self._heapify_down(0)
        return root

    def heapify(self, data, n: int) -> None:
        """Floyd build-heap, O(n)."""
        if n > self._max_size:
            print("So luong phan tu vuot qua suc chua cua heap!")
            return
        self._arr[:n] = data[:n]
        self._size = n
        for i in range(n // 2 - 1, -1, -1):
            self._heapify_down(i)

    def remove_at(self, index: int) -> bool:
        if index < 0 or index >= self._size:
            return False
        self._size -= 1
        self._arr[index] = self._arr[self._size]
        if index < self._size:
            if index > 0 and self._arr[index] < self._arr[(index - 1) >> 1]:
                self._heapify_up(index)
            else:
                self._heapify_down(index)
        return True

    @staticmethod
    def heap_sort(a: list, n: int) -> None:
        h = MinHeap(n)
        h.heapify(a, n)
        for i in range(n):
            a[i] = h.poll()

    def is_valid(self) -> bool:
        arr = self._arr
        return all(arr[(i - 1) >> 1] <= arr[i] for i in range(1, self._size))

    def print(self, label: str) -> None:
        print(f"{label}: {' '.join(map(str, self._arr[:self._size]))} ")


# ---------- tien ich do thoi gian ----------
def time_ms(f) -> float:
    s = time.perf_counter()
    f()
    return (time.perf_counter() - s) * 1000.0


def print_row(name: str, ms: float, ops: int, note: str = "") -> None:
    print(f"{name:<24} {ms:10.4f} ms | {ms * 1e6 / ops:10.2f} ns/lan | {note}")


def main() -> None:
    MAXN = 100000
    print(f"=== TEST MIN HEAP VOI DATASET CUA BAN (toi da {MAXN} phan tu) ===")
    print("Nhap N: ", end="", flush=True)

    tokens = sys.stdin.read().split()
    try:
        n = int(tokens[0])
    except (IndexError, ValueError):
        n = 0
    if n <= 0 or n > MAXN:
        print(f"N phai nam trong khoang 1..{MAXN}")
        return
    print(f"Nhap {n} so nguyen (cach nhau boi khoang trang hoac xuong dong):")
    try:
        data = [int(x) for x in tokens[1:n + 1]]
    except ValueError:
        data = []
    if len(data) < n:
        print("Thieu du lieu!")
        return
    print("\n" + "-" * 50)

    sink = 0  # tranh bi toi uu hoa
    rng = random.Random(42)
    h = MinHeap(n)

    # 1. add
    def run_add():
        for x in data:
            h.add(x)
    t = time_ms(run_add)
    print_row("add x N", t, n, "heap hop le: OK" if h.is_valid() else "LOI")
    if n <= 20:
        h.print("   heap sau add")

    # 2. peek (lap 1e6 lan)
    min_true = min(data)
    R = 1_000_000

    def run_peek():
        nonlocal sink
        peek = h.peek
        s = 0
        for _ in range(R):
            s += peek()
        sink = s
    t = time_ms(run_peek)
    print_row("peek x 1e6", t, R,
              f"peek = {h.peek()}" + (" (dung min)" if h.peek() == min_true else " (SAI)"))

    # 3. remove_at ngau nhien (N/10 phan tu, it nhat 1)
    K = max(1, n // 10)

    def run_remove():
        for _ in range(K):
            h.remove_at(rng.randrange(h.get_size()))
    t = time_ms(run_remove)
    print_row("removeAt x (N/10)", t, K, "heap hop le: OK" if h.is_valid() else "LOI")

    # 4. poll toan bo phan tu con lai, kiem tra tang dan
    remain = h.get_size()
    is_sorted_flag = True

    def run_poll():
        nonlocal is_sorted_flag
        prev = float("-inf")
        while not h.is_empty():
            v = h.poll()
            if v < prev:
                is_sorted_flag = False
            prev = v
    t = time_ms(run_poll)
    print_row("poll x (con lai)", t, remain, "thu tu tang: OK" if is_sorted_flag else "LOI")

    # 5. heapify (Floyd, O(n))
    h2 = MinHeap(n)
    t = time_ms(lambda: h2.heapify(data, n))
    print_row("heapify (build heap)", t, n, "heap hop le: OK" if h2.is_valid() else "LOI")
    if n <= 20:
        h2.print("   heap sau heapify")

    # 6. heap_sort so voi list.sort
    s1, s2 = data[:], data[:]
    th = time_ms(lambda: MinHeap.heap_sort(s1, n))
    ts = time_ms(s2.sort)
    print_row("heapSort", th, n, "da sap xep: OK" if s1 == sorted(data) else "LOI")
    print_row("list.sort (so sanh)", ts, n)

    print("-" * 50)
    print("(ns/lan = thoi gian trung binh moi lan goi; heapify/heapSort chia deu cho N)")


if __name__ == "__main__":
    main()
