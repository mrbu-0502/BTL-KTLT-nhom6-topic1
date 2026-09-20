**HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG**

**KHOA VIỄN THÔNG 1**

**ĐỀ BÀI BÀI TẬP LỚN**

**HỌC PHẦN: KỸ THUẬT LẬP TRÌNH**

*Xây dựng ứng dụng quản lý bằng C++ theo hướng đối tượng, lưu trữ dữ liệu trên file*

|  |  |
| --- | --- |
| **Hình thức** | Làm theo nhóm, mỗi nhóm 5 sinh viên |
| **Ngôn ngữ lập trình** | C++ |
| **Phương pháp** | Lập trình hướng đối tượng (OOP) |
| **Khối lượng công việc mỗi sinh viên** | 2 use case, mỗi use case là 1 chức năng quản lý CRUD (Thêm – Xem/Tìm – Sửa – Xóa) cho một đối tượng |
| **Lưu trữ dữ liệu** | File (văn bản hoặc nhị phân); không dùng hệ quản trị cơ sở dữ liệu |
| **Giao diện** | Console (dòng lệnh), điều khiển bằng menu |
| **Thời gian thực hiện** | Đến tuần 11 hoặc tuần 12 sẽ bắt đầu thuyết minh về bài làm. Giảng viên sẽ nhắc nhở trước 1 tuần. |

# 1. MỤC TIÊU

Bài tập lớn giúp sinh viên vận dụng tổng hợp kiến thức của học phần Kỹ thuật lập trình vào một sản phẩm phần mềm hoàn chỉnh có quy mô vừa phải, được phát triển theo nhóm, đồng thời giải quyết bài toán thuộc lĩnh vực điện tử viễn thông. Sau khi hoàn thành, sinh viên cần đạt được các kết quả sau:

* Phân tích một bài toán quản lý thực tế, xác định các đối tượng cần quản lý, thuộc tính và quan hệ giữa các đối tượng này.
* Thiết kế và cài đặt hệ thống lớp vận dụng đầy đủ các nguyên lý lập trình hướng đối tượng trong C++.
* Đọc và ghi dữ liệu có cấu trúc bằng file, đảm bảo dữ liệu được lưu bền vững giữa các lần chạy chương trình.
* Tổ chức mã nguồn nhiều file, kiểm tra dữ liệu đầu vào, xử lý lỗi và kiểm thử chương trình.
* Làm việc nhóm: phân chia công việc, thống nhất thiết kế chung, tích hợp và trình bày sản phẩm.

# 2. MÔ TẢ BÀI TOÁN VÀ YÊU CẦU CHỨC NĂNG

Mỗi nhóm chọn một đề tài trong danh sách ở Mục 4 (hoặc tự đề xuất đề tài tương đương) và xây dựng **một chương trình console duy nhất** để quản lý nghiệp vụ của đề tài đó. Chương trình có một menu chính, từ đó người dùng truy cập vào các phân hệ quản lý. Mỗi phân hệ tương ứng với một use case và do đúng một thành viên phụ trách.

Toàn bộ dữ liệu được lưu trong file: chương trình tự nạp dữ liệu khi khởi động và cập nhật file sau mỗi thao tác thay đổi, sao cho khi tắt rồi mở lại chương trình, dữ liệu vẫn được giữ nguyên.

| **Số thành viên** | **Số use case của nhóm** | **Ghi chú** |
| --- | --- | --- |
| 4 sinh viên (sau khi phân thành 5 sinh viên nếu bị thiếu) | 8 use case | Chọn 8 trong 10 đối tượng gợi ý của đề tài |
| 5 sinh viên | 10 use case | Thực hiện đủ 10 đối tượng gợi ý của đề tài |

*Bảng 1. Số lượng use case theo quy mô nhóm*

## 2.1. Yêu cầu của một use case quản lý CRUD

Trong đề bài này, một use case là toàn bộ chức năng quản lý một loại đối tượng (ví dụ: Quản lý Sách, Quản lý Độc giả). Mỗi use case phải cung cấp tối thiểu bốn nhóm thao tác sau:

