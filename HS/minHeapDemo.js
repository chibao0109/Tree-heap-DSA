// ============================================================
// MIN-HEAP + TEST & DO THOI GIAN TUNG HAM (JavaScript / Node.js)
// Cha i -> con 2i+1, 2i+2 (goc = nho nhat)
//   add O(log n) | poll O(log n) | peek O(1) | heapify O(n)
//   removeAt O(log n) | heapSort O(n log n)
// Chay: node minHeapDemo.js < input.txt
// ============================================================
"use strict";
const fs = require("fs");
const { performance } = require("perf_hooks");

class MinHeap {
  #arr;
  #maxSize;
  #size = 0;

  constructor(maxSize) {
    this.#maxSize = maxSize;
    this.#arr = new Int32Array(maxSize);
  }

  #heapifyUp(cur) {
    const a = this.#arr;
    while (cur > 0) {
      const parent = (cur - 1) >> 1;
      if (a[parent] <= a[cur]) break;
      [a[parent], a[cur]] = [a[cur], a[parent]];
      cur = parent;
    }
  }

  #heapifyDown(cur) {
    const a = this.#arr, size = this.#size;
    for (;;) {
      const left = 2 * cur + 1;
      if (left >= size) break;
      const right = left + 1;
      const smallest = right < size && a[right] < a[left] ? right : left;
      if (a[cur] <= a[smallest]) break;
      [a[cur], a[smallest]] = [a[smallest], a[cur]];
      cur = smallest;
    }
  }

  isEmpty() { return this.#size <= 0; }
  getSize() { return this.#size; }
  peek() { return this.#size === 0 ? -1 : this.#arr[0]; }

  add(v) {
    if (this.#size >= this.#maxSize) { console.log("Heap da day!"); return; }
    this.#arr[this.#size++] = v;
    this.#heapifyUp(this.#size - 1);
  }

  poll() {
    if (this.#size === 0) return -1;
    const root = this.#arr[0];
    this.#arr[0] = this.#arr[--this.#size];
    if (this.#size > 0) this.#heapifyDown(0);
    return root;
  }

  // Floyd build-heap, O(n)
  heapify(input, n) {
    if (n > this.#maxSize) { console.log("So luong phan tu vuot qua suc chua cua heap!"); return; }
    for (let i = 0; i < n; i++) this.#arr[i] = input[i];
    this.#size = n;
    for (let i = (n >> 1) - 1; i >= 0; i--) this.#heapifyDown(i);
  }

  removeAt(index) {
    if (index < 0 || index >= this.#size) return false;
    this.#arr[index] = this.#arr[--this.#size];
    if (index < this.#size) {
      if (index > 0 && this.#arr[index] < this.#arr[(index - 1) >> 1]) this.#heapifyUp(index);
      else this.#heapifyDown(index);
    }
    return true;
  }

  static heapSort(a, n) {
    const h = new MinHeap(n);
    h.heapify(a, n);
    for (let i = 0; i < n; i++) a[i] = h.poll();
  }

  isValid() {
    for (let i = 1; i < this.#size; i++)
      if (this.#arr[(i - 1) >> 1] > this.#arr[i]) return false;
    return true;
  }

  print(label) {
    console.log(`${label}: ${Array.from(this.#arr.subarray(0, this.#size)).join(" ")} `);
  }
}

// ---------- tien ich do thoi gian ----------
const timeMs = (f) => {
  const s = performance.now();
  f();
  return performance.now() - s;
};

const printRow = (name, ms, ops, note = "") =>
  console.log(`${name.padEnd(24)} ${ms.toFixed(4).padStart(10)} ms | ${(ms * 1e6 / ops).toFixed(2).padStart(10)} ns/lan | ${note}`);

// PRNG co seed (mulberry32) de ket qua lap lai duoc
const makeRng = (seed) => () => {
  seed = (seed + 0x6d2b79f5) | 0;
  let t = Math.imul(seed ^ (seed >>> 15), 1 | seed);
  t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
  return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
};

function main() {
  const MAXN = 100000;
  console.log(`=== TEST MIN HEAP VOI DATASET CUA BAN (toi da ${MAXN} phan tu) ===`);
  process.stdout.write("Nhap N: ");

  const tokens = fs.readFileSync(0, "utf8").split(/\s+/).filter(Boolean);
  const n = parseInt(tokens[0], 10);
  if (!Number.isInteger(n) || n <= 0 || n > MAXN) {
    console.log(`N phai nam trong khoang 1..${MAXN}`);
    return;
  }
  console.log(`Nhap ${n} so nguyen (cach nhau boi khoang trang hoac xuong dong):`);
  const data = new Int32Array(n);
  for (let i = 0; i < n; i++) {
    const v = parseInt(tokens[i + 1], 10);
    if (!Number.isInteger(v)) { console.log("Thieu du lieu!"); return; }
    data[i] = v;
  }
  console.log("\n" + "-".repeat(50));

  let sink = 0; // tranh bi toi uu hoa
  const rand = makeRng(42);
  const h = new MinHeap(n);

  // 1. add
  let t = timeMs(() => { for (const x of data) h.add(x); });
  printRow("add x N", t, n, h.isValid() ? "heap hop le: OK" : "LOI");
  if (n <= 20) h.print("   heap sau add");

  // 2. peek (lap 1e6 lan)
  const minTrue = data.reduce((m, x) => (x < m ? x : m), data[0]);
  const R = 1_000_000;
  t = timeMs(() => { for (let i = 0; i < R; i++) sink += h.peek(); });
  printRow("peek x 1e6", t, R, `peek = ${h.peek()}` + (h.peek() === minTrue ? " (dung min)" : " (SAI)"));

  // 3. removeAt ngau nhien (N/10 phan tu, it nhat 1)
  const K = Math.max(1, Math.floor(n / 10));
  t = timeMs(() => { for (let k = 0; k < K; k++) h.removeAt(Math.floor(rand() * h.getSize())); });
  printRow("removeAt x (N/10)", t, K, h.isValid() ? "heap hop le: OK" : "LOI");

  // 4. poll toan bo phan tu con lai, kiem tra tang dan
  const remain = h.getSize();
  let sorted = true;
  t = timeMs(() => {
    let prev = -Infinity;
    while (!h.isEmpty()) {
      const v = h.poll();
      if (v < prev) sorted = false;
      prev = v;
    }
  });
  printRow("poll x (con lai)", t, remain, sorted ? "thu tu tang: OK" : "LOI");

  // 5. heapify (Floyd, O(n))
  const h2 = new MinHeap(n);
  t = timeMs(() => h2.heapify(data, n));
  printRow("heapify (build heap)", t, n, h2.isValid() ? "heap hop le: OK" : "LOI");
  if (n <= 20) h2.print("   heap sau heapify");

  // 6. heapSort so voi TypedArray.sort
  const s1 = Int32Array.from(data), s2 = Int32Array.from(data);
  const th = timeMs(() => MinHeap.heapSort(s1, n));
  const ts = timeMs(() => s2.sort());
  const isSorted = s1.every((v, i) => i === 0 || s1[i - 1] <= v);
  printRow("heapSort", th, n, isSorted ? "da sap xep: OK" : "LOI");
  printRow("TypedArray.sort (so sanh)", ts, n);

  console.log("-".repeat(50));
  console.log("(ns/lan = thoi gian trung binh moi lan goi; heapify/heapSort chia deu cho N)");
  return sink;
}

main();
