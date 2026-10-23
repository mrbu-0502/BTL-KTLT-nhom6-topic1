# KỊCH BẢN DEMO NGƯỜI DÙNG

**Hệ thống quản lý thuê bao di động — Nhóm 06 — Đề tài 01**
Khoa Viễn thông 1 — PTIT · Học phần: Kỹ thuật lập trình (OOP C++)

| Thông tin | Nội dung |
| :--- | :--- |
| Thời lượng | khoảng 9 phút 30 giây (giới hạn video demo: 10 phút) |
| Hình thức | Quay màn hình console + thuyết minh, có thể chèn webcam góc nhỏ |
| Người nói | 1 bạn (có thể luân phiên theo phần phụ trách) |
| Người thao tác | 1 bạn, hoặc người nói tự thao tác |
| Dữ liệu | Bộ `data/` gốc của nhóm, đã sao lưu trước khi quay |
| Mạch kể | Một khách hàng mới đến làm SIM → dùng dịch vụ → nạp tiền → nhận hóa đơn → gửi khiếu nại |

> Ghi chú: giao diện chương trình dùng tiếng Việt không dấu. Lời thoại vẫn nói tiếng Việt bình thường; khi đọc kết quả trên màn hình, đọc theo nghĩa, không đọc từng ký tự.

---

## 1. Mục tiêu của buổi demo

- Đi qua đủ **10 use case**: Gói cước, Khách hàng, Loại hình thuê bao, Khiếu nại, Thiết bị, CDR, SIM, Hóa đơn, Nạp tiền, Hợp đồng.
- Cho thấy mỗi phân hệ có **thêm – xem – tìm – sửa – xóa** (riêng Khiếu nại dùng **đóng phiếu** thay cho xóa cứng).
- Chứng minh 4 điểm kỹ thuật bằng thao tác thật, không chỉ nói:
  1. Kiểm tra dữ liệu đầu vào, nhập sai không làm treo chương trình.
  2. Kiểm tra khóa ngoại và chặn xóa bản ghi đang được tham chiếu.
  3. Mã định danh không được sửa; nhiều mã được tự sinh.
  4. Dữ liệu được ghi xuống file ngay sau mỗi thay đổi và nạp lại khi khởi động.
- Kết thúc bằng việc mở file `data/*.txt` để chứng minh dữ liệu tồn tại bền vững.

---

## 2. Chuẩn bị trước khi quay

### 2.1. Build và chạy

Mở PowerShell tại thư mục gốc dự án:

- Build: `g++ -std=c++11 -I./include src/*.cpp -o app.exe`
- Chạy: `.\app.exe`

Lưu ý: phải chạy từ thư mục gốc vì chương trình dùng đường dẫn tương đối `data/...`. Nếu mở `app.exe` bằng cách double-click trong thư mục khác, chương trình sẽ không tìm thấy file dữ liệu.

### 2.2. Sao lưu và khôi phục dữ liệu

Trước khi quay, sao lưu nguyên thư mục `data`:

- Sao lưu: `Copy-Item -Path data -Destination data_backup -Recurse -Force`
- Khôi phục sau khi quay: `Copy-Item -Path data_backup\* -Destination data -Force`

Nên quay lại từ dữ liệu sạch cho mỗi lần thu. Không sửa tay file dữ liệu trong lúc quay.

### 2.3. Quy ước ngày trong kịch bản

Kịch bản dùng:

- **D = 01/10/2026** — ngày kích hoạt SIM, ngày ký hợp đồng.
- **D+4 = 05/10/2026** — ngày nạp tiền.
- **D+5 = 06/10/2026** — ngày gửi khiếu nại.
- **D+6 = 07/10/2026** — thời điểm cuộc gọi và ngày cập nhật khiếu nại.
- **M = 10/2026** — kỳ hóa đơn (tháng hiện tại).

Nếu hôm quay không phải tháng 10/2026, thay D bằng ngày quay, M bằng tháng hiện tại hoặc tháng trước. Các ràng buộc cần nhớ:

- Ngày kích hoạt SIM không được ở tương lai.
- Ngày ký hợp đồng không nhỏ hơn ngày kích hoạt SIM.
- Ngày nạp tiền và ngày gửi khiếu nại không nhỏ hơn ngày ký hợp đồng.
- Kỳ hóa đơn không vượt tháng hiện tại.

