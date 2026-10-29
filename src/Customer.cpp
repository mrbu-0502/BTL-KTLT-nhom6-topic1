/**
 * ==============================================================================
 * BÀI TẬP LỚN: HỆ THỐNG QUẢN LÝ THUÊ BAO DI ĐỘNG (NHÓM 06 - ĐỀ TÀI 01)
 * Học phần: Kỹ thuật lập trình
 * Người thực hiện: Bùi Gia Khánh
 * MSSV: B24DCVT191
 * Module phụ trách: Entity.h (Lớp cơ sở trừu tượng)
 * Use Case: Quản lý Khách hàng (Customer)
 * ==============================================================================
 */
#include "Customer.h"       // Header khai báo lớp Customer và CustomerMenu
#include "Contract.h"       // Khai báo lớp Contract (để kiểm tra khóa ngoại trước khi xóa khách)
#include "Date.h"           // Xử lý và kiểm tra ngày sinh hợp lệ (leap, valid, output)
#include "InputHelper.h"    // Tiện ích nhập xuất an toàn trên console
#include "Exceptions.h"     // Các lớp ngoại lệ tự định nghĩa
#include <iostream>         // Luồng xuất nhập cout, cin
#include <iomanip>          // Định dạng căn lề cột in bảng khách hàng
#include <string>           // Xử lý chuỗi string
#include <vector>           // Chứa danh sách dữ liệu trong Repository
#include <sstream>          // Tách/ghép chuỗi qua dấu '|'
#include <cctype>           // Dùng hàm isdigit() kiểm tra 12 số CCCD
#include <algorithm>        // Thuật toán sort và biểu thức lambda

using namespace std; 

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ CUSTOMER
// =====================================================================

Customer :: Customer()
    : Entity(""), FullName(""), CCCD(""), Address(""), DateOfBirth("") {}

Customer :: Customer(string ID, string name, string cccd, string address, string date)
    : Entity(ID) {
    setFullName(name);
    setCCCD(cccd);
    setAddress(address);
    setDateOfBirth(date);
}

string Customer :: getID() const {
    return ID;
}

string Customer :: toString() const {
    stringstream ss;
    ss << ID << "|" << FullName << "|" << CCCD << "|" 
       << Address << "|" << DateOfBirth;
    return ss.str();
}

void Customer :: fromString(const string& line) {
    if (line.empty()) return;
    stringstream ss(line);
    string tempID;
    getline(ss, tempID, '|');
    if (tempID == "MaKH" || tempID.empty()) {
        ID = "";
        return;
    }
    // [FIX LOI 4] Kiem tra du so cot truoc khi gan; dung setter de validate du lieu doc tu file.
    string tempName, tempCCCD, tempAddress, tempDOB;
    if (!getline(ss, tempName,    '|')) return;
    if (!getline(ss, tempCCCD,    '|')) return;
    if (!getline(ss, tempAddress, '|')) return;
    if (!getline(ss, tempDOB))          return;
    try {
        ID = tempID;
        setFullName(tempName);
        setCCCD(tempCCCD);
        setAddress(tempAddress);
        setDateOfBirth(tempDOB);
    } catch (...) {
        // Du lieu file khong hop le: dat ID rong de Repository bo qua ban ghi nay
        ID = "";
    }
}

void Customer :: getInformation() {
    ID = InputHelper :: getString("Nhap ma khach hang: ", false);
    setFullName(InputHelper :: getString("Nhap ten khach hang: ", false));
    setCCCD(InputHelper :: getString("Nhap can cuoc cong dan cua khach hang: ", false));
    setAddress(InputHelper :: getString("Nhap dia chi khach hang: ", false));
    setDateOfBirth(InputHelper :: getString("Nhap ngay sinh cua khach hang: ", false));
}

void Customer :: display() const {
    cout << left << setw(10) << ID 
         << setw(22) << FullName 
         << setw(14) << CCCD 
         << setw(18) << Address 
         << setw(14) << DateOfBirth << "\n";
}

string Customer :: getFullName() const{
    return FullName; 
}
string Customer :: getCCCD() const{
    return CCCD; 
}
string Customer :: getAddress() const{
    return Address;
}
string Customer :: getDateOfBirth() const{
    return DateOfBirth;
}