| **Thao tác** | **Chức năng** | **Yêu cầu tối thiểu** |
| --- | --- | --- |
| C – Create | Thêm mới | Nhập dữ liệu từ bàn phím; kiểm tra hợp lệ từng trường; mã định danh không được trùng; ghi bản ghi mới vào file. |
| R – Read | Xem, tìm kiếm | Hiển thị toàn bộ danh sách dạng bảng căn cột; xem chi tiết theo mã. |
| U – Update | Cập nhật | Tìm theo mã; hiển thị thông tin hiện tại; cho phép sửa từng trường (bỏ trống nghĩa là giữ nguyên); không cho sửa mã; kiểm tra hợp lệ; ghi lại file. |
| D – Delete | Xóa | Tìm theo mã; hiển thị bản ghi; yêu cầu xác nhận (y/n); kiểm tra ràng buộc trước khi xóa; cập nhật file. |

*Bảng 2. Các thao tác bắt buộc của một use case CRUD*

## 2.2. Yêu cầu đối với mỗi đối tượng quản lý

* Có ít nhất **3 thuộc tính**, trong đó có một **mã định danh duy nhất** (ví dụ: S001, DG015).
* Hai thành viên trong nhóm không được quản lý cùng một đối tượng; mỗi thành viên phụ trách các đối tượng khác nhau.

## 2.3. Yêu cầu tích hợp ở mức nhóm

* Toàn bộ nhóm xây dựng **một chương trình duy nhất** với một hàm main và một menu chính dẫn tới tất cả các use case.
* Dùng chung lớp cơ sở, lớp khuôn mẫu và các lớp tiện ích (nhập liệu, kiểm tra ngày tháng, đọc ghi file); không để mỗi thành viên tự viết một kiểu riêng.
* Thống nhất định dạng file dữ liệu, quy ước đặt tên và phong cách giao diện trong toàn chương trình.

# 3. YÊU CẦU KỸ THUẬT

## 3.1. Lập trình hướng đối tượng

| **Kỹ thuật** | **Yêu cầu áp dụng** | **Mức độ** |
| --- | --- | --- |
| **Đóng gói** | Thuộc tính khai báo private hoặc protected; truy cập qua getter/setter, trong đó setter có kiểm tra dữ liệu hợp lệ. | Bắt buộc |
| **Kế thừa** | Mọi lớp đối tượng kế thừa từ một lớp cơ sở chung (ví dụ Entity). Khuyến khích kế thừa nhiều tầng khi hợp lý, ví dụ Person → Customer, Employee. | Bắt buộc |
| **Đa hình** | Có hàm ảo (virtual) được ghi đè (override) ở lớp dẫn xuất và được gọi thông qua con trỏ hoặc tham chiếu tới lớp cơ sở. | Tùy chọn |
| **Trừu tượng** | Có ít nhất một lớp trừu tượng chứa hàm thuần ảo (pure virtual). | Bắt buộc |
| **Xử lý ngoại lệ** | Dùng try/catch/throw cho lỗi nhập liệu và lỗi file; khuyến khích tự định nghĩa lớp ngoại lệ riêng. | Bắt buộc |
| **Template** | Lớp khuôn mẫu dùng chung cho các thao tác CRUD và lưu trữ, ví dụ Repository<T>. | Khuyến khích |
| **STL** | Sử dụng vector, map, các thuật toán sort, find\_if, remove\_if, biểu thức lambda. | Khuyến khích |
| **Quản lý bộ nhớ** | Nếu cấp phát động phải giải phóng đúng; khuyến khích dùng smart pointer (unique\_ptr, shared\_ptr). | Khuyến khích |

*Bảng 3. Yêu cầu áp dụng kỹ thuật lập trình hướng đối tượng*

## 3.2. Lưu trữ dữ liệu bằng file