### 2.4. Phiếu dữ liệu sẽ nhập (in ra giấy để không quên)

| Bước | Đối tượng | Giá trị nhập | ID dự kiến |
| :---: | :--- | :--- | :---: |
| 1 | Gói cước | `PKG11` · `Demo Data 5G` · `135000` · `204800` · `1000` · `100` · `30` | PKG11 |
| 2 | Khách hàng | `KH11` · `Nguyen Van Demo` · `001203012345` · `Ha Noi` · `12/05/2003` | KH11 |
| 3 | Loại hình | `Tra Sau Demo` · `Thue bao tra sau demo cho khach moi` · `70000` | SUB12 (tự sinh) |
| 4 | Thiết bị | `860123456789011` · `Samsung Galaxy S24` · `Samsung` · `2024` · `Moi` | IMEI |
| 5 | SIM | `0981000011` · `452040000000011` · `SUB12` · `860123456789011` · `01/10/2026` · `HoatDong` | 0981000011 |
| 6 | Hợp đồng | `KH11` · `0981000011` · `PKG11` · `01/10/2026` | HD11 (tự sinh) |
| 7 | Nạp tiền | `0981000011` · `100000` · `05/10/2026` · `ChuyenKhoan` | RC11 (tự sinh) |
| 8 | Hóa đơn | `BILL11` · `0981000011` · `10/2026` · `275000` · `ChuaThanhToan` | BILL11 |
| 9 | CDR | `CDR11` · `0981000011` · `0912345678` · `07/10/2026 09:15:30` · `180` · `1080` | CDR11 |
| 10 | Khiếu nại | `HD11` · `06/10/2026` · `Mat song 5G tai khu vuc Cau Giay` · `TiepNhan` | KN16 (tự sinh) |

ID tự sinh phụ thuộc dữ liệu hiện có. Nếu máy hiện ID khác (HD12, KN17, RC12, SUB13…), dùng đúng ID đang hiện trên màn hình, không đọc theo kịch bản.

### 2.5. Không gian quay

- Cỡ chữ terminal 18–20, nền tối, cửa sổ rộng để bảng không bị xuống dòng.
- Đóng các cửa sổ không liên quan; bật chế độ không làm phiền.
- Chuẩn bị sẵn một cửa sổ PowerShell thứ hai để mở file dữ liệu ở cảnh cuối.
- Nói chậm, sau mỗi kết quả hiện ra thì dừng 1–2 giây cho người xem đọc.

---

## 3. Phân cảnh tổng thể

| Thời gian | Cảnh | Nội dung chính |
| :--- | :--- | :--- |
| 0:00 – 0:35 | 0 | Chào, giới thiệu nhóm, mạch câu chuyện |
| 0:35 – 1:10 | 1 | Khởi động, menu chính, thử nhập sai |
| 1:10 – 1:55 | 2 | Gói cước: thêm PKG11, xem danh sách |
| 1:55 – 2:35 | 3 | Khách hàng: thêm KH11, xem danh sách |
| 2:35 – 3:10 | 4 | Loại hình: thêm SUB12, thử xóa SUB01 bị chặn |
| 3:10 – 3:45 | 5 | Thiết bị: thêm IMEI mới |
| 3:45 – 4:35 | 6 | SIM: thêm SIM mới, tìm chi tiết |
| 4:35 – 5:55 | 7 | Hợp đồng: thêm HD11, chặn SIM đang dùng, chặn xóa HD01 |
| 5:55 – 6:30 | 8 | Nạp tiền: thêm RC11, xem danh sách |
| 6:30 – 7:05 | 9 | Hóa đơn: thêm BILL11, tìm theo SIM |
| 7:05 – 7:35 | 10 | CDR: thêm CDR11, xem danh sách |
| 7:35 – 8:55 | 11 | Khiếu nại: thêm, sửa, đóng, chặn sửa phiếu đã đóng |
| 8:55 – 9:30 | 12 | Thoát, mở file dữ liệu, kết luận |

---

## 4. Lời thoại chi tiết

### Cảnh 0 — Chào và giới thiệu (0:00 – 0:35)

