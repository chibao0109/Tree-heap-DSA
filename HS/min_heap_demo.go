// ============================================================
// MIN-HEAP + TEST & DO THOI GIAN TUNG HAM (Go)
// Cha i -> con 2i+1, 2i+2 (goc = nho nhat)
//   Add O(log n) | Poll O(log n) | Peek O(1) | Heapify O(n)
//   RemoveAt O(log n) | HeapSort O(n log n)
// Chay: go run min_heap_demo.go < input.txt
// ============================================================
package main

import (
	"bufio"
	"fmt"
	"math"
	"math/rand"
	"os"
	"sort"
	"strings"
	"time"
)

type MinHeap struct {
	arr     []int
	maxSize int
	size    int
}

func NewMinHeap(maxSize int) *MinHeap {
	return &MinHeap{arr: make([]int, maxSize), maxSize: maxSize}
}

func (h *MinHeap) heapifyUp(cur int) {
	for cur > 0 {
		parent := (cur - 1) / 2
		if h.arr[parent] <= h.arr[cur] {
			break
		}
		h.arr[parent], h.arr[cur] = h.arr[cur], h.arr[parent]
		cur = parent
	}
}

func (h *MinHeap) heapifyDown(cur int) {
	for {
		left := 2*cur + 1
		if left >= h.size {
			break
		}
		smallest := left
		if right := left + 1; right < h.size && h.arr[right] < h.arr[left] {
			smallest = right
		}
		if h.arr[cur] <= h.arr[smallest] {
			break
		}
		h.arr[cur], h.arr[smallest] = h.arr[smallest], h.arr[cur]
		cur = smallest
	}
}

func (h *MinHeap) IsEmpty() bool { return h.size <= 0 }
func (h *MinHeap) Size() int     { return h.size }

func (h *MinHeap) Peek() int {
	if h.IsEmpty() {
		return -1
	}
	return h.arr[0]
}

func (h *MinHeap) Add(v int) {
	if h.size >= h.maxSize {
		fmt.Println("Heap da day!")
		return
	}
	h.arr[h.size] = v
	h.size++
	h.heapifyUp(h.size - 1)
}

func (h *MinHeap) Poll() int {
	if h.IsEmpty() {
		return -1
	}
	root := h.arr[0]
	h.size--
	h.arr[0] = h.arr[h.size]
	if h.size > 0 {
		h.heapifyDown(0)
	}
	return root
}

// Heapify: Floyd build-heap, O(n)
func (h *MinHeap) Heapify(input []int, n int) {
	if n > h.maxSize {
		fmt.Println("So luong phan tu vuot qua suc chua cua heap!")
		return
	}
	copy(h.arr, input[:n])
	h.size = n
	for i := h.size/2 - 1; i >= 0; i-- {
		h.heapifyDown(i)
	}
}

func (h *MinHeap) RemoveAt(index int) bool {
	if index < 0 || index >= h.size {
		return false
	}
	h.size--
	h.arr[index] = h.arr[h.size]
	if index < h.size {
		if index > 0 && h.arr[index] < h.arr[(index-1)/2] {
			h.heapifyUp(index)
		} else {
			h.heapifyDown(index)
		}
	}
	return true
}

func HeapSort(a []int, n int) {
	h := NewMinHeap(n)
	h.Heapify(a, n)
	for i := 0; i < n; i++ {
		a[i] = h.Poll()
	}
}

func (h *MinHeap) IsValid() bool {
	for i := 1; i < h.size; i++ {
		if h.arr[(i-1)/2] > h.arr[i] {
			return false
		}
	}
	return true
}

func (h *MinHeap) Print(label string) {
	var sb strings.Builder
	for i := 0; i < h.size; i++ {
		fmt.Fprintf(&sb, "%d ", h.arr[i])
	}
	fmt.Printf("%s: %s\n", label, sb.String())
}

// ---------- tien ich do thoi gian ----------
func timeMs(f func()) float64 {
	s := time.Now()
	f()
	return float64(time.Since(s).Nanoseconds()) / 1e6
}