* Mỗi đối tượng quản lý được lưu trong **một file riêng** đặt trong thư mục data/, ví dụ data/books.txt, data/readers.txt.
* Nhóm thống nhất chọn một trong hai hình thức: file văn bản (mỗi dòng một bản ghi, các trường ngăn cách bởi ký tự phân cách như dấu |) hoặc file nhị phân (.dat). Với file văn bản, phải xử lý được trường hợp dữ liệu chứa khoảng trắng.
* Dữ liệu được nạp toàn bộ khi chương trình khởi động và được ghi lại ngay sau mỗi thao tác thêm, sửa, xóa thành công.
* Nếu file chưa tồn tại, chương trình tự tạo file rỗng và tiếp tục chạy. Nếu gặp dòng sai định dạng, chương trình bỏ qua dòng đó, đưa ra cảnh báo và không bị dừng đột ngột.
* Chỉ sử dụng thư viện <fstream> của C++; không dùng hệ quản trị cơ sở dữ liệu (MySQL, SQLite…) hay thư viện serialize bên ngoài.
* Mỗi file dữ liệu mẫu nộp kèm sản phẩm có **ít nhất 10 bản ghi hợp lệ**.

Ví dụ định dạng file văn bản cho đối tượng Sách:

# data/books.txt

# MaSach|TenSach|MaTacGia|MaNXB|NamXB|GiaBia|SoLuong|NgayNhap

S001|Lap trinh C++ can ban|TG01|NXB02|2021|125000|10|15/03/2024

S002|Cau truc du lieu va giai thuat|TG03|NXB01|2019|98000|5|02/11/2023

S003|Nhap mon cong nghe phan mem|TG02|NXB02|2022|110000|7|20/01/2025

## 3.3. Tổ chức mã nguồn

* Chỉ sử dụng thư viện chuẩn C++; không dùng thư viện đồ họa hay thư viện bên ngoài.
* Quy ước đặt tên thống nhất; có chú thích cho mỗi lớp và mỗi hàm public; đầu mỗi file ghi rõ họ tên tác giả để xác định phần đóng góp của từng cá nhân.
* Khuyến khích dùng Git (GitHub/GitLab) để làm việc nhóm; lịch sử commit là một căn cứ đánh giá mức độ đóng góp.

## 3.4. Kiến trúc lớp gợi ý

Kiến trúc dưới đây chỉ mang tính gợi ý; nhóm có thể thiết kế khác nếu vẫn đáp ứng đầy đủ các yêu cầu ở Mục 3.1. Ý tưởng chính là tách riêng ba phần: dữ liệu của đối tượng (lớp thực thể), việc quản lý tập bản ghi và lưu file (Repository), và việc giao tiếp với người dùng (Menu).

| **Lớp** | **Vai trò** |
| --- | --- |
| **Entity (lớp trừu tượng)** | Lớp cơ sở của mọi đối tượng: chứa mã định danh và các hàm thuần ảo nhập, hiển thị, chuyển đổi sang/từ bản ghi trong file. |
| **Các lớp thực thể (Book, Reader, …)** | Kế thừa Entity; mỗi sinh viên cài đặt 2 lớp ứng với 2 use case của mình. |
| **Repository<T>** | Lớp khuôn mẫu quản lý danh sách bản ghi: thêm, tìm, sửa, xóa, nạp và lưu file. Dùng chung cho cả nhóm. |
| **XxxMenu** | Giao diện console của một use case: hiển thị menu con, nhận lựa chọn, gọi Repository tương ứng. |
| **InputHelper, Date, Validator** | Tiện ích dùng chung: nhập số an toàn, kiểm tra chuỗi rỗng, kiểm tra ngày tháng, định dạng bảng. |
| **Application** | Khởi tạo các Repository, liên kết các ràng buộc tham chiếu và hiển thị menu chính. |

*Bảng 4. Vai trò các lớp trong kiến trúc gợi ý*

## 3.5. Giao diện console

* Mọi chức năng được điều khiển bằng menu đánh số; luôn có lựa chọn quay lại menu trước và thoát chương trình.
* Nhập sai kiểu dữ liệu (ví dụ nhập chữ khi yêu cầu số) không làm chương trình bị treo hoặc lặp vô hạn; chương trình thông báo lỗi và cho nhập lại.
* Danh sách được hiển thị dạng bảng căn cột bằng thư viện <iomanip>.
* Có thể dùng tiếng Việt không dấu trên giao diện để tránh lỗi hiển thị ký tự trên một số cửa sổ console.