void Customer :: setFullName(const string& n) {
    if (n.empty()) {
        throw InvalidDataException("Ten khach hang khong duoc de trong!");
    }
    this->FullName = n;
}

void Customer :: setCCCD(const string& c) {
    if (c.empty()) {
        throw InvalidDataException("CCCD khong duoc de trong!");
    }
    if (c.length() != 12) {
        throw InvalidDataException("CCCD phai dung 12 chu so!");
    }
    for (char ch : c) {
        if (!isdigit(ch)) {
            throw InvalidDataException("Can cuoc cong dan khong hop le (chi gom cac chu so)!");
        }
    }
    this->CCCD = c;
}

void Customer :: setAddress(const string& a) {
    if (a.empty()) {
        throw InvalidDataException("Dia chi khong duoc de trong!");
    }
    if (a.find('|') != string::npos) {
        throw InvalidDataException("Dia chi khong duoc chua ky tu dac biet '|'!");
    }
    this->Address = a;
}

void Customer :: setDateOfBirth(const string& dStr) {
    if (dStr.empty()) {
        throw InvalidDataException("Ngay sinh khong duoc de trong!");
    }
    int ngay, thang, nam;
    char c1 = 0, c2 = 0;
    stringstream ss(dStr);
    if (!(ss >> ngay >> c1 >> thang >> c2 >> nam) || c1 != '/' || c2 != '/') {
        throw InvalidDataException("Ngay sinh phai dung dinh dang dd/mm/yyyy!");
    }
    Date d(ngay, thang, nam);
    if (!d.valid()) {
        throw InvalidDataException("Ngay sinh khong hop le hoac vuot qua nam 2026!");
    }
    this->DateOfBirth = d.output();
}

// =====================================================================
// PHẦN 3: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA CUSTOMERMENU
// =====================================================================

void CustomerMenu :: printHeader() {
    cout << string(78, '-') << "\n";
    cout << left  << setw(10) << "Ma KH"
         << setw(22) << "HO TEN"
         << setw(14) << "CCCD"
         << setw(18) << "DIA CHI"
         << setw(14) << "NGAY SINH" << "\n";
    cout << string(78, '-') << "\n";
}

