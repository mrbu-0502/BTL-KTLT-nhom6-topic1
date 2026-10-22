# HỆ THỐNG QUẢN LÝ THUÊ BAO DI ĐỘNG

**Học phần:** Kỹ thuật lập trình (OOP C++) · **Nhóm:** 06 · **Đề tài:** 01 · **Khoa Viễn thông 1 - PTIT**

---

## 1. Giới thiệu

Dự án xây dựng ứng dụng console quản lý thuê bao di động bằng C++ hướng đối tượng, với 10 Use Case (mỗi Use Case đủ 4 thao tác CRUD và lưu/đọc file text). Mã nguồn được phân tách rõ ràng giữa khai báo (`include/`) và cài đặt (`src/`), dùng chung lớp cơ sở `Entity` và template `Repository<T>`.

---

## 2. Phân công nhiệm vụ

Mỗi thành viên phụ trách **1 module base dùng chung** và **2 Use Case**.

| STT | Họ và tên | Mã sinh viên | Module Base | Use Case 1 | Use Case 2 |
| :---: | :--- | :---: | :--- | :--- | :--- |
| 1 | Bùi Gia Khánh | B24DCVT191 | `Entity.h` (Lớp trừu tượng) | Gói cước | Khách hàng |
| 2 | Nguyễn Văn Cường | B24DCVT053 | `InputHelper.h` (Nhập an toàn) | Loại hình thuê bao | Phiếu khiếu nại |
| 3 | Kiều Đức Hiệp | *(Bổ sung)* | `Date.h` (Xử lý ngày tháng) | Thiết bị (IMEI) | Bản ghi cuộc gọi (CDR) |
| 4 | Vũ Quang Anh | *(Bổ sung)* | `Exceptions.h` (Lớp ngoại lệ) | SIM số thuê bao | Hóa đơn cước |
| 5 | Lương Đức Anh | B24DCVT006 | `Repository.h` (Template Repo) | Phiếu nạp tiền | Hợp đồng đăng ký |

### Chi tiết file phụ trách

| Thành viên | File code | File dữ liệu |
| :--- | :--- | :--- |
| Bùi Gia Khánh | `Package.h/.cpp`, `Customer.h/.cpp` | `packages.txt`, `customers.txt` |
| Nguyễn Văn Cường | `SubType.h/.cpp`, `Complaint.h/.cpp` | `sub_types.txt`, `complaints.txt` |
| Kiều Đức Hiệp | `Device.h/.cpp`, `CDR.h/.cpp` | `devices.txt`, `cdrs.txt` |
| Vũ Quang Anh | `Sim.h/.cpp`, `Bill.h/.cpp` | `sims.txt`, `bills.txt` |
| Lương Đức Anh | `Recharge.h/.cpp`, `Contract.h/.cpp` | `recharges.txt`, `contracts.txt` |

---

## 3. Cấu trúc thư mục

```text
BTL-KTLT-nhom6-topic1/
├── data/          # 10 file text dữ liệu (.txt)
├── include/       # Toàn bộ file khai báo (.h)
│   ├── Entity.h        InputHelper.h    Date.h
│   ├── Exceptions.h    Repository.h     (5 module base)
│   └── Package.h  Customer.h  SubType.h  Complaint.h  Device.h
│       CDR.h  Sim.h  Bill.h  Recharge.h  Contract.h
├── src/           # Toàn bộ file cài đặt (.cpp) + main.cpp
├── docs/          # Báo cáo và slide thuyết trình
├── README.md
└── De_bai.md      # Đề bài gốc
```

---

## 4. Quy chuẩn dữ liệu (`data/`)

Dữ liệu lưu trong file `.txt`, các trường phân tách bằng dấu `|`. Mỗi file có tối thiểu **10 bản ghi mẫu hợp lệ**, đồng bộ khóa ngoại. Ngày tháng thống nhất chuẩn **DD/MM/YYYY**.

| Use Case | File dữ liệu | Định dạng trường |
| :--- | :--- | :--- |
| Gói cước (`Package`) | `packages.txt` | `MaGoi\|TenGoi\|GiaCuoc\|DataMB\|ThoaiPhut\|SMS\|ChuKyNgay` |
| Khách hàng (`Customer`) | `customers.txt` | `MaKH\|HoTen\|CCCD\|DiaChi\|NgaySinh` |
| Loại hình thuê bao (`SubType`) | `sub_types.txt` | `MaLoai\|TenLoai\|MoTa\|CuocThang` |
| Phiếu khiếu nại (`Complaint`) | `complaints.txt` | `MaKN\|MaHD\|NgayGui\|NoiDung\|TrangThai` |
| Thiết bị (`Device`) | `devices.txt` | `MaIMEI\|TenMay\|HangSX\|NamSX\|TinhTrang` |
| Bản ghi cuộc gọi (`CDR`) | `cdrs.txt` | `MaCDR\|SoGoi\|SoNhan\|ThoiDiem\|SoGiay\|CuocPhi` |
| SIM (`Sim`) | `sims.txt` | `SoSim\|MaIMSI\|MaLoai\|MaIMEI\|NgayKichHoat\|TrangThai` |
| Hóa đơn cước (`Bill`) | `bills.txt` | `MaHoaDon\|SoSim\|ThangNam\|TongTien\|TrangThai` |
| Phiếu nạp tiền (`Recharge`) | `recharges.txt` | `MaPhieu\|SoSim\|MenhGia\|NgayNap\|HinhThuc` |
| Hợp đồng đăng ký (`Contract`) | `contracts.txt` | `MaHopDong\|MaKH\|SoSim\|MaGoi\|NgayDangKy\|TrangThai` |

> Ghi chú khóa ngoại: `Complaint.MaHD` tham chiếu `contracts.txt`; `Sim.MaLoai` → `sub_types.txt`, `Sim.MaIMEI` → `devices.txt`; `CDR.SoGoi/SoNhan`, `Bill.SoSim`, `Recharge.SoSim` → `sims.txt`; `Contract.MaKH` → `customers.txt`, `Contract.SoSim` → `sims.txt`, `Contract.MaGoi` → `packages.txt`.

---

## 5. Biên dịch & Chạy

```bash
g++ -std=c++11 -I./include src/*.cpp -o app.exe
(biên dịch theo phiên bản c++ 11)

./app.exe
```

---

## 6. Quy tắc làm việc với Git

1. **Ai làm file người đó** — chỉ tạo/sửa file thuộc 2 Use Case của mình.
2. **Luôn pull trước khi code:** `git pull origin main`
3. **Commit sau mỗi chức năng hoàn thành:**
   ```bash
   git add .
   git commit -m "Khanh: xong chuc nang them xem goi cuoc"
   git push origin main
   ```
4. **File `main.cpp`** chỉ do trưởng nhóm (hoặc người được chỉ định) chỉnh sửa để ghép các Use Case.