func printRow(name string, ms float64, ops int, note string) {
	fmt.Printf("%-24s %10.4f ms | %10.2f ns/lan | %s\n", name, ms, ms*1e6/float64(ops), note)
}

func okOr(cond bool, ok string) string {
	if cond {
		return ok
	}
	return "LOI"
}

var sink int // tranh compiler xoa code

func main() {
	const MAXN = 100000
	fmt.Printf("=== TEST MIN HEAP VOI DATASET CUA BAN (toi da %d phan tu) ===\n", MAXN)
	fmt.Print("Nhap N: ")

	in := bufio.NewReaderSize(os.Stdin, 1<<20)
	var n int
	if _, err := fmt.Fscan(in, &n); err != nil || n <= 0 || n > MAXN {
		fmt.Printf("N phai nam trong khoang 1..%d\n", MAXN)
		return
	}
	fmt.Printf("Nhap %d so nguyen (cach nhau boi khoang trang hoac xuong dong):\n", n)
	data := make([]int, n)
	for i := range data {
		if _, err := fmt.Fscan(in, &data[i]); err != nil {
			fmt.Println("Thieu du lieu!")
			return
		}
	}
	fmt.Println("\n" + strings.Repeat("-", 50))

	rng := rand.New(rand.NewSource(42))
	h := NewMinHeap(n)

	// 1. add
	t := timeMs(func() {
		for _, x := range data {
			h.Add(x)
		}
	})
	printRow("add x N", t, n, okOr(h.IsValid(), "heap hop le: OK"))
	if n <= 20 {
		h.Print("   heap sau add")
	}

	// 2. peek (lap 1e6 lan)
	minTrue := data[0]
	for _, x := range data {
		if x < minTrue {
			minTrue = x
		}
	}
	const R = 1_000_000
	t = timeMs(func() {
		s := 0
		for i := 0; i < R; i++ {
			s += h.Peek()
		}
		sink = s
	})
	verdict := " (SAI)"
	if h.Peek() == minTrue {
		verdict = " (dung min)"
	}
	printRow("peek x 1e6", t, R, fmt.Sprintf("peek = %d%s", h.Peek(), verdict))

	// 3. removeAt ngau nhien (N/10 phan tu, it nhat 1)
	K := n / 10
	if K < 1 {
		K = 1
	}
	t = timeMs(func() {
		for k := 0; k < K; k++ {
			h.RemoveAt(rng.Intn(h.Size()))
		}
	})
	printRow("removeAt x (N/10)", t, K, okOr(h.IsValid(), "heap hop le: OK"))

	// 4. poll toan bo phan tu con lai, kiem tra tang dan
	remain := h.Size()
	sorted := true
	t = timeMs(func() {
		prev := math.MinInt
		for !h.IsEmpty() {
			v := h.Poll()
			if v < prev {
				sorted = false
			}
			prev = v
		}
	})
	printRow("poll x (con lai)", t, remain, okOr(sorted, "thu tu tang: OK"))

	// 5. heapify (Floyd, O(n))
	h2 := NewMinHeap(n)
	t = timeMs(func() { h2.Heapify(data, n) })
	printRow("heapify (build heap)", t, n, okOr(h2.IsValid(), "heap hop le: OK"))
	if n <= 20 {
		h2.Print("   heap sau heapify")
	}

	// 6. heapSort so voi sort.Ints
	s1 := append([]int(nil), data...)
	s2 := append([]int(nil), data...)
	th := timeMs(func() { HeapSort(s1, n) })
	ts := timeMs(func() { sort.Ints(s2) })
	printRow("heapSort", th, n, okOr(sort.IntsAreSorted(s1), "da sap xep: OK"))
	printRow("sort.Ints (so sanh)", ts, n, "")

	fmt.Println(strings.Repeat("-", 50))
	fmt.Println("(ns/lan = thoi gian trung binh moi lan goi; heapify/heapSort chia deu cho N)")
}