# 4. DANH SÁCH ĐỀ TÀI GỢI Ý

Dưới đây là các đề tài thuộc lĩnh vực điện tử viễn thông. Mỗi đề tài dưới đây có 10 đối tượng quản lý, mỗi đối tượng tương ứng với một use case CRUD. Nhóm 4 người chọn 8 đối tượng, nhóm 5 người thực hiện đủ 10 đối tượng. Nhóm được phép thay thế tối đa 2 đối tượng bằng đối tượng khác phù hợp với nghiệp vụ, hoặc tự đề xuất một đề tài mới, nhưng phải giải quyết vấn đề thuộc lĩnh vực điện tử viễn thông; mọi thay đổi phải được giảng viên duyệt trước khi thực hiện.

| **STT** | **Đề tài** | **Đối tượng quản lý gợi ý (mỗi đối tượng = 1 use case CRUD)** |
| --- | --- | --- |
| 1 | Quản lý thuê bao di động | Gói cước 1; Loại hình thuê bao (trả trước/trả sau); Khách hang 1; SIM – số thuê bao 1; Hợp đồng đăng ký 1; Thiết bị đầu cuối (IMEI) 1; Phiếu nạp tiền 1; Bản ghi cuộc gọi (CDR) 1; Hóa đơn cước 1; Phiếu khiếu nại |
| 2 | Quản lý hạ tầng trạm BTS | Nhà cung cấp thiết bị; Loại thiết bị; Khu vực/Tỉnh; Trạm BTS; Thiết bị lắp đặt; Anten và cấu hình phát; Kỹ thuật viên; Hợp đồng thuê vị trí; Phiếu bảo trì định kỳ; Nhật ký sự cố trạm |
| 3 | Quản lý dịch vụ Internet FTTH | Gói Internet; Tuyến cáp/Khu vực; Khách hàng; Hợp đồng thuê bao; Thiết bị ONT/Modem cấp phát; Cổng (port) trên OLT; Phiếu lắp đặt; Phiếu bảo hành – sửa chữa; Hóa đơn cước; Nhân viên kỹ thuật |
| 4 | Quản lý trung tâm CSKH (Call Center) | Nhóm dịch vụ; Kênh tiếp nhận; Mức SLA; Khách hàng; Điện thoại viên; Ca trực; Cuộc gọi tiếp nhận; Ticket yêu cầu; Phiếu chuyển xử lý; Phiếu đánh giá hài lòng |
| 5 | Quản lý giám sát mạng (NOC) | Loại sự cố; Mức nghiêm trọng; Phần tử mạng (node); Cảnh báo (alarm); Ticket sự cố; Kỹ sư trực; Lịch trực; Phiếu xử lý; Báo cáo nguyên nhân gốc (RCA); Cam kết khôi phục |
| 6 | Quản lý tài nguyên kho số và tần số | Loại tài nguyên; Dải số/Đầu số; Băng tần; Doanh nghiệp được cấp; Giấy phép sử dụng; Phiếu cấp phát; Phiếu thu hồi; Phí sử dụng; Trạm phát đăng ký; Biên bản kiểm tra |
| 7 | Quản lý dịch vụ truyền hình số/IPTV | Thể loại kênh; Kênh truyền hình; Gói kênh; Khách hàng; Set-top box; Hợp đồng; Lịch phát sóng; Nội dung VOD; Hóa đơn; Phiếu hỗ trợ kỹ thuật |
| 8 | Quản lý vật tư và dự án triển khai mạng | Danh mục vật tư; Vật tư (cáp, connector, splitter); Nhà cung cấp; Kho; Phiếu nhập kho; Phiếu xuất kho; Dự án triển khai; Đội thi công; Phiếu nghiệm thu; Hóa đơn thanh toán |
| 9 | Quản lý bưu cục và vận chuyển bưu phẩm | Loại dịch vụ; Bưu cục; Tuyến vận chuyển; Khách hàng gửi; Bưu phẩm; Phiếu gửi; Bưu tá; Bản ghi hành trình (tracking); Cước phí – hóa đơn; Phiếu khiếu nại |

