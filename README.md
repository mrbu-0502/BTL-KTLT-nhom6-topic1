# DỰ ÁN BÀI TẬP LỚN: HỆ THỐNG QUẢN LÝ THUÊ BAO DI ĐỘNG
**Học phần:** Kỹ thuật lập trình (OOP C++) | **Nhóm:** 06 | **Đề tài:** 01 (Khoa Viễn thông 1 - PTIT)

---

## 🎯 MỤC TIÊU DỰ ÁN

- **Mục tiêu ưu tiên số 1:** Xây dựng code đơn giản, phân tách rõ ràng giữa khai báo (`include/`) và cài đặt (`src/`), dễ chia việc cho từng thành viên. Không sa đà vào kỹ thuật phức tạp để cả nhóm hoàn thành đúng hạn.
- **Yêu cầu cốt lõi bắt buộc phải đạt:**
  1. Đủ 10 Use Case cho 5 người (mỗi người 2 Use Case).
  2. Mỗi Use Case làm đủ 4 thao tác CRUD (Thêm - Xem - Sửa - Xóa) và Lưu/Đọc file text (`data/`).
  3. Cả 5 thành viên đều tự viết và commit phần module base của mình trên Git theo yêu cầu của giảng viên.
  4. Áp dụng OOP cơ bản: Có thuộc tính private, getter/setter, kế thừa từ lớp cơ sở chung (`Entity`), dùng template chung `Repository<T>`.
  5. Có menu console chọn chức năng, nhập liệu không bị văng chương trình khi nhập sai.

---

## 1. BẢNG PHÂN CÔNG NHIỆM VỤ (5 THÀNH VIÊN - 10 USE CASE)

Mỗi người phụ trách **1 module base dùng chung** và **2 Use Case** (1 danh mục dễ làm trước + 1 nghiệp vụ làm sau):

| STT | Họ và tên | Mã sinh viên | Module Base phụ trách | Use Case 1 (Làm trước) | Use Case 2 (Làm sau) | File code phụ trách | File dữ liệu (`data/`) |
| :---: | :--- | :---: | :--- | :--- | :--- | :--- | :--- |
| **1** | **Bùi Gia Khánh** | B24DCVT191 | `Entity.h` (Lớp trừu tượng) | **Gói Cước** | **Khách Hàng** | `include/Package.h`, `src/Package.cpp`<br>`include/Customer.h`, `src/Customer.cpp` | `packages.txt`<br>`customers.txt` |
| **2** | **Nguyễn Văn Cường** | B24DCVT053 | `InputHelper.h` (Nhập an toàn) | **Loại hình thuê bao** | **Phiếu khiếu nại** | `include/SubType.h`, `src/SubType.cpp`<br>`include/Complaint.h`, `src/Complaint.cpp` | `sub_types.txt`<br>`complaints.txt` |
| **3** | **Kiều Đức Hiệp** | *(Bổ sung)* | `Date.h` (Xử lý ngày tháng) | **Thiết bị (IMEI)** | **Bản ghi cuộc gọi (CDR)** | `include/Device.h`, `src/Device.cpp`<br>`include/CDR.h`, `src/CDR.cpp` | `devices.txt`<br>`cdrs.txt` |
| **4** | **Vũ Quang Anh** | *(Bổ sung)* | `Exceptions.h` (Lớp ngoại lệ) | **SIM số thuê bao** | **Hóa đơn cước** | `include/Sim.h`, `src/Sim.cpp`<br>`include/Bill.h`, `src/Bill.cpp` | `sims.txt`<br>`bills.txt` |
| **5** | **Lương Đức Anh** | B24DCVT006 | `Repository.h` (Template Repo)[cite: 2, 7] | **Phiếu nạp tiền** | **Hợp đồng đăng ký** | `include/Recharge.h`, `src/Recharge.cpp`<br>`include/Contract.h`, `src/Contract.cpp` | `recharges.txt`<br>`contracts.txt` |

---

## 2. CẤU TRÚC THƯ MỤC CHUẨN HÓA

Mã nguồn được phân tách rõ ràng: toàn bộ file khai báo (`.h`) đặt trong thư mục `include/`, toàn bộ file cài đặt thực thi (`.cpp`) đặt trong thư mục `src/`:

```text
BTL-KTLT-nhom6-topic1/
├── data/                          <-- Chứa 10 file text dữ liệu
│   ├── packages.txt
│   ├── customers.txt
│   ├── sub_types.txt
│   ├── complaints.txt
│   ├── devices.txt
│   ├── cdrs.txt
│   ├── sims.txt
│   ├── bills.txt
│   ├── recharges.txt
│   └── contracts.txt
│
├── include/                       <-- CHỨA TOÀN BỘ FILE KHAI BÁO (.h)
│   ├── Entity.h                   <-- (Khánh: Lớp cơ sở trừu tượng có hàm thuần ảo)
│   ├── InputHelper.h              <-- (Cường: Nhập chuỗi, nhập số an toàn, chống trôi lệnh)
│   ├── Date.h                     <-- (Hiệp: Xử lý và chuẩn hóa ngày tháng dd/mm/yyyy)
│   ├── Exceptions.h               <-- (Quang Anh: Các lớp ngoại lệ tự định nghĩa riêng)
│   ├── Repository.h               <-- (Đức Anh: Template chung lưu trữ và nạp/ghi file)
│   │
│   ├── Package.h                  (Khánh: Khai báo class Gói cước)
│   ├── Customer.h                 (Khánh: Khai báo class Khách hàng)
│   ├── SubType.h                  (Cường: Khai báo class Loại hình)
│   ├── Complaint.h                (Cường: Khai báo class Khiếu nại)
│   ├── Device.h                   (Hiệp: Khai báo class Thiết bị)
│   ├── CDR.h                      (Hiệp: Khai báo class Cuộc gọi)
│   ├── Sim.h                      (Quang Anh: Khai báo class SIM)
│   ├── Bill.h                     (Quang Anh: Khai báo class Hóa đơn)
│   ├── Recharge.h                 (Đức Anh: Khai báo class Nạp tiền)
│   └── Contract.h                 (Đức Anh: Khai báo class Hợp đồng)
│
├── src/                           <-- CHỨA CÁC FILE CÀI ĐẶT THỰC THI (.cpp)
│   ├── Package.cpp                (Khánh: Cài đặt chi tiết chức năng Gói cước)
│   ├── Customer.cpp               (Khánh: Cài đặt chi tiết chức năng Khách hàng)
│   ├── SubType.cpp                (Cường: Cài đặt chi tiết chức năng Loại hình)
│   ├── Complaint.cpp              (Cường: Cài đặt chi tiết chức năng Khiếu nại)
│   ├── Device.cpp                 (Hiệp: Cài đặt chi tiết chức năng Thiết bị)
│   ├── CDR.cpp                    (Hiệp: Cài đặt chi tiết chức năng Cuộc gọi)
│   ├── Sim.cpp                    (Quang Anh: Cài đặt chi tiết chức năng SIM)
│   ├── Bill.cpp                   (Quang Anh: Cài đặt chi tiết chức năng Hóa đơn)
│   ├── Recharge.cpp               (Đức Anh: Cài đặt chi tiết chức năng Nạp tiền)
│   ├── Contract.cpp               (Đức Anh: Cài đặt chi tiết chức năng Hợp đồng)
│   │
│   └── main.cpp                   <-- Menu chính điều phối hệ thống chung
│
├── docs/                          <-- Báo cáo Word/PDF và Slide thuyết trình
├── README.md                      <-- Hướng dẫn này
└── De_bai.md                      <-- Đề bài gốc
## 3. CÁCH TỔ CHỨC CODE TRONG TỪNG FILE CỦA MỖI BẠN

Để đảm bảo nguyên tắc tái sử dụng mã nguồn và tránh việc cả nhóm phải tự viết lặp lại các thao tác đọc/ghi file 10 lần:

1. **Phần 1: Lớp dữ liệu (Khai báo trong `include/*.h`, cài đặt trong `src/*.cpp`):**
   * Kế thừa công khai từ `Entity` (`: public Entity`).
   * Đặt toàn bộ thuộc tính ở phạm vi `private` hoặc `protected`.
   * Xây dựng constructor mặc định, constructor tham số cùng các getter/setter có kiểm tra hợp lệ dữ liệu[cite: 1, 7].
   * Ghi đè đầy đủ 3 phương thức thuần ảo bắt buộc từ `Entity`[cite: 1, 7]:
     * `getId() const override`: Trả về mã định danh duy nhất (khóa chính) của đối tượng[cite: 1, 7].
     * `toString() const override`: Chuyển đổi dữ liệu đối tượng thành chuỗi phân tách bởi dấu `|` để lưu file[cite: 1, 7].
     * `fromString(const std::string& line) override`: Bóc tách dòng văn bản đọc từ file `.txt` nạp ngược lại vào thuộc tính[cite: 1, 7].

2. **Phần 2: Xử lý nghiệp vụ CRUD và Menu con (Trong `src/*.cpp`):**
   * Khởi tạo và sử dụng trực tiếp template lưu trữ dùng chung: `Repository<TenLop> repo("data/ten_file.txt");`[cite: 1, 7].
   * Tái sử dụng các phương thức có sẵn của `Repository` (`add`, `getAll`, `findById`, `removeById`, `update`)[cite: 1, 7].
   * Phối hợp với `InputHelper` để nhập liệu an toàn, chống lỗi trôi lệnh và chống treo ứng dụng khi nhập sai kiểu[cite: 1, 7].
   * Xây dựng hàm `menu()` điều khiển con bằng vòng lặp `while` gồm các thao tác: 1. Xem danh sách, 2. Thêm mới, 3. Cập nhật, 4. Xóa, 0. Quay lại.

---

## 4. QUY CHUẨN ĐỊNH DẠNG DỮ LIỆU (`data/`)

Toàn bộ dữ liệu được lưu trữ bền vững trong các tệp văn bản (`.txt`), các trường thông tin phân tách bởi dấu gạch đứng `|`[cite: 1, 7]. Mỗi tệp phải có sẵn **tối thiểu 10 bản ghi mẫu hợp lệ** và đồng bộ chuẩn xác về các khóa ngoại tham chiếu[cite: 1, 7]:

1. `data/packages.txt`: `MaGoi|TenGoi|GiaCuoc|DataMB|ThoaiPhut|SMS|ChuKyNgay`
2. `data/customers.txt`: `MaKH|HoTen|CCCD|DiaChi|SDT|NgaySinh`
3. `data/sub_types.txt`: `MaLoai|TenLoai|MoTa|CuocThang`
4. `data/complaints.txt`: `MaKN|MaKH|NgayGui|NoiDung|TrangThai`
5. `data/devices.txt`: `MaIMEI|TenMay|HangSX|NamSX|TinhTrang`[cite: 7]
6. `data/cdrs.txt`: `MaCDR|SoGoi|SoNhan|ThoiDiem|SoGiay|CuocPhi`[cite: 7]
7. `data/sims.txt`: `SoSim|MaIMSI|MaLoai|MaIMEI|NgayKichHoat|TrangThai`[cite: 7]
8. `data/bills.txt`: `MaHD|SoSim|ThangNam|TongTien|TrangThai`[cite: 7]
9. `data/recharges.txt`: `MaPhieu|SoSim|MenhGia|NgayNap|HinhThuc`[cite: 7]
10. `data/contracts.txt`: `MaHD|MaKH|SoSim|MaGoi|NgayDangKy|TrangThai`[cite: 7]

---

## 5. LỘ TRÌNH THỰC HIỆN DỰ ÁN (6 TUẦN)

### 🟢 Tuần 1: Dựng khung móng dùng chung (Base) & Khởi tạo dự án
* Cả nhóm họp thống nhất cấu trúc thư mục `include/`, `src/` và `data/`[cite: 7].
* Cả 5 thành viên tự tạo nhánh cá nhân trên Git, tự code và commit 5 module base độc lập[cite: 2]:
  * Khánh: `include/Entity.h` (Lớp cơ sở trừu tượng có hàm thuần ảo)[cite: 2, 7]
  * Cường: `include/InputHelper.h` (Nhập số, nhập chuỗi an toàn, dọn bộ đệm)[cite: 2, 7]
  * Hiệp: `include/Date.h` (Xử lý, kiểm tra năm nhuận và chuẩn hóa `dd/mm/yyyy`)[cite: 2, 7]
  * Quang Anh: `include/Exceptions.h` (Lớp ngoại lệ tùy biến kế thừa `std::exception`)[cite: 2, 7]
  * Đức Anh: `include/Repository.h` (Template CRUD, đồng bộ file và quản lý `std::vector`)[cite: 2, 7]
* Tạo 10 file text rỗng trong thư mục `data/`[cite: 7].
* Kiểm tra thử nghiệm lệnh biên dịch liên kết chung: `g++ -Iinclude src/*.cpp -o test.exe`[cite: 7].

### 🟢 Tuần 2: Cài đặt 5 Use Case Danh mục (Dễ làm trước)
* Mỗi thành viên tập trung hoàn thiện Model và chức năng Thêm - Xem - Nạp/Ghi file[cite: 7]:
  * Khánh: Gói cước (`Package`)[cite: 7]
  * Cường: Loại hình thuê bao (`SubType`)[cite: 7]
  * Hiệp: Thiết bị IMEI (`Device`)[cite: 7]
  * Quang Anh: SIM số thuê bao (`Sim`)[cite: 7]
  * Đức Anh: Phiếu nạp tiền (`Recharge`)[cite: 7]
* **Mục tiêu hoàn thành tuần 2:** Chạy được menu riêng của từng use case, thêm mới dữ liệu và lưu bền vững vào file text[cite: 7].

### 🟢 Tuần 3: Cài đặt 5 Use Case Nghiệp vụ (Kiểm tra ràng buộc khóa ngoại)
* Cài đặt các thực thể có liên kết dữ liệu nghiệp vụ[cite: 1, 7]:
  * Khánh: Khách hàng (`Customer`)[cite: 7]
  * Cường: Phiếu khiếu nại (`Complaint` - kiểm tra `MaKH` tồn tại)[cite: 7]
  * Hiệp: Bản ghi cuộc gọi (`CDR` - kiểm tra `SoSim` tồn tại)[cite: 7]
  * Quang Anh: Hóa đơn cước (`Bill` - kiểm tra `SoSim` tồn tại)[cite: 7]
  * Đức Anh: Hợp đồng đăng ký (`Contract` - kiểm tra `MaKH`, `SoSim`, `MaGoi` tồn tại)[cite: 7]

### 🟢 Tuần 4: Bổ sung chức năng Sửa - Xóa & Nhập 10 bản ghi mẫu
* Hoàn thiện chức năng Sửa (tuyệt đối không sửa mã ID) và Xóa có xác nhận `(y/n)`[cite: 1, 7].
* Cài đặt kiểm tra toàn vẹn dữ liệu khi Xóa: Không cho phép xóa nếu bản ghi đang được thực thể khác tham chiếu[cite: 1, 7].
* Mỗi bạn nhập đủ **10 bản ghi mẫu hợp lệ** vào file `.txt` tương ứng của mình trong `data/`[cite: 1, 7].

### 🟢 Tuần 5: Ghép Menu chính (`main.cpp`) & Kiểm thử toàn diện
* Tích hợp toàn bộ 10 menu con vào vòng lặp điều khiển chính trong `src/main.cpp`[cite: 7].
* Kiểm thử các ca lỗi: nhập chữ vào ô số, ngày tháng không hợp lệ, trùng mã định danh[cite: 1, 7].
* Kiểm tra giải phóng bộ nhớ và đảm bảo trình biên dịch không xuất hiện cảnh báo (warning)[cite: 1, 7].

### 🟢 Tuần 6: Viết báo cáo BTL, Làm Slide & Bảo vệ
* Viết báo cáo Word/PDF hoàn chỉnh (20 - 30 trang) đúng theo chuẩn PI 3.1, PI 3.2, PI 3.3[cite: 1, 7].
* Thiết kế slide thuyết trình tóm tắt (10 - 15 slide)[cite: 1, 7].
* Phân công nội dung thuyết trình, chuẩn bị kịch bản demo và bảo vệ vấn đáp trước giảng viên[cite: 1, 7].

---

## 6. QUY TẮC LÀM VIỆC ĐỂ XONG NHANH VÀ TRÁNH XUNG ĐỘT (GIT)

1. **Ai làm file người đó**: Bạn chỉ tạo và sửa file liên quan đến 2 use case của mình. Không chạm vào file của bạn khác.
2. **Luôn Pull trước khi Code**:
   ```bash
   git pull origin main
   ```
3. **Commit sau khi hoàn thành một chức năng**:
   ```bash
   git add .
   git commit -m "Khanh: xong chuc nang them xem goi cuoc"
   git push origin main
   ```
4. **Hàm Main**: Chỉ trưởng nhóm (hoặc người được chỉ định) sửa file `main.cpp` để nối các use case lại với nhau.

---

## 🚀 7. PHẦN MỞ RỘNG (DÀNH CHO THÀNH VIÊN KHÁ/GIỎI MUỐN LẤY ĐIỂM TỐI ĐA)

*Nếu bạn đã hoàn thành sớm các chức năng cơ bản trên và muốn nâng cấp bài làm để đạt điểm 9 - 10 hoặc lấy điểm cộng của giảng viên, bạn có thể áp dụng thêm các kỹ thuật sau:*

1. **Nạp chồng toán tử (Operator Overloading):**
   - Nạp chồng `operator<<` để in đối tượng ra màn hình dạng bảng hoặc ghi file.
   - Nạp chồng `operator>>` để nhập đối tượng từ bàn phím.
   - Nạp chồng `operator==` để so sánh 2 đối tượng theo mã định danh.
2. **Xử lý ngoại lệ nâng cao (Custom Exception):**
   - Tự viết lớp ngoại lệ riêng kế thừa từ `std::exception` (ví dụ `DuplicateIdException`, `NotFoundException`, `FileException`) và dùng `throw - try - catch` thay vì chỉ dùng `if-else` thông thường.
3. **Thuật toán STL & Biểu thức Lambda:**
   - Dùng `std::sort` kết hợp hàm lambda để sắp xếp danh sách linh hoạt (theo tên A-Z, theo giá tăng dần).
   - Dùng `std::find_if` với lambda để tìm kiếm gần đúng theo từ khóa.
4. **Template / Lớp khuôn mẫu:**
   - Tạo lớp `Repository<T>` dùng chung cho các thao tác đọc ghi file và CRUD nếu thành thạo cú pháp template.
5. **Con trỏ thông minh (Smart Pointers):**
   - Sử dụng `std::unique_ptr` hoặc `std::shared_ptr` để quản lý danh sách con trỏ đối tượng, thể hiện khả năng kiểm soát bộ nhớ an toàn không lo rò rỉ.