**Màn hình:** cửa sổ PowerShell tại thư mục dự án, có thể mở nhanh cây thư mục `include/`, `src/`, `data/`.

**Lời thoại:**

> Dạ, em xin chào thầy cô và các bạn. Em là Nguyễn Văn Cường, thành viên nhóm 06. Hôm nay nhóm em xin demo chương trình quản lý thuê bao di động viết bằng C++.

> Để buổi demo dễ theo dõi, nhóm em sẽ kể một câu chuyện: một khách hàng mới đến làm SIM, sau đó dùng dịch vụ, nạp tiền, nhận hóa đơn và gửi khiếu nại. Đi theo câu chuyện này, mình sẽ đi qua đủ 10 phân hệ.

> Cấu trúc dự án gồm ba phần: `include` là khai báo, `src` là cài đặt, còn `data` là 10 file text, mỗi file ứng với một đối tượng. Toàn bộ dữ liệu nằm trong file, không dùng cơ sở dữ liệu.

*Chuyển cảnh:* "Mình bắt đầu bằng việc chạy chương trình."

### Cảnh 1 — Khởi động, menu chính, thử nhập sai (0:35 – 1:10)

**Thao tác:**

1. Chạy `.\app.exe`.
2. Tại dòng `Nhap lua chon cua ban (0-10):`, gõ `abc` rồi Enter.
3. Gõ `1` rồi Enter để vào Gói cước.

**Lời thoại:**

> Mình chạy chương trình từ thư mục gốc, vì đường dẫn tới file dữ liệu là đường dẫn tương đối. Vừa mở lên, chương trình đã tự nạp toàn bộ dữ liệu trong thư mục `data` vào bộ nhớ.

> Menu chính có 10 lựa chọn, tương ứng 10 use case; mỗi bạn trong nhóm phụ trách 2 use case.

> Mình thử nhập chữ vào chỗ chọn menu… Các bạn thấy đấy, chương trình báo phải nhập số nguyên và cho nhập lại. Nó không bị treo, cũng không thoát ra. Đây là phần `InputHelper` – module nhập liệu an toàn.

> Giờ mình vào Gói cước.

**Điểm nhấn:** nhập sai kiểu dữ liệu không làm chương trình dừng đột ngột.

### Cảnh 2 — Gói cước (1:10 – 1:55)

**Thao tác:**

1. `1` — Thêm gói cước mới.
2. Nhập lần lượt: `PKG11`, `Demo Data 5G`, `135000`, `204800`, `1000`, `100`, `30`.
3. `2` — Xem danh sách gói cước.
4. `0` — Quay lại menu chính.

**Lời thoại:**

> Đầu tiên là gói cước. Mình thêm gói `PKG11`. Mã gói phải là duy nhất; nếu nhập lại `PKG01`, chương trình sẽ báo mã đã tồn tại và dừng thao tác.

> Các con số như giá cước, dung lượng, số phút, số SMS và chu kỳ đều được kiểm tra trong setter: không được âm, chu kỳ phải lớn hơn 0. Nếu nhập sai, chương trình báo lỗi và không thêm bản ghi.

> Mình xem lại danh sách… đây là bảng căn cột, gói mới nằm ở cuối. Các bạn để ý là không có nút Lưu: vì `Repository` tự ghi xuống file ngay sau khi thêm thành công.

*Chuyển cảnh:* "Tiếp theo mình tạo khách hàng cho SIM sắp làm."

### Cảnh 3 — Khách hàng (1:55 – 2:35)

**Thao tác:**

1. `2` — Quản lý Khách hàng.
2. `1` — Thêm khách hàng mới.
3. Nhập: `KH11`, `Nguyen Van Demo`, `001203012345`, `Ha Noi`, `12/05/2003`.
4. `2` — Xem danh sách khách hàng.
5. `0` — Quay lại menu chính.

**Lời thoại:**

> Mình nhập mã `KH11`, tên, CCCD, địa chỉ và ngày sinh. CCCD phải đúng 12 chữ số; ngày sinh phải đúng định dạng ngày/tháng/năm và không vượt quá năm 2026.