*Bảng 5. Danh sách đề tài và đối tượng quản lý gợi ý*

Khi phân công, nhóm nên chia sao cho mỗi thành viên có một đối tượng "danh mục" (đơn giản, được tham chiếu nhiều, như Thể loại, Nhà xuất bản) và một đối tượng "nghiệp vụ" (phức tạp hơn, tham chiếu tới đối tượng khác, như Phiếu mượn, Hóa đơn). Cách chia này giúp khối lượng công việc giữa các thành viên cân bằng.

# 5. VÍ DỤ ĐẶC TẢ MỘT USE CASE

Phần này minh họa mức độ chi tiết mong đợi khi đặc tả một use case, lấy ví dụ use case **Quản lý Sách** trong đề tài Quản lý thư viện. Mỗi sinh viên đặc tả 2 use case của mình theo mẫu tương tự.

| **Thuộc tính** | **Kiểu dữ liệu** | **Ràng buộc** |
| --- | --- | --- |
| **Mã sách** | string | Dạng Sxxx (x là chữ số), duy nhất, không được sửa |
| **Tên sách** | string | Không rỗng, tối đa 100 ký tự |
| **Mã tác giả** | string | Phải tồn tại trong danh sách Tác giả (tham chiếu use case Quản lý tác giả) |
| **Mã NXB** | string | Phải tồn tại trong danh sách Nhà xuất bản |
| **Năm xuất bản** | int | Từ 1900 đến năm hiện tại |
| **Giá bìa** | double | Lớn hơn 0 |
| **Số lượng** | int | Lớn hơn hoặc bằng 0 |
| **Ngày nhập** | Date | Ngày hợp lệ, không sau ngày hiện tại |

*Bảng 6. Thuộc tính của đối tượng Sách*

| **Chức năng** | **Mô tả xử lý** |
| --- | --- |
| **1. Thêm sách** | Nhập lần lượt các trường; kiểm tra từng trường theo Bảng 6; báo lỗi và cho nhập lại nếu sai; kiểm tra mã chưa tồn tại; lưu vào data/books.txt. |
| **2. Xem danh sách** | Hiển thị bảng gồm Mã, Tên sách, Tên tác giả (tra từ mã), Năm XB, Giá, Số lượng; cuối bảng hiển thị tổng số đầu sách. |
| **3. Tìm kiếm** | Theo mã (chính xác); theo tên (gần đúng, không phân biệt hoa thường); theo khoảng năm xuất bản. |
| **4. Sắp xếp** | Theo tên A–Z; theo giá bìa tăng dần hoặc giảm dần. |
| **5. Cập nhật** | Nhập mã; hiển thị thông tin hiện tại; nhập giá trị mới cho từng trường, bỏ trống để giữ nguyên; kiểm tra hợp lệ; lưu file. |
| **6. Xóa** | Nhập mã; hiển thị thông tin; hỏi xác nhận; không cho xóa nếu sách còn nằm trong phiếu mượn chưa trả; lưu file. |

*Bảng 7. Các chức năng của use case Quản lý Sách*

Minh họa menu con của use case trên giao diện console:

============ QUAN LY SACH ============

1. Them sach moi

2. Xem danh sach sach

3. Tim kiem sach

4. Sap xep danh sach

5. Cap nhat thong tin sach

6. Xoa sach

0. Quay lai menu chinh

======================================

Nhap lua chon: \_

# 6. SẢN PHẨM PHẢI NỘP

| **Sản phẩm** | **Yêu cầu** |
| --- | --- |
| **Mã nguồn** | Toàn bộ mã nguồn biên dịch và chạy được. |
| **Dữ liệu mẫu** | Đầy đủ các file trong thư mục data/, mỗi file có ít nhất 10 bản ghi hợp lệ, dữ liệu tham chiếu nhất quán giữa các file. |
| **Báo cáo** | Định dạng .docx hoặc .pdf, từ 20 đến 30 trang, trình bày theo cấu trúc ở Mục 6.1. |
| **Slide thuyết trình** | Từ 10 đến 15 slide, dùng khi bảo vệ. |
| **Video demo (nếu giảng viên yêu cầu)** | Không quá 10 phút, trình bày đủ các use case. |