// ============================================================================
// CHỨC NĂNG 1: THÊM MỚI KHÁCH HÀNG (CREATE) 
// ============================================================================
void CustomerMenu::handleCreate(Repository<Customer>& repo) {
    cout << "\n---------------- THEM KHACH HANG MOI ----------------\n";

    // 1. Nhập Tên khách hàng
    string name;
    while (true) {
        name = InputHelper::getString("Nhap ten khach hang moi (nhap 0 de thoat): ", false);
        if (name == "0") {
            cout << ">> Da huy thao tac them moi khach hang.\n";
            return;
        }
        if (name.find('|') != string::npos) {
            cout << ">> [LOI] Ten khong duoc chua ky tu '|'. Vui long nhap lai!\n";
            continue;
        }
        break;
    }

    // 2. Nhập CCCD: Kiểm tra cả độ dài, định dạng số VÀ tính duy nhất ngay tại chỗ
    string CCCD;
    while (true) {
        CCCD = InputHelper::getString("Nhap CCCD cua khach hang moi (12 chu so, nhap 0 de thoat): ", false);
        if (CCCD == "0") {
            cout << ">> Da huy thao tac them moi khach hang.\n";
            return;
        }

        // Kiểm tra độ dài và ký tự số
        if (CCCD.length() != 12) {
            cout << ">> [LOI] CCCD phai co dung 12 chu so! Vui long nhap lai.\n";
            continue;
        }
        bool isNumber = true;
        for (char ch : CCCD) {
            if (!isdigit(ch)) {
                isNumber = false;
                break;
            }
        }
        if (!isNumber) {
            cout << ">> [LOI] CCCD chi duoc chua cac chu so! Vui long nhap lai.\n";
            continue;
        }

        // Kiểm tra trùng lặp
        bool isDuplicate = false;
        for (const auto& cus : repo.getAll()) {
            if (cus.getCCCD() == CCCD) {
                isDuplicate = true;
                break;
            }
        }
        if (isDuplicate) {
            cout << ">> [LOI] So CCCD [" << CCCD << "] da ton tai tren he thong! Vui long nhap lai.\n";
            continue;
        }
        break;
    }

    // 3. Nhập Địa chỉ
    string Address;
    while (true) {
        Address = InputHelper::getString("Nhap dia chi cua khach hang moi (nhap 0 de thoat): ", false);
        if (Address == "0") {
            cout << ">> Da huy thao tac them moi khach hang.\n";
            return;
        }
        if (Address.find('|') != string::npos) {
            cout << ">> [LOI] Dia chi khong duoc chua ky tu '|'. Vui long nhap lai!\n";
            continue;
        }
        break;
    }

    // 4. Nhập Ngày sinh: Kiểm tra định dạng dd/mm/yyyy và tính hợp lệ ngay tại chỗ
    string DateOfBirth;
    while (true) {
        DateOfBirth = InputHelper::getString("Nhap ngay sinh cua khach hang moi (dd/mm/yyyy, nhap 0 de thoat): ", false);
        if (DateOfBirth == "0") {
            cout << ">> Da huy thao tac them moi khach hang.\n";
            return;
        }

        int ngay, thang, nam;
        char c1 = 0, c2 = 0;
        stringstream ss(DateOfBirth);
        if (!(ss >> ngay >> c1 >> thang >> c2 >> nam) || c1 != '/' || c2 != '/') {
            cout << ">> [LOI] Ngay sinh phai theo dinh dang dd/mm/yyyy! Vui long nhap lai.\n";
            continue;
        }
        Date d(ngay, thang, nam);
        if (!d.valid()) {
            cout << ">> [LOI] Ngay sinh khong hop le theo lich hoac vuot qua nam hien tai! Vui long nhap lai.\n";
            continue;
        }
        DateOfBirth = d.output(); // Chuẩn hóa về dạng chuẩn (ví dụ 5/3/2000 -> 05/03/2000)
        break;
    }

    // 5. CHỈ SINH MÃ ID KHI TẤT CẢ DỮ LIỆU ĐÃ HỢP LỆ 100% (Tránh làm thất thoát bộ đếm counter)
    string id = InputHelper::generateAutoID(repo, "KH", "data_counter/customer_counter.txt");
    cout << ">> Ma khach hang (tu dong sinh): " << id << "\n";

    try {
        Customer newCus(id, name, CCCD, Address, DateOfBirth);
        repo.add(newCus); // Lưu vào RAM và tự động ghi đè ra file customers.txt
        cout << ">> [THANH CONG] Da them khach hang [" << id << "] vao he thong!\n";
        printHeader();
        newCus.display();
    }
    catch (const exception& e) {
        cout << ">> [THAT BAI] " << e.what() << "\n";
    }
}
// ============================================================================
// CHỨC NĂNG 2: XEM TẤT CẢ KHÁCH HÀNG (READ ALL)
// - Mục đích: Hiển thị danh sách tất cả khách hàng dưới dạng bảng có căn lề cột.
// - Kỹ thuật áp dụng:
//   + Sử dụng tham chiếu const auto& list = repo.getAll() để tối ưu bộ nhớ.
//   + Kiểm tra danh sách rỗng (list.empty()).
//   + In tiêu đề bảng bằng printHeader() sử dụng std::setw.
//   + Duyệt danh sách bằng Range-based for loop C++11 và gọi hàm display().
// ============================================================================
void CustomerMenu :: handleReadAll(Repository<Customer>& repo) {
    cout << "\n---------------- DANH SACH KHACH HANG ----------------\n";
    
    // Bước 1: Lấy danh sách khách hàng từ Repository
    const auto& list = repo.getAll();

    // Bước 2: Kiểm tra danh sách rỗng
    if (list.empty()) {
        cout << ">> Danh sach khach hang hien dang trong!\n";
        return;
    }

    // Bước 3: In tiêu đề bảng các cột
    printHeader();

    // Bước 4: Duyệt qua từng đối tượng và gọi phương thức display() để hiển thị
    for (const auto& cus : list) {
        cus.display();
    }
    cout << string(78, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " khach hang.\n";
}

// ============================================================================
// CHỨC NĂNG 3: TÌM KIẾM KHÁCH HÀNG THEO MÃ (READ / SEARCH)
// - Mục đích: Tra cứu thông tin hồ sơ của khách hàng theo Mã khách hàng.
// - Kỹ thuật áp dụng:
//   + Tra cứu nhanh trên RAM thông qua repo.findById(id).
//   + Kiểm tra con trỏ nullptr để thông báo kết quả chính xác, tránh truy cập vùng nhớ rác.
// ============================================================================
void CustomerMenu :: handleSearch(Repository<Customer>& repo) {
    cout << "\n---------------- TIM KIEM KHACH HANG ----------------\n";
    
    while (true) {
        // Bước 1: Nhập mã khách hàng cần tìm (nhap 0 de thoat)
        string id = InputHelper :: getString("Nhap ma khach hang can xem (nhap 0 de thoat): ", false);
        if (id == "0") return;

        // Bước 2: Tìm kiếm đối tượng mang mã tương ứng trong vector
        auto* cus = repo.findById(id);

        // Bước 3: Hiển thị thông tin chi tiết nếu tìm thấy hoặc thông báo không tồn tại
        if (cus == nullptr) {
            cout << ">> [THONG BAO] Khong tim thay khach hang co ma: " << id << ". Vui long thu lai!\n";
            continue;
        }

        cout << "\n>> THONG TIN CHI TIET KHACH HANG:\n";
        printHeader();
        cus->display();
        cout << string(78, '-') << "\n";
        break;
    }
}

// ============================================================================
// CHỨC NĂNG 4: SẮP XẾP DANH SÁCH KHÁCH HÀNG (SORT)
// - Mục đích: Cho phép người dùng sắp xếp và hiển thị danh sách khách hàng theo
//   nhiều tiêu chí thực tế (Tên, Mã KH, Ngày sinh).
// - Kỹ thuật áp dụng:
//   + Sử dụng thuật toán std::sort từ thư viện chuẩn <algorithm> (tiêu chuẩn STL).
//   + Biểu thức Lambda C++11 linh hoạt làm comparator tùy biến:
//     * Xử lý trích xuất TÊN từ HỌ VÀ TÊN chuẩn theo quy tắc xưng tên tiếng Việt.
//     * Chuyển đổi định dạng ngày dd/mm/yyyy -> yyyymmdd để so sánh thời gian chính xác.
//   + Sắp xếp trên bản sao (vector<Customer> list = repo.getAll()), bảo toàn nguyên vẹn
//     thứ tự lưu trữ trong cơ sở dữ liệu / file text.
// ============================================================================
void CustomerMenu :: handleSort(Repository<Customer>& repo) {
    cout << "\n---------------- SAP XEP KHACH HANG ----------------\n";

    // Bước 1: Lấy bản sao danh sách khách hàng từ Repository
    vector<Customer> list = repo.getAll();

    // Kiểm tra danh sách rỗng để tránh xử lý thừa và báo cho người dùng
    if (list.empty()) {
        cout << ">> Danh sach khach hang hien dang trong!\n";
        return;
    }

    // Bước 2: Hiển thị các tiêu chí sắp xếp cho người dùng lựa chọn
    cout << "  1. Ho ten (A -> Z)\n";
    cout << "  2. Ho ten (Z -> A)\n";
    cout << "  3. Ma khach hang tang dan\n";
    cout << "  4. Ngay sinh (Cu -> Moi nhat)\n";
    cout << "  0. Quay lai\n";
    int subChoice = InputHelper::getInt("Nhap tieu chi sap xep (0-4): ", 0, 4);

    // Người dùng chọn 0 -> Thoát khỏi chức năng sắp xếp
    if (subChoice == 0) return;

    // Lambda helper: Trích xuất phần "Tên" từ chuỗi "Họ và Tên"
    // Ví dụ: "Nguyen Van An" -> cắt lấy "An" để sắp xếp theo chữ cái của Tên
    auto layTen = [](const string& hoTen) {
        size_t pos = hoTen.rfind(' '); // Tìm vị trí dấu cách cuối cùng
        if (pos != string :: npos) return hoTen.substr(pos + 1); // Lấy chuỗi sau dấu cách
        return hoTen; // Nếu không có dấu cách, giữ nguyên họ tên
    };

    // Bước 3: Áp dụng thuật toán std::sort tương ứng với tiêu chí được chọn
    switch (subChoice) {
        case 1:
            // Tiêu chí 1: Họ tên tăng dần từ A -> Z (Ưu tiên so sánh Tên)
            sort(list.begin(), list.end(), [&](const Customer& a, const Customer& b) {
                string tenA = layTen(a.getFullName());
                string tenB = layTen(b.getFullName());
                // Nếu Tên khác nhau -> So sánh theo Tên
                if (tenA != tenB) return tenA < tenB;
                // Nếu Tên trùng nhau -> So sánh toàn bộ Họ và Tên để phân định
                return a.getFullName() < b.getFullName();
            });
            cout << "\n>> DANH SACH KHACH HANG (HO TEN A -> Z):\n";
            break;

        case 2:
            // Tiêu chí 2: Họ tên giảm dần từ Z -> A (Đảo ngược chiều so sánh)
            sort(list.begin(), list.end(), [&](const Customer& a, const Customer& b) {
                string tenA = layTen(a.getFullName());
                string tenB = layTen(b.getFullName());
                if (tenA != tenB) return tenA > tenB;
                return a.getFullName() > b.getFullName();
            });
            cout << "\n>> DANH SACH KHACH HANG (HO TEN Z -> A):\n";
            break;

        case 3:
            // Tiêu chí 3: Mã khách hàng tăng dần (so sánh chuỗi std::string)
            sort(list.begin(), list.end(), [](const Customer& a, const Customer& b) {
                return a.getID() < b.getID();
            });
            cout << "\n>> DANH SACH KHACH HANG (MA KH TANG DAN):\n";
            break;

        case 4:
            // Tiêu chí 4: Ngày sinh từ cũ nhất đến mới nhất (Người lớn tuổi xếp trước)
            // Kỹ thuật: Định dạng ngày gốc là dd/mm/yyyy.
            // Để so sánh theo đúng thứ tự thời gian, chuyển đổi về định dạng yyyymmdd.
            // Ví dụ: 25/12/1990 -> 19901225; 01/01/2005 -> 20050101 (19901225 < 20050101).
            sort(list.begin(), list.end(), [](const Customer& a, const Customer& b) {
                string dA = a.getDateOfBirth();
                string dB = b.getDateOfBirth();
                auto convertDate = [](const string& d) {
                    if (d.length() >= 10) {
                        return d.substr(6, 4) + d.substr(3, 2) + d.substr(0, 2);
                    }
                    return d;
                };
                return convertDate(dA) < convertDate(dB);
            });
            cout << "\n>> DANH SACH KHACH HANG (NGAY SINH CU -> MOI):\n";
            break;
    }

    // Bước 4: In tiêu đề bảng và duyệt danh sách khách hàng đã sắp xếp để hiển thị
    printHeader();
    for (const auto& cus : list) {
        cus.display();
    }
    cout << string(78, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " khach hang.\n";
}

// ============================================================================
// CHỨC NĂNG 5: CẬP NHẬT THÔNG TIN KHÁCH HÀNG (UPDATE)
// - Mục đích: Cập nhật thông tin cá nhân (Họ tên, CCCD, Địa chỉ, Ngày sinh).
// - Quy tắc nghiệp vụ & Kỹ thuật:
//   + Mã khách hàng (ID) giữ vai trò định danh duy nhất, KHÔNG được sửa đổi.
//   + Hỗ trợ phím ENTER để giữ nguyên thông tin cũ (chỉ cập nhật trường có dữ liệu mới).
//   + Xác thực chặt chẽ qua các hàm Setter (setCCCD, setDateOfBirth,...).
//   + Cơ chế Exception Handling: Thông báo lỗi và không làm biến đổi dữ liệu cũ nếu sai.
//   + Gọi repo.saveToFile() để lưu thay đổi xuống file customers.txt.
// ============================================================================
void CustomerMenu :: handleUpdate(Repository<Customer>& repo) {
    cout << "\n---------------- CAP NHAT KHACH HANG ----------------\n";
    
    // Bước 1: Nhập mã khách hàng cần sửa và kiểm tra tồn tại (nhap 0 de thoat)
    string id;
    Customer* cus = nullptr;
    while (true) {
        id = InputHelper :: getString("Nhap ma khach hang can sua (nhap 0 de thoat): ", false);
        if (id == "0") return;

        cus = repo.findById(id);
        if (cus == nullptr) {
            cout << ">> [THONG BAO] Khong tim thay khach hang co ma: " << id << ". Vui long thu lai!\n";
            continue;
        }
        break;
    }
    
    // Bước 2: Hiển thị thông tin hiện tại của khách hàng để người dùng đối chiếu
    cout << "\n>> Thong tin hien tai cua khach hang:\n";
    printHeader();
    cus->display();
    cout << string(78, '-') << "\n";
    cout << "(Luu y: Nhan Enter de trong neu muon GIU NGUYEN thong tin cu. Ma khach hang khong duoc sua)\n\n";
    
    // Bước 3: Nhập dữ liệu mới (cho phép bỏ trống: allowEmpty = true)
    string newName        = InputHelper :: getString("Nhap ten khach hang moi: ", true);
    string newCCCD        = InputHelper :: getString("Nhap CCCD cua khach hang moi: ", true);
    string newAddress     = InputHelper :: getString("Nhap dia chi cua khach hang moi: ", true);
    string newDateOfBirth = InputHelper :: getString("Nhap ngay sinh moi: ", true);

    // [FIX LOI 3] Dung ban sao: kiem tra tinh hop le cua TAT CA truong moi
    // truoc khi ghi vao doi tuong goc. Neu bat ky truong nao loi, khong co gi thay doi.
    try {
        // [FIX LOI 3] Dung ban sao: kiem tra tat ca truong hop le truoc khi ghi vao goc.
        Customer updated = *cus;
        if (!newName.empty())        updated.setFullName(newName);
        if (!newCCCD.empty()) {
            for (const auto& item : repo.getAll()) {
                if (item.getID() != id && item.getCCCD() == newCCCD) {
                    cout << "\n>> [THAT BAI] So CCCD [" << newCCCD << "] da ton tai tren he thong!\n";
                    return;
                }
            }
            updated.setCCCD(newCCCD);
        }
        if (!newAddress.empty())     updated.setAddress(newAddress);
        if (!newDateOfBirth.empty()) updated.setDateOfBirth(newDateOfBirth);
        // Tat ca setter deu thanh cong -> ghi ban cap nhat vao repo va file
        repo.update(id, updated);
        cout << "\n>> [THANH CONG] Da cap nhat thong tin khach hang [" << id << "] thanh cong!\n";
    }
    catch (const exception& e) {
        cout << "\n>> [THAT BAI] Khong the cap nhat: " << e.what() << "\n";
    }
}

// ============================================================================
// CHỨC NĂNG 6: XÓA KHÁCH HÀNG (DELETE)
// - Mục đích: Xóa hồ sơ khách hàng khỏi hệ thống nếu không còn hợp đồng sử dụng.
// - Kỹ thuật & Ràng buộc toàn vẹn khóa ngoại (Foreign Key):
//   + Quét file hợp đồng (contracts.txt).
//   + Nếu khách hàng đang đứng tên hợp đồng thuê bao -> TUYỆT ĐỐI KHÔNG XÓA.
//   + Xác nhận 2 bước (y/n) trước khi thực thi lệnh xóa.
//   + Gọi repo.remove(id) để giải phóng vùng nhớ và lưu lại file.
// ============================================================================
void CustomerMenu :: handleDelete(Repository<Customer>& repo) {
    cout << "\n---------------- XOA KHACH HANG ----------------\n";
    
    // Bước 1: Nhập mã và kiểm tra khách hàng có tồn tại trong hệ thống không (nhap 0 de thoat)
    string id;
    Customer* cus = nullptr;
    while (true) {
        id = InputHelper :: getString("Nhap ma khach hang can xoa (nhap 0 de thoat): ", false);
        if (id == "0") return;

        cus = repo.findById(id);
        if (cus == nullptr) {
            cout << ">> [Loi]: Khong tim thay khach hang co ma: " << id << ". Vui long thu lai!\n";
            continue;
        }
        break;
    }

    // Bước 2: Hiển thị lại thông tin khách hàng chuẩn bị xóa
    cout << "\n>> Thong tin khach hang duoc chon de xoa:\n";
    printHeader();
    cus->display();
    cout << string(78, '-') << "\n";

    // Bước 3: Kiểm tra ràng buộc toàn vẹn khóa ngoại (liên kết với hợp đồng)
    // [FIX LOI 1] Neu tai file hop dong that bai, DUNG thao tac xoa de an toan.
    Repository<Contract> contractRepo("data/contracts.txt");
    try {
        contractRepo.loadFromFile();
    } catch (const exception& e) {
        cout << ">> [LOI] Khong the tai danh sach hop dong de kiem tra rang buoc: "
             << e.what() << "\n";
        cout << ">> [TU CHOI] Khong the xac minh rang buoc - thao tac xoa bi huy bo.\n";
        return;
    }

    bool isLinked = false;
    for (const auto& contract : contractRepo.getAll()) {
        if (contract.getCustomerID() == id) { // getCustomerID() lưu mã khách hàng trong hợp đồng
            isLinked = true;
            break;
        }
    } 
    // Nếu khách hàng đang có hợp đồng -> Từ chối xóa để đảm bảo toàn vẹn dữ liệu
    if (isLinked) {
        cout << ">> [Tu choi xoa]: Khach hang [" << id 
             << "] dang dung ten trong hop dong, khong duoc phep xoa!\n";
        return;
    }
    
    // Bước 4: Hỏi xác nhận an toàn từ người dùng
    string confirm = InputHelper::getString("Ban co chac chan muon xoa? (y/n): ", false);
    if (confirm == "y" || confirm == "Y") {
        repo.remove(id); // Xóa phần tử khỏi vector và cập nhật lại file customers.txt
        cout << ">> [THANH CONG] Xoa thanh cong khach hang [" << id << "]!\n";
    } else {
        cout << ">> [THONG BAO] Thao tac xoa da duoc huy bo an toan.\n";
    }
}

// =====================================================================
// PHẦN 4: MENU CONSOLE ĐIỀU KHIỂN CHÍNH CỦA KHÁCH HÀNG
// =====================================================================

void CustomerMenu :: run() {
    // Tự khởi tạo Repository và nạp dữ liệu từ file customers.txt vào RAM khi mở menu
    // [FIX LOI 2] Nap du lieu tu file: neu that bai phai bao loi va thoat,
    // khong duoc de menu chay voi danh sach rong roi ghi de mat toan bo du lieu cu.
    Repository<Customer> repo("data/customers.txt");
    try {
        repo.loadFromFile();
    } catch (const exception& e) {
        cout << "\n>> [LOI NGHIEM TRONG] Khong the tai du lieu khach hang: " << e.what() << "\n";
        cout << ">> Vui long kiem tra file data/customers.txt. Quay lai menu chinh...\n";
        return;
    }

    int choice;
    do {
        cout << "\n============ QUAN LY KHACH HANG ============\n";
        cout << "  1. Them khach hang moi\n";
        cout << "  2. Xem danh sach khach hang\n";
        cout << "  3. Tim kiem khach hang\n";
        cout << "  4. Sap xep danh sach khach hang\n";
        cout << "  5. Cap nhat thong tin khach hang\n";
        cout << "  6. Xoa khach hang\n";
        cout << "  0. Quay lai menu chinh\n";
        cout << "==========================================\n";
        choice = InputHelper::getInt("Nhap lua chon: ", 0, 6);

        switch (choice) {
            case 1: handleCreate(repo);   break;
            case 2: handleReadAll(repo);  break;
            case 3: handleSearch(repo);   break;
            case 4: handleSort(repo);     break;
            case 5: handleUpdate(repo);   break;
            case 6: handleDelete(repo);   break;
            case 0:
                cout << ">> Dang quay lai Menu chinh...\n";
                break;
            default:
                cout << "[!] Lua chon khong hop le, vui long nhap tu 0 den 6!\n";
        }
    } while (choice != 0);
}

// Hàm để main.cpp gọi trực tiếp
void menuKhachHang() {
    CustomerMenu::run();
}