> Nếu CCCD thiếu số hoặc ngày sinh sai định dạng, chương trình báo lỗi ngay và không thêm vào danh sách. Đây là kiểm tra dữ liệu ở tầng setter, không phải kiểm tra hình thức bên ngoài.

> Danh sách hiện có 11 khách hàng, `KH11` nằm ở cuối.

### Cảnh 4 — Loại hình thuê bao (2:35 – 3:10)

**Thao tác:**

1. `3` — Quản lý Loại hình thuê bao.
2. `2` — Thêm mới loại hình.
3. Nhập: `Tra Sau Demo`, `Thue bao tra sau demo cho khach moi`, `70000`.
4. `1` — Xem danh sách loại hình.
5. `5` — Xóa loại hình; nhập `SUB01`.
6. `0` — Quay lại menu chính.

**Lời thoại:**

> Loại hình thuê bao thì mã không nhập tay. Hệ thống tự tìm mã trống nhỏ nhất, nên lần này là `SUB12`. Mình đặt tên, mô tả và cước tháng 70.000.

> Mình thử xóa `SUB01`… bị chặn: loại hình này vẫn còn SIM đang sử dụng. Đây là kiểm tra ràng buộc tham chiếu, để tránh dữ liệu mồ côi: SIM không thể trỏ tới một loại hình đã bị xóa.

*Chuyển cảnh:* "Giờ mình thêm chiếc điện thoại sẽ gắn với SIM."

### Cảnh 5 — Thiết bị (3:10 – 3:45)

**Thao tác:**

1. `5` — Quản lý Thiết bị.
2. `2` — Thêm thiết bị mới.
3. Nhập: `860123456789011`, `Samsung Galaxy S24`, `Samsung`, `2024`, `Moi`.
4. `1` — Xem danh sách thiết bị.
5. `0` — Quay lại menu chính.

**Lời thoại:**

> Thiết bị được quản lý theo IMEI. IMEI phải đúng 15 chữ số, tên máy và hãng không được để trống, năm sản xuất trong khoảng 1990 đến 2026, tình trạng chỉ nhận `Moi` hoặc `Cu`.

> Thiết bị này sẽ được gán cho SIM ngay sau đây, nên mình chưa xóa gì cả.

### Cảnh 6 — SIM (3:45 – 4:35)

**Thao tác:**

1. `7` — Quản lý SIM.
2. `1` — Thêm SIM mới.
3. Nhập: `0981000011`, `452040000000011`, `SUB12`, `860123456789011`, `01/10/2026`, `HoatDong`.
4. `3` — Tìm kiếm SIM; chọn `1` — tìm chính xác theo số SIM; nhập `0981000011`.
5. `0` — Quay lại menu chính.

**Lời thoại:**

> SIM là trung tâm của hệ thống, vì hợp đồng, hóa đơn, cuộc gọi và nạp tiền đều trỏ về số SIM.

> Số SIM phải gồm 10 chữ số và bắt đầu bằng 0. Mã loại hình phải tồn tại trong `sub_types`, IMEI phải tồn tại trong `devices` – đây là kiểm tra khóa ngoại. Ngày kích hoạt không được ở tương lai.

> Mình tìm thử theo số SIM để xem chi tiết… đúng thông tin vừa nhập, kèm tên loại hình và tên máy được tra từ hai file danh mục.

*Chuyển cảnh:* "Bây giờ là phần liên kết nhiều đối tượng nhất: hợp đồng."

### Cảnh 7 — Hợp đồng (4:35 – 5:55)

**Thao tác:**

1. `10` — Quản lý Hợp đồng.
2. `2` — Thêm hợp đồng mới.
3. Mã hợp đồng tự sinh: `HD11`.
4. Nhập mã khách hàng: `KH11`.
5. Cố tình nhập số SIM: `0981000001` → bị chặn.
6. Nhập lại số SIM: `0981000011`.
7. Nhập mã gói cước: `PKG11`.
8. Nhập ngày đăng ký: `01/10/2026`.
9. `3` — Tìm kiếm hợp đồng; nhập `KH11`.
10. `5` — Xóa hợp đồng; nhập `HD01` → bị chặn.
11. `0` — Quay lại menu chính.

**Lời thoại:**

> Hợp đồng là nơi liên kết bốn thứ: khách hàng, SIM, gói cước và ngày đăng ký. Mã hợp đồng tự sinh, lần này là `HD11`.

> Mình cố tình nhập số `0981000001`. Số này đang thuộc hợp đồng `HD01` của khách `KH01`, nên hệ thống chặn và nói rõ đang bị ai sử dụng. Đây là quy tắc một SIM chỉ thuộc một hợp đồng còn hiệu lực.

> Đổi sang SIM mới `0981000011` thì qua. Gói `PKG11`. Ngày đăng ký `01/10/2026`, đúng bằng ngày kích hoạt SIM – hợp lệ vì chỉ cấm nhỏ hơn ngày kích hoạt.

> Mình tìm theo mã khách hàng `KH11` để xem hợp đồng vừa tạo.

> Thử xóa `HD01`… bị chặn vì hợp đồng đang ở trạng thái `HieuLuc`. Muốn xóa phải chuyển sang `DaHuy` hoặc `HetHan` trước. Thêm nữa, nếu hợp đồng đã có phiếu khiếu nại thì cũng không cho xóa. Mình giữ nguyên để lát nữa tạo khiếu nại.

**Điểm nhấn:** vừa kiểm tra khóa ngoại khi thêm, vừa kiểm tra ràng buộc khi xóa.

### Cảnh 8 — Nạp tiền (5:55 – 6:30)

**Thao tác:**

1. `9` — Quản lý Nạp tiền.
2. `2` — Thêm phiếu nạp tiền.
3. Mã phiếu tự sinh: `RC11`.
4. Nhập: `0981000011`, `100000`, `05/10/2026`, `ChuyenKhoan`.
5. `1` — Xem danh sách phiếu nạp.
6. `0` — Quay lại menu chính.

**Lời thoại:**

> Phiếu nạp tiền phải gắn với SIM đang có hợp đồng hiệu lực. Nếu SIM chưa có hợp đồng, hoặc hợp đồng đã bị hủy, chương trình không cho nạp.

> Ngày nạp cũng không được trước ngày ký hợp đồng. Mình nhập `05/10/2026`, sau ngày ký `01/10/2026`, nên hợp lệ.

> Mệnh giá phải lớn hơn 0, hình thức chỉ nhận `TienMat`, `ChuyenKhoan` hoặc `ViDienTu`.

### Cảnh 9 — Hóa đơn (6:30 – 7:05)

**Thao tác:**

1. `8` — Quản lý Hóa đơn.
2. `1` — Thêm hóa đơn mới.
3. Nhập: `BILL11`, `0981000011`, `10/2026`, `275000`, `ChuaThanhToan`.
4. `3` — Tìm kiếm hóa đơn; chọn `2` — theo số SIM; nhập `0981000011`.
5. `0` — Quay lại menu chính.

**Lời thoại:**

> Mỗi SIM chỉ được có một hóa đơn trong một tháng. Nếu nhập trùng kỳ, chương trình báo SIM đã có hóa đơn nào trong tháng đó.

> Kỳ hóa đơn cũng không được vượt tháng hiện tại. Mình nhập kỳ `10/2026`, tổng tiền 275.000, trạng thái `ChuaThanhToan`.

> Mình tìm lại theo số SIM để kiểm tra hóa đơn vừa tạo.

### Cảnh 10 — Bản ghi cuộc gọi CDR (7:05 – 7:35)

**Thao tác:**

1. `6` — Quản lý Cuộc gọi.
2. `2` — Thêm bản ghi cuộc gọi mới.
3. Nhập: `CDR11`, `0981000011`, `0912345678`, `07/10/2026 09:15:30`, `180`, `1080`.
4. `1` — Xem danh sách cuộc gọi.
5. `0` — Quay lại menu chính.

**Lời thoại:**

> Bản ghi cuộc gọi yêu cầu số gọi phải có trong kho SIM, số nhận đúng 10 chữ số và khác số gọi, thời điểm đúng định dạng ngày giờ, thời lượng lớn hơn 0.

> Mình nhập cuộc gọi 180 giây, cước 1.080 đồng.

### Cảnh 11 — Khiếu nại (7:35 – 8:55)

**Thao tác:**

1. `4` — Quản lý Khiếu nại.
2. `2` — Thêm mới phiếu khiếu nại.
3. Nhập mã hợp đồng: `HD11`.
4. Cố tình nhập ngày: `01/09/2026` → bị chặn.
5. Nhập lại ngày: `06/10/2026`.
6. Nhập nội dung: `Mat song 5G tai khu vuc Cau Giay`.
7. Nhập trạng thái: `TiepNhan`.
8. `3` — Tìm kiếm khiếu nại theo mã; nhập `KN16`.
9. `4` — Cập nhật khiếu nại; nhập `KN16`.
10. Tại mã hợp đồng mới: nhấn Enter để giữ nguyên.
11. Ngày gửi mới: `07/10/2026`.
12. Nội dung mới: nhấn Enter để giữ nguyên.
13. Trạng thái mới: `DangXuLy`.
14. `5` — Đóng phiếu khiếu nại; nhập `KN16`; xác nhận `y`.
15. `4` — Cập nhật lại `KN16` → bị chặn.
16. `0` — Quay lại menu chính.

**Lời thoại:**

> Cuối cùng là khiếu nại. Mã phiếu tự sinh, lần này là `KN16`. Mình nhập hợp đồng `HD11`.

> Mình thử nhập ngày `01/09/2026`, tức là trước ngày ký hợp đồng `01/10/2026`. Bị chặn đúng như mong đợi: khiếu nại không thể phát sinh trước khi có hợp đồng. Đổi sang `06/10/2026` thì qua.

> Trạng thái ban đầu là `TiepNhan`. Mình tìm `KN16` để xem lại, rồi cập nhật sang `DangXuLy`. Ở bước cập nhật, bỏ trống là giữ nguyên giá trị cũ; riêng ngày gửi phải nhập lại để kiểm tra hợp lệ.

> Xử lý xong thì mình chọn Đóng phiếu. Phiếu chuyển sang trạng thái `Dong`.

> Giờ thử cập nhật lại `KN16`… bị chặn: khiếu nại đã đóng thì không sửa được nữa.

> Phân hệ này cố tình không có xóa cứng. Nhóm em dùng đóng phiếu để giữ lịch sử chăm sóc khách hàng, đúng nghiệp vụ viễn thông: phiếu đã xử lý vẫn phải lưu lại để tra cứu.

**Điểm nhấn:** ràng buộc thời gian, trạng thái và tính bất biến của phiếu đã đóng.

### Cảnh 12 — Lưu bền vững và kết luận (8:55 – 9:30)

**Thao tác:**

1. `0` — Thoát chương trình.
2. Chuyển sang cửa sổ PowerShell thứ hai, chạy:
   - `Get-Content data\contracts.txt -Tail 3`
   - `Get-Content data\complaints.txt -Tail 3`

**Lời thoại:**

> Mình thoát chương trình. Đây là lời cảm ơn của chương trình.

> Mình mở nhanh hai file dữ liệu. Các bạn thấy `HD11` và `KN16` nằm ở cuối, đúng định dạng mỗi dòng một bản ghi, các trường ngăn bằng dấu gạch đứng.

> Nghĩa là mọi thay đổi đã được ghi xuống file ngay khi thao tác thành công. Khi mở lại chương trình, `Repository` sẽ nạp lại đúng những gì mình vừa nhập.

> Như vậy nhóm em đã demo đủ 10 use case, có thêm – xem – tìm – sửa – xóa, có kiểm tra dữ liệu, kiểm tra khóa ngoại, chặn xóa bản ghi đang được tham chiếu và lưu file bền vững.

> Em xin cảm ơn thầy cô và các bạn đã theo dõi.

---

## 5. Câu chốt kỹ thuật và hỏi đáp nhanh