*Bảng 8. Danh mục sản phẩm nộp*

Toàn bộ sản phẩm được nén thành một file có tên **NhomXX\_TenDeTai.zip** (ví dụ Nhom03\_QuanLyThuVien.zip) và do trưởng nhóm nộp đúng hạn.

## 6.1. Cấu trúc báo cáo

Báo cáo được viết để đáp ứng với tiêu chí CLO 3 dưới đây:

![](data:image/png;base64...)

Do đó, cấu trúc báo cáo bao gồm các phần sau:

* **Bìa, mục lục**
* **Chương 1. Giới thiệu và phân tích yêu cầu:** mô tả bài toán và các chức năng thực hiện
* **Chương 2. Môi trường và công cụ lập trình (PI 3.1):** Nêu về môi trường phát triển và các công cụ lập trình sử dụng (IDE) và công cụ phối hợp theo nhóm (VD: Git hoặc công cụ khác), công cụ gỡ lỗi. Đồng thời bổ sung danh sách các thư viện chuẩn đã dùng và mục tiêu của thư viện đó.
* **Chương 3. Thiết kế giải pháp (PI 3.2):** Nêu giải pháp và lí do lựa chọn giải pháp đó, đồng thời cần tính đến phần rủi ro xuất hiện (Ví dụ: File dữ liệu chưa tồn tại lần chạy đầu, hoặc Xóa bản ghi đang được tham chiếu, ví dụ xóa gói cước khi còn hợp đồng), đồng thời nêu phương hướng giải quyết nếu rủi ro đó xuất hiện. Đề cập cơ hội phát triển hệ thống này, ví dụ: hệ thống có thể mở rộng theo hướng nào (thêm nhà mạng, thêm loại gói cước, xuất báo cáo thống kê, thay file bằng CSDL).
* **Chương 4. Kiến thức mới tự tìm hiểu và vận dụng (PI 3.3):** Ví dụ như Xử lí lỗi (sử dụng Ngoại lệ) hoặc nghiệp vụ mới của đề tài lựa chọn (Ví dụ: Cấu trúc bản ghi CDR trong hệ thống mạng), trình bày theo dạng: **học gì → nguồn ở đâu→ áp dụng ,** và nêu lợi ích của kiến thức mới đó trong bài**.**
* **Kết luận**.

Báo cáo được nộp qua Google Drive do giảng viên thiết lập với cách đặt tên như đã đề cập trong mục 6.0

# 8. TIÊU CHÍ ĐÁNH GIÁ

Điểm bài tập lớn của mỗi sinh viên gồm **điểm cá nhân (8 điểm)**, chấm dựa trên 2 use case do sinh viên đó phụ trách, và **điểm nhóm (2 điểm)**, chung cho mọi thành viên. Vì vậy các thành viên trong cùng một nhóm có thể nhận điểm khác nhau.

| **Nội dung** | **Tiêu chí** | **Điểm** |
| --- | --- | --- |
| **A. PHẦN CÁ NHÂN (5.5 điểm)** | | |
| **Use case thứ nhất** | Đủ và đúng các thao tác CRUD | 1.5 |
| **Use case thứ hai** | Đủ và đúng các thao tác CRUD | 1.5 |
| **Áp dụng OOP** | Đóng gói, kế thừa, đa hình, nạp chồng toán tử được áp dụng hợp lý trong các lớp của mình | 1 |
| **Kiểm tra dữ liệu và ngoại lệ** | Kiểm tra hợp lệ đầy đủ; chương trình không bị dừng đột ngột khi nhập sai | 1 |
| **Chất lượng mã nguồn** | Tách file đúng, đặt tên rõ ràng, có chú thích, không lặp mã. | 0,5 |
| **B. PHẦN NHÓM (4.5 điểm)** | | |
| **Báo cáo** | Đúng và đủ cấu trúc các phần như trình bày ở trên. | 4 |
| **Trình bày** | Slide rõ ràng, demo mạch lạc, đúng thời gian | 0,5 |

