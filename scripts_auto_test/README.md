# HƯỚNG DẪN BỘ SCRIPTS KIỂM THỬ TỰ ĐỘNG (BTL-KTLT NHÓM 6)

Hệ thống kiểm thử tự động dành cho 2 module chức năng do **Nguyễn Văn Cường** phụ trách:
- **Module 3:** Quản lý Loại hình thuê bao (`SubType`)
- **Module 4:** Quản lý Khiếu nại (`Complaint`)

---

## 1. Cấu trúc thư mục `scripts_auto_test`

```text
scripts_auto_test/
├── runner.py                  # Script chính thực thi kiểm thử bằng Python
├── run_tests.bat              # Script Batch tiện lợi chạy trên Windows
├── README.md                  # Tài liệu hướng dẫn sử dụng
├── testcase/                  # Thư mục chứa từng file testcase (.txt) riêng biệt
│   ├── TC_SUB_01_view_all.txt
│   ├── TC_SUB_02_add_success.txt
│   ├── ... (17 testcases SubType)
│   ├── TC_COM_01_view_all.txt
│   ├── TC_COM_02_add_success.txt
│   └── ... (18 testcases Complaint)
└── logs/                      # Thư mục lưu trữ log sau mỗi lượt chạy kiểm thử
    ├── test_run_YYYYMMDD_HHMMSS.log
    └── latest.log             # Luôn chứa kết quả của lần chạy gần nhất
```

---

## 2. Cách chạy kiểm thử

### Cách 1: Chạy trực tiếp qua file Batch (.bat)
Từ thư mục gốc dự án hoặc nháy đúp chuột vào file:
```cmd
.\scripts_auto_test\run_tests.bat
```

Lọc chỉ chạy testcase của SubType:
```cmd
.\scripts_auto_test\run_tests.bat --filter SUB
```

Lọc chỉ chạy testcase của Complaint:
```cmd
.\scripts_auto_test\run_tests.bat --filter COM
```

Lọc một testcase cụ thể:
```cmd
.\scripts_auto_test\run_tests.bat --filter TC_SUB_02
```

---

### Cách 2: Chạy thông qua lệnh Python
```bash
python scripts_auto_test/runner.py
```

Các tham số hỗ trợ:
- `--filter` hoặc `-f`: Lọc testcase theo tên (ví dụ: `python scripts_auto_test/runner.py -f SUB`).
- `--timeout` hoặc `-t`: Thời gian chờ tối đa cho mỗi testcase tính bằng giây (mặc định: `10`s) để phòng tránh vòng lặp vô hạn.
- `--no-preserve-data`: Tắt cơ chế tự động khôi phục dữ liệu ban đầu.

---

## 3. Cơ chế hoạt động & An toàn dữ liệu

1. **Tự động sao lưu & khôi phục (Data Isolation)**:
   - Trước khi chạy kiểm thử, script tự động backup các file trong thư mục `data/` (`sub_types.txt`, `complaints.txt`,...).
   - Trước mỗi testcase, dữ liệu được đồng bộ về trạng thái chuẩn để các thao tác Thêm / Sửa / Xóa ở testcase trước **không ảnh hưởng** đến testcase sau.
   - Sau khi kết thúc toàn bộ test suite, dữ liệu gốc được khôi phục nguyên vẹn.

2. **Chống treo / Infinite Loop**:
   - Sử dụng `subprocess.Popen` với bộ đếm timeout, nếu nhập sai dẫn đến vòng lặp vô tận, script sẽ tự động dừng sau 10 giây và ghi nhận trạng thái `TIMEOUT`.

3. **Ghi Log chi tiết**:
   - Mọi dữ liệu đầu vào (`stdin`), kết quả đầu ra (`stdout`), lỗi hệ thống (`stderr`), thời gian thực thi và lý do thất bại (nếu có) đều được ghi nhận đầy đủ vào thư mục `scripts_auto_test/logs/`.

---

## 4. Định dạng file Testcase (.txt)

Mỗi file trong thư mục `testcase/` có thể được cấu hình linh hoạt theo cú pháp:

```ini
[DESCRIPTION]
Mô tả mục đích của testcase

[EXPECTED]
Chuỗi 1 mong đợi xuất hiện trong kết quả console
Chuỗi 2 mong đợi xuất hiện trong kết quả console

[EXPECTED_NOT]
Chuỗi không được phép xuất hiện (tùy chọn)

[INPUT]
Dòng_nhập_1
Dòng_nhập_2
Dòng_nhập_3
```

*(Nếu file không chứa các thẻ section trên, script sẽ tự động coi toàn bộ file là các dòng nhập liệu `stdin`)*.

---

## 5. Hướng dẫn Biên dịch (Compilation Guide)

### 5.1. Biên dịch phân hệ SubType & Complaint (Khuyên dùng để chạy test)
Để chạy kiểm thử nhanh và độc lập mà không bị ảnh hưởng bởi lỗi chưa hoàn thiện từ các module khác của nhóm:

- **Lệnh PowerShell / CMD (từ thư mục gốc dự án):**
  ```bash
  g++ -std=c++11 -I./include src/main.cpp src/SubType.cpp src/Complaint.cpp src/Contract.cpp -o app.exe
  ```
  *(Trong đó `Contract.cpp` được liên kết để phục vụ việc kiểm tra ràng buộc khóa ngoại Mã Hợp Đồng `HDxx` trong module Complaint)*.

- **Biên dịch riêng từng file thành Object (.o):**
  ```bash
  # Biên dịch module SubType
  g++ -std=c++17 -I./include -c src/SubType.cpp

  # Biên dịch module Complaint
  g++ -std=c++17 -I./include -c src/Complaint.cpp

  # Biên dịch Contract
  g++ -std=c++17 -I./include -c src/Contract.cpp

  # Liên kết với main thành file thực thi
  g++ -std=c++17 -I./include src/main.cpp SubType.o Complaint.o Contract.o -o app.exe
  ```

---

### 5.2. Biên dịch toàn bộ dự án (Full Build)
Khi tất cả các thành viên trong nhóm hoàn tất tích hợp các module:

- **Lệnh g++ biên dịch tất cả file nguồn:**
  ```bash
  g++ -std=c++17 -I./include src/*.cpp -o app.exe
  ```

- **Sử dụng VS Code Build Task:**
  1. Mở dự án trong Visual Studio Code.
  2. Nhấn tổ hợp phím `Ctrl + Shift + B`.
  3. Chọn task **"Build BTL Project"** để tự động biên dịch toàn bộ mã nguồn theo cấu hình trong `.vscode/tasks.json`.

---

> [!NOTE]
> Sau khi biên dịch ra file [app.exe](file:///c:/Users/cuong/Desktop/BTL-KTLT-nhom6-topic1/app.exe) ở thư mục gốc, bạn chỉ cần chạy `.\scripts_auto_test\run_tests.bat` để bắt đầu kiểm thử tự động toàn bộ 35 testcase.