| Câu hỏi có thể gặp | Trả lời ngắn |
| :--- | :--- |
| Vì sao khiếu nại không có chức năng xóa? | Giữ lịch sử xử lý khách hàng. Phiếu được "đóng" thay vì xóa; phiếu đã đóng không sửa được. |
| Mã định danh có sửa được không? | Không. `Repository::update` chặn đổi ID; menu cập nhật không cho nhập lại mã. |
| Nếu file dữ liệu chưa tồn tại thì sao? | `Repository::loadFromFile` tự tạo file rỗng rồi tiếp tục chạy. |
| Nếu ghi file thất bại thì dữ liệu trên RAM có bị lệch không? | `add`, `update`, `remove` giữ bản cũ và khôi phục nếu `saveToFile` ném lỗi. |
| Đa hình thể hiện ở đâu? | `Entity` có các hàm thuần ảo `getInformation`, `display`, `toString`, `fromString`; các lớp con ghi đè; `Repository<T>` làm việc qua `getID()`. |
| Vì sao dùng `Repository<T>`? | Một cài đặt CRUD và đọc/ghi file dùng chung cho 10 đối tượng, tránh lặp code. |
| Chống nhập sai bằng cách nào? | `InputHelper` đọc cả dòng bằng `getline`, kiểm tra bằng `stringstream`; sai thì báo lỗi và lặp lại; ngoại lệ tự định nghĩa trong `Exceptions.h`. |
| Làm sao đảm bảo không xóa dữ liệu đang được dùng? | Kiểm tra tham chiếu trước khi xóa: SIM bị Bill/Contract/CDR/Recharge dùng; thiết bị bị SIM dùng; loại hình bị SIM dùng; hợp đồng đang hiệu lực hoặc đã có khiếu nại. |
| Ngày tháng được kiểm tra thế nào? | Dùng lớp `Date` dùng chung: kiểm tra định dạng, ngày hợp lệ, so sánh trước/sau theo nghiệp vụ. |
| Dữ liệu lưu ở đâu? | 10 file text trong `data/`, mỗi dòng một bản ghi, trường ngăn bằng `|`, ngày theo `DD/MM/YYYY`. |

---

## 6. Phương án dự phòng khi quay

- **ID tự sinh lệch so với kịch bản:** dùng đúng ID trên màn hình; không cố nhập ID cũ.
- **Dữ liệu đã có bản ghi demo:** khôi phục `data_backup` rồi quay lại, không sửa tay trong lúc quay.
- **Lỡ nhập sai làm mất dữ liệu đang nhập:** bình tĩnh nói "phần này em xin phép làm lại từ menu", quay lại menu và nhập lại; không cắt ghép quá nhiều.
- **Console lỗi font tiếng Việt:** giữ nguyên giao diện không dấu; lời thoại vẫn tiếng Việt.
- **Quá thời gian:** cắt phần thử lỗi ở cảnh 7 và cảnh 11, giữ đủ 10 phân hệ; không cắt cả phân hệ.
- **Quên thao tác:** dùng câu nối "Mình quay lại menu chính để kiểm tra lại dữ liệu vừa nhập" rồi tiếp tục.

---

## 7. Sau khi quay

1. Khôi phục dữ liệu gốc:
   `Copy-Item -Path data_backup\* -Destination data -Force`
2. Kiểm tra Git không phát sinh thay đổi ngoài ý muốn:
   `git status`
3. Kiểm tra video:
   - Thời lượng không quá 10 phút.
   - Âm thanh rõ, không có tiếng thông báo.
   - Chữ trên console đọc được ở chế độ 1080p.
   - Thấy rõ thông báo lỗi và thông báo thành công ở các cảnh trọng tâm.
4. Đặt tên video theo quy ước của nhóm, ví dụ `Nhom06_Demo_QuanLyThueBao.mp4`.

---

## 8. Bản luyện nhanh 5 phút (chỉ để tập, không dùng nộp chính thức)

| Thời gian | Nội dung |
| :--- | :--- |
| 0:00 – 0:25 | Chào, giới thiệu mạch demo |
| 0:25 – 0:45 | Chạy chương trình, menu, thử nhập `abc` |
| 0:45 – 3:50 | Đi nhanh 10 phân hệ: mỗi phân hệ thêm mới + xem danh sách, không thử lỗi |
| 3:50 – 4:20 | Khiếu nại: thêm, cập nhật, đóng phiếu |
| 4:20 – 4:50 | Thoát, mở file dữ liệu, kết luận |

> Bản này dùng để luyện nhịp và làm quen thao tác. Khi nộp, dùng bản đầy đủ ở Mục 4 để chứng minh đủ 10 use case và các ràng buộc nghiệp vụ.