*Bảng 10. Thang điểm đánh giá*

## 8.1. Các trường hợp trừ điểm

* Chương trình không biên dịch được: phần chức năng tương ứng bị 0 điểm.
* Use case không lưu được dữ liệu vào file: mất điểm lưu trữ và chỉ được tối đa 50% điểm của use case đó.
* Sinh viên không giải thích được mã nguồn do mình nộp: điểm cá nhân bị giảm hoặc bị hủy tùy mức độ.
* Sao chép mã nguồn giữa các nhóm, hoặc lấy mã từ nguồn khác mà không ghi rõ nguồn: 0 điểm toàn bài đối với các bên liên quan.
* Nộp muộn: trừ 1 điểm cho mỗi ngày; sau 3 ngày không nhận bài.

# 9. QUY ĐỊNH CHUNG

* Mỗi nhóm bầu một trưởng nhóm làm đầu mối liên hệ với giảng viên, chịu trách nhiệm điều phối, tích hợp và nộp bài.
* Nếu có thành viên không tham gia, nhóm báo cáo giảng viên bằng văn bản kèm minh chứng; giảng viên sẽ xem xét đánh giá riêng thành viên đó.
* Mọi thắc mắc về đề bài được giải đáp trong giờ học hoặc qua kênh liên lạc chính thức của lớp.

# PHỤ LỤC A. MẪU BẢNG PHÂN CÔNG

Tên nhóm: …………………………………………………………………………

Đề tài: ………………………………………………………………………………

Lớp: ……………………… Giảng viên hướng dẫn: ……………………………

| **STT** | **Họ và tên** | **MSSV** | **Vai trò** | **Use case 1** | **Use case 2** | **Đóng góp (%)** |
| --- | --- | --- | --- | --- | --- | --- |
| 1 |  |  | Trưởng nhóm |  |  |  |
| 2 |  |  | Thành viên |  |  |  |
| 3 |  |  | Thành viên |  |  |  |
| 4 |  |  | Thành viên |  |  |  |
| 5 |  |  | Thành viên |  |  |  |

*Ghi chú: cột Đóng góp do cả nhóm thống nhất, tổng bằng 100%. Nhóm 4 người để trống dòng 5.*

# PHỤ LỤC B. MẪU ĐẶC TẢ USE CASE

| **Mục** | **Nội dung** |
| --- | --- |
| **Mã use case** | UC…… |
| **Tên use case** | Quản lý …………………… |
| **Sinh viên thực hiện** | Họ tên: ……………………… MSSV: …………… |
| **Lớp đối tượng (C++)** |  |
| **Lớp cơ sở kế thừa** |  |
| **Danh sách thuộc tính và ràng buộc** |  |
| **Tên file dữ liệu** | data/…………………… |
| **Định dạng một bản ghi** |  |
| **Tham chiếu tới (khóa ngoại)** |  |
| **Được tham chiếu bởi** |  |
| **Chức năng Thêm (C)** |  |
| **Chức năng Xem, tìm kiếm, sắp xếp (R)** |  |
| **Chức năng Cập nhật (U)** |  |
| **Chức năng Xóa (D)** |  |
| **Kỹ thuật OOP áp dụng** |  |

## Mẫu bảng test case

| **STT** | **Chức năng** | **Dữ liệu vào** | **Kết quả mong đợi** | **Kết quả thực tế** | **Đạt** |
| --- | --- | --- | --- | --- | --- |
| 1 | Thêm sách | Mã S001 (đã tồn tại) | Thông báo "Mã đã tồn tại" và yêu cầu nhập lại |  |  |
| 2 | Thêm sách | Ngày nhập 30/02/2024 | Thông báo ngày không hợp lệ |  |  |
| 3 | Xóa sách | Mã S002 (đang được mượn) | Không cho xóa, thông báo lý do |  |  |
| … |  |  |  |  |  |