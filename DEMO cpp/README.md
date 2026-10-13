# So sánh thời gian chạy: BT, BST, AVL, Heapsort

Chương trình C++ cài đặt 4 cấu trúc dữ liệu và đo thời gian chạy của chúng trên cùng một tập dữ liệu.

| Cấu trúc | Mô tả | Chèn | Tìm kiếm | Xuất dãy tăng dần |
|---|---|---|---|---|
| **BT** | Cây nhị phân thường, chèn theo tầng, không có thứ tự | O(1) | O(n) | O(n log n) (gom phần tử rồi sắp xếp) |
| **BST** | Cây nhị phân tìm kiếm, không cân bằng | O(h), từ log n đến n | O(h) | O(n) (duyệt inorder) |
| **AVL** | BST tự cân bằng bằng các phép xoay LL, RR, LR, RL | O(log n) | O(log n) | O(n) (duyệt inorder) |
| **Heap** | Max-heap lưu trên mảng, sắp xếp bằng heapsort | O(log n) | O(n) | O(n log n) |

## Cấu trúc thư mục

```
ds_benchmark.cpp          # mã nguồn
bst_stable_5000.txt       # 5000 phần tử, thứ tự ngẫu nhiên   -> case ổn định của BST
bst_worst_5000.txt        # 5000 phần tử, tăng dần             -> case tệ nhất của BST
bst_stable_100000.txt     # 100000 phần tử, thứ tự ngẫu nhiên
bst_worst_100000.txt      # 100000 phần tử, tăng dần
results.csv               # kết quả, được tạo ra sau khi chạy
```

## Biên dịch

```bash
g++ -O2 -std=c++17 ds_benchmark.cpp -o ds_benchmark
```

## Chạy

**Chạy trên các file dữ liệu có sẵn**, truyền một hoặc nhiều file:

```bash
./ds_benchmark bst_stable_5000.txt bst_worst_5000.txt bst_stable_100000.txt bst_worst_100000.txt
```

**Tự sinh dữ liệu** (dãy ngẫu nhiên và dãy tăng dần, kích thước lấy theo `SIZES`):

```bash
./ds_benchmark
```

Trên Windows, chạy bằng `ds_benchmark.exe ...`.

## Định dạng file dữ liệu

Dòng đầu là số phần tử `n`. Sau đó là `n` số nguyên, cách nhau bởi khoảng trắng hoặc xuống dòng. Các giá trị không được trùng nhau, vì BST và AVL bỏ qua khóa trùng.

```
5
3
1
4
5
2
```

## Các tập dữ liệu

| File | Thứ tự | Chiều cao BST | Ý nghĩa |
|---|---|---|---|
| `bst_stable_5000.txt` | xáo trộn ngẫu nhiên | 29 | case ổn định, h ≈ O(log n) |
| `bst_worst_5000.txt` | tăng dần 1..n | 5000 | case tệ nhất, cây suy biến thành danh sách |
| `bst_stable_100000.txt` | xáo trộn ngẫu nhiên | 39 | case ổn định |
| `bst_worst_100000.txt` | tăng dần 1..n | 100000 | case tệ nhất |

Dãy tăng dần cũng là case tệ nhất cho bước xây heap: mỗi phần tử mới đều nổi lên tận gốc.

## Cách đo

Với mỗi tập dữ liệu, chương trình làm như sau:

1. Sinh `NUM_SEARCH` khóa tìm kiếm. Một nửa có trong dữ liệu, một nửa chắc chắn không có.
2. Với từng cấu trúc, đo 3 phép:
   - **insert**: chèn lần lượt n phần tử.
   - **search**: tìm tất cả các khóa tìm kiếm.
   - **sort**: xuất dãy tăng dần.
3. Lặp lại `REPEAT` lần và lấy trung bình. Đơn vị là mili-giây.
4. Kiểm tra kết quả: dãy xuất ra phải đúng thứ tự, số khóa tìm thấy phải đúng. Nếu sai, chương trình dừng và báo lỗi.
5. In bảng ra màn hình và ghi vào `results.csv` với các cột `data, n, structure, insert_ms, search_ms, sort_ms`.

## Cấu hình

Các hằng số ở đầu file `ds_benchmark.cpp`:

| Hằng số | Mặc định | Ý nghĩa |
|---|---|---|
| `SIZES` | {5000, 10000, 20000, 40000} | kích thước n khi tự sinh dữ liệu |
| `NUM_SEARCH` | 1000 | số khóa tìm kiếm |
| `REPEAT` | 3 | số lần lặp để lấy trung bình |
| `SEED` | 42 | seed ngẫu nhiên |
| `CSV_FILE` | `results.csv` | file lưu kết quả |

## Kết quả mẫu

Kết quả với n = 100000, biên dịch `-O2`, đơn vị ms:

| Dữ liệu | Cấu trúc | insert | search | sort | tổng |
|---|---|---|---|---|---|
| stable | BT | 2.8 | 227.5 | 7.7 | 238.0 |
| stable | BST | 26.6 | 0.1 | 3.0 | 29.7 |
| stable | AVL | 37.7 | 0.1 | 1.9 | 39.7 |
| stable | Heap | 2.7 | 27.4 | 6.3 | 36.4 |
| worst | BT | 2.1 | 234.2 | 4.2 | 240.6 |
| worst | BST | **25783.9** | 166.6 | 1.0 | **25951.5** |
| worst | AVL | 10.2 | 0.1 | 0.8 | 11.0 |
| worst | Heap | 2.2 | 26.9 | 4.4 | 33.5 |

**Nhận xét:**
- Với dữ liệu ngẫu nhiên, BST nhanh nhất vì không tốn chi phí cân bằng. AVL chậm hơn một chút do các phép xoay.
- Với dữ liệu tăng dần, BST suy biến: phép chèn tốn O(n²), chậm hơn AVL khoảng 2000 lần. AVL vẫn ổn định.
- BT và Heap tìm kiếm theo O(n), nên chậm hơn hẳn ở phép search.
- Heapsort luôn là Θ(n log n), nên thời gian ít phụ thuộc vào thứ tự đầu vào.

## Lưu ý

- Chạy file `bst_worst_100000.txt` mất khoảng 25 giây cho mỗi lần chèn vào BST. Với `REPEAT = 3`, tổng thời gian khoảng 80 giây. Giảm `REPEAT` xuống 1 nếu cần chạy nhanh hơn.
- Phép sort của BT dùng `std::sort`, được tối ưu rất tốt. Vì vậy con số này không so sánh công bằng với cách xuất dãy của các cấu trúc khác.
- BST dùng vòng lặp thay cho đệ quy, và việc duyệt cây dùng stack, để tránh tràn stack khi cây suy biến với n = 100000.
- Thời gian thực tế phụ thuộc vào máy chạy. Khi so sánh, hãy chú ý đến tỉ lệ giữa các cấu trúc, không nên so con số tuyệt đối.
