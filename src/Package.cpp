/**
 * ==============================================================================
 * BÀI TẬP LỚN: HỆ THỐNG QUẢN LÝ THUÊ BAO DI ĐỘNG (NHÓM 06 - ĐỀ TÀI 01)
 * Học phần: Kỹ thuật lập trình
 * Người thực hiện: Bùi Gia Khánh
 * MSSV: B24DCVT191
 * Module phụ trách: Entity.h (Lớp cơ sở trừu tượng)
 * Use Case: Quản lý Gói cước (Package)
 * ==============================================================================
 */

#include "Package.h"        // Header khai báo lớp Package và PackageMenu
#include "Contract.h"       // Khai báo lớp Contract (để kiểm tra khóa ngoại trước khi xóa gói)
#include "InputHelper.h"    // Tiện ích nhập xuất an toàn trên console
#include "Exceptions.h"     // Các lớp ngoại lệ tự định nghĩa
#include <iostream>         // Luồng xuất nhập cout, cin
#include <iomanip>          // Định dạng bảng (setw, left, right, fixed, setprecision)
#include <string>           // Xử lý chuỗi string
#include <vector>           // Chứa danh sách dữ liệu trong Repository
#include <sstream>          // Tách/ghép chuỗi qua stringstream (toString, fromString)
#include <algorithm>        // Thuật toán sort và biểu thức lambda

using namespace std;

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ PACKAGE
// =====================================================================

// Cài đặt hàm khởi tạo 
Package :: Package()
    : Entity(""), Price(0), DataMB(0), VoiceMinutes(0), SMS(0), Days(30) {}

Package :: Package(string ID, string name, double price, int data, int voice, int sms, int days)
    : Entity(ID) {
    setName(name);
    setPrice(price);
    setDataMB(data);
    setVoiceMinutes(voice);
    setSMS(sms);
    setDays(days);
}

// Cài đặt các hàm thuần ảo 
string Package :: getID() const {
    return ID;
}

string Package :: toString() const {
    stringstream ss;
    ss << ID << "|" << PackageName << "|" 
       << fixed << setprecision(0) << Price << "|" 
       << DataMB << "|" << VoiceMinutes << "|" << SMS << "|" << Days;
    return ss.str();
}

void Package :: fromString(const string& line) {
    if (line.empty()) return;
    stringstream ss(line);
    string PriceStr;
    string DataMBStr;
    string VoiceMinutesStr;
    string SMSStr;
    string DaysStr;
    string tempName;

    // [FIX LỖI 4] Kiểm tra đủ số cột trước khi parse; dùng setter để validate dữ liệu đọc từ file
    string tempID;
    getline(ss, tempID, '|');
    if (tempID == "MaGoi" || tempID.empty()) {
        ID = "";
        return;
    }
    if (!getline(ss, tempName,       '|')) return;
    if (!getline(ss, PriceStr,       '|')) return;
    if (!getline(ss, DataMBStr,      '|')) return;
    if (!getline(ss, VoiceMinutesStr,'|')) return;
    if (!getline(ss, SMSStr,         '|')) return;
    if (!getline(ss, DaysStr))             return; // cột cuối, không có '|' theo sau

    try {
        size_t pos;
        double parsedPrice = 0;
        int parsedData = 0, parsedVoice = 0, parsedSMS = 0, parsedDays = 30;

        if (!PriceStr.empty()) {
            parsedPrice = stod(PriceStr, &pos);
            if (pos != PriceStr.size())  // [FIX LỖI 4] "100abc" sẽ bị loại bỏ
                throw invalid_argument("Gia cuoc chua ky tu la!");
        }
        if (!DataMBStr.empty()) {
            parsedData = stoi(DataMBStr, &pos);
            if (pos != DataMBStr.size()) throw invalid_argument("DataMB chua ky tu la!");
        }
        if (!VoiceMinutesStr.empty()) {
            parsedVoice = stoi(VoiceMinutesStr, &pos);
            if (pos != VoiceMinutesStr.size()) throw invalid_argument("VoiceMinutes chua ky tu la!");
        }
        if (!SMSStr.empty()) {
            parsedSMS = stoi(SMSStr, &pos);
            if (pos != SMSStr.size()) throw invalid_argument("SMS chua ky tu la!");
        }
        if (!DaysStr.empty()) {
            parsedDays = stoi(DaysStr, &pos);
            if (pos != DaysStr.size()) throw invalid_argument("Days chua ky tu la!");
        }

        // [FIX LỖI 4] Gọi setter để đảm bảo dữ liệu file hợp lệ (không âm, chu kỳ > 0...)
        ID = tempID;
        setName(tempName);
        setPrice(parsedPrice);
        setDataMB(parsedData);
        setVoiceMinutes(parsedVoice);
        setSMS(parsedSMS);
        setDays(parsedDays);
    } catch (...) {
        // Nếu dữ liệu bất hợp lệ: đặt ID rỗng để Repository bỏ qua bản ghi này
        ID = "";
    }
}

void Package :: getInformation() {
    ID = InputHelper :: getString("Nhap ma goi cuoc: ", false);
    setName(InputHelper :: getString("Nhap ten goi cuoc: ", false));
    setPrice(InputHelper :: getDouble("Nhap gia cuoc VND: ", 0.0));
    setDataMB(InputHelper :: getInt("Nhap dung luong DataMB (MB): ", 0));
    setVoiceMinutes(InputHelper :: getInt("Nhap thoi luong cuoc goi (phut): ", 0));
    setSMS(InputHelper :: getInt("Nhap so luong SMS: ", 0));
    setDays(InputHelper :: getInt("Nhap chu ky goi cuoc (ngay): ", 1));
}

void Package :: display() const {
    cout << left  << setw(10) << ID 
         << left  << setw(18) << PackageName 
         << right << setw(11) << (long long)Price << " D"
         << right << setw(11) << DataMB << " MB"
         << right << setw(8)  << VoiceMinutes << " P"
         << right << setw(5)  << SMS << " tin"
         << right << setw(6)  << Days << " ngay" << endl;
}

// Cài đặt getter
string Package :: getName() const {
    return PackageName;
} 
double Package :: getPrice() const {
    return Price;
}
int Package :: getDataMB() const {
    return DataMB;
}
int Package :: getVoiceMinutes() const {
    return VoiceMinutes;
}
int Package :: getSMS() const {
    return SMS;
}
int Package :: getDays() const {
    return Days;
}

// Cài đặt setter có kiểm tra tính hợp lệ dữ liệu
void Package :: setName(const string& n) {
    if (n.empty()) {
        throw InvalidDataException("Ten goi cuoc khong duoc de trong!");
    }
    PackageName = n;
}

void Package :: setPrice(double p) {
    if (p < 0.0) {
        throw InvalidDataException("Gia cuoc khong the am!");
    }
    Price = p;
}

void Package :: setDataMB(int d) {
    if (d < 0) {
        throw InvalidDataException("Dung luong Data khong the am!");
    }
    DataMB = d;
}

void Package :: setVoiceMinutes(int v) {
    if (v < 0) {
        throw InvalidDataException("Thoi luong thoai khong the am!");
    }
    VoiceMinutes = v;
}

void Package :: setSMS(int s) {
    if (s < 0) {
        throw InvalidDataException("So luong SMS khong the am!");
    }
    SMS = s;
}

void Package :: setDays(int d) {
    if (d <= 0) {
        throw InvalidDataException("Chu ky ngay phai lon hon 0!");
    }
    Days = d;
}

// =====================================================================
// PHẦN 2: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA PACKAGEMENU
// =====================================================================

void PackageMenu :: printHeader() {
    cout << string(85, '-') << "\n";
    cout << left  << setw(10) << "Ma Goi"
         << left  << setw(18) << "Ten Goi Cuoc"
         << right << setw(13) << "Gia Cuoc"
         << right << setw(14) << "Data"
         << right << setw(10) << "Thoai"
         << right << setw(9)  << "SMS"
         << right << setw(11) << "Chu Ky" << "\n";
    cout << string(85, '-') << "\n";
}

// ============================================================================
// CHỨC NĂNG 1: THÊM MỚI GÓI CƯỚC (CREATE)
// - Mục đích: Cho phép nhân viên nhập thông số kỹ thuật và thêm một gói cước mới vào hệ thống.
// - Kỹ thuật áp dụng:
//   + Cơ chế tự động sinh mã ID (Auto-ID): Sử dụng InputHelper::generateAutoID cấp phát mã
//     duy nhất dạng PKG + số thứ tự tăng dần, đảm bảo toàn vẹn khóa chính và đồng bộ với Contract.
//   + Cơ chế trì hoãn cấp mã: Chỉ sinh mã sau khi nhân viên đã nhập đầy đủ và hợp lệ toàn bộ
//     thông số, tránh việc hủy hoặc dừng giữa chừng làm nhảy số bộ đếm (package_counter.txt).
//   + Tiện ích nhập liệu an toàn: Sử dụng InputHelper (getString, getDouble, getInt) để chống trôi
//     lệnh, ép kiểu an toàn và kiểm tra miền giá trị hợp lệ (giá >= 0, chu kỳ >= 1 ngày...).
//   + Áp dụng tính đóng gói: Constructor gọi các Setter để validate dữ liệu đầu vào.
//   + Cơ chế Exception Handling: Bắt InvalidDataException nếu thông số vi phạm nghiệp vụ.
//   + Đồng bộ hóa: repo.add() vừa nạp vào vector trong RAM, vừa ghi tự động ra file text.
// ============================================================================

void PackageMenu :: handleCreate(Repository<Package>& repo) {
    cout << "\n---------------- THEM GOI CUOC MOI ----------------\n";
    
    // Bước 1: Nhập tên gói cước và kiểm tra trùng lặp với danh sách đã có
    string name;
    while (true) {
        name = InputHelper :: getString("Nhap ten goi cuoc (nhap 0 de thoat): ", false);
        if (name == "0") {
            cout << ">> Da huy thao tac them moi goi cuoc.\n";
            return;
        }

        // Kiểm tra xem tên gói cước đã tồn tại trong hệ thống chưa (tránh trùng lặp)
        bool isDuplicate = false;
        for (const auto& pkg : repo.getAll()) {
            if (pkg.getName() == name) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            cout << ">> [LOI] Ten goi cuoc [" << name << "] da ton tai tren he thong! Vui long nhap ten khac.\n";
            continue;
        }
        break; // Tên hợp lệ, chưa từng tồn tại
    }

    // Nhập các thông số kỹ thuật của gói cước
    double price = InputHelper :: getDouble("Nhap gia cuoc VND: ", 0.0);
    int data     = InputHelper :: getInt("Nhap dung luong DataMB (MB): ", 0);
    int voice    = InputHelper :: getInt("Nhap thoi luong cuoc goi (phut): ", 0);
    int sms      = InputHelper :: getInt("Nhap so luong SMS: ", 0);
    int days     = InputHelper :: getInt("Nhap chu ky goi cuoc (ngay): ", 1);

    // Bước 2: Chỉ khi đã nhập đầy đủ, hợp lệ và không trùng lặp, mới tự động sinh mã ID
    // Tránh việc đang nhập dở hoặc dữ liệu không hợp lệ làm tăng bộ đếm ID
    string id = InputHelper::generateAutoID(repo, "PKG", "data_counter/package_counter.txt");
    cout << ">> Ma goi cuoc (tu dong sinh): " << id << "\n";

    // Bước 3: Khởi tạo đối tượng Package và thêm vào Repository
    // Toàn bộ logic kiểm tra tính hợp lệ dữ liệu (giá > 0, chu kỳ > 0, ...)
    // được kiểm tra thông qua các Setter bên trong constructor của Package.
    try {
        Package newPkg(id, name, price, data, voice, sms, days);
        repo.add(newPkg); // Thêm vào std::vector và tự động đồng bộ ghi ra file text
        cout << ">> [THANH CONG] Da them goi cuoc [" << id << "] vao he thong!\n";
        printHeader();
        newPkg.display();
    } 
    catch (const exception& e) {
        // Bắt ngoại lệ InvalidDataException ném ra từ các Setter nếu dữ liệu vi phạm
        cout << ">> [THAT BAI] " << e.what() << "\n";
    }
}

// ============================================================================
// CHỨC NĂNG 2: XEM TẤT CẢ GÓI CƯỚC (READ ALL)
// - Mục đích: Hiển thị toàn bộ danh sách gói cước hiện có theo định dạng bảng ngay ngắn.
// - Kỹ thuật áp dụng:
//   + Lấy tham chiếu hằng (const auto& list) để tránh sao chép dữ liệu, tối ưu hiệu năng.
//   + Kiểm tra danh sách rỗng trước khi hiển thị.
//   + Sử dụng hàm printHeader() kết hợp std::setw để căn chỉnh cột ngay thẳng.
//   + Range-based for loop (C++11) duyệt qua từng phần tử và gọi phương thức display().
// ============================================================================
void PackageMenu :: handleReadAll(Repository<Package>& repo) { 
    cout << "\n---------------- DANH SACH GOI CUOC ----------------\n";
    
    // Bước 1: Lấy tham chiếu danh sách gói cước từ Repository
    const auto& list = repo.getAll();
    
    // Bước 2: Kiểm tra nếu danh sách rỗng thì thông báo và thoát
    if (list.empty()) {
        cout << ">> [THONG BAO] Danh sach goi cuoc hien tai dang trong!\n";
        return;
    }
    
    // Bước 3: In dòng tiêu đề bảng với các cột được định dạng cố định
    printHeader();
    
    // Bước 4: Duyệt toàn bộ danh sách và gọi phương thức display() của từng đối tượng
    for (const auto& pkg : list) {
        pkg.display();  
    }
    cout << string(85, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " goi cuoc.\n";
}

// ============================================================================
// CHỨC NĂNG 3: TÌM KIẾM GÓI CƯỚC THEO MÃ (READ / SEARCH)
// - Mục đích: Tra cứu và xem chi tiết thông tin của một gói cước theo mã ID.
// - Kỹ thuật áp dụng:
//   + Vòng lặp kiểm tra nhập liệu: Cho phép nhập lại nếu mã không tồn tại.
//   + Cơ chế thoát an toàn: Nhập 0 để quay lại menu chính bất kỳ lúc nào.
//   + Tìm kiếm trực tiếp trên RAM qua repo.findById(id).
//   + Hiển thị đồng bộ theo định dạng bảng chuẩn bằng printHeader() và display().
// ============================================================================

void PackageMenu :: handleSearch(Repository<Package>& repo) {
    cout << "\n---------------- TIM KIEM GOI CUOC ----------------\n";
    
    while (true) {
        // Bước 1: Nhập mã gói cước cần tra cứu (cho phép nhập 0 để thoát)
        string id = InputHelper::getString("Nhap ma goi can tim (nhap 0 de thoat): ", false);
        if (id == "0") {
            cout << ">> Dang quay lai Menu...\n";
            return;
        }
        
        // Bước 2: Tìm kiếm đối tượng trong vector bộ nhớ
        auto* pkg = repo.findById(id);
        
        // Bước 3: Kiểm tra kết quả tìm kiếm
        if (pkg == nullptr) {
            cout << ">> [THONG BAO] Khong tim thay goi cuoc co ma: " << id << ". Vui long thu lai!\n";
            continue; // Cho phép nhập lại mã khác mà không bị văng ra menu
        }

        // Bước 4: Hiển thị thông tin chi tiết gói cước tìm thấy
        cout << "\n>> THONG TIN CHI TIET GOI CUOC:\n";
        printHeader();
        pkg->display();
        cout << string(85, '-') << "\n";
        break; // Tìm thấy và in ra thành công -> kết thúc chức năng
    }
}

// ============================================================================
// CHỨC NĂNG 4: SẮP XẾP DANH SÁCH GÓI CƯỚC (SORT)
// - Mục đích: Cho phép người dùng sắp xếp và xem danh sách gói cước theo các tiêu chí khác nhau.
// - Kỹ thuật áp dụng: 
//   + Sử dụng hàm std::sort từ thư viện chuẩn <algorithm> (đáp ứng yêu cầu sử dụng STL của BTL).
//   + Kết hợp biểu thức Lambda (C++11) làm hàm so sánh tuỳ biến (custom comparator).
//   + Sắp xếp trên bản sao (vector<Package> list = repo.getAll()), đảm bảo KHÔNG làm
//     thay đổi thứ tự dữ liệu gốc được lưu trong file/repository.
// ============================================================================
void PackageMenu :: handleSort(Repository<Package>& repo) {
    cout << "\n---------------- SAP XEP GOI CUOC ----------------\n";

    // Bước 1: Lấy bản sao danh sách gói cước hiện có từ Repository
    vector<Package> list = repo.getAll();

    // Kiểm tra danh sách rỗng để tránh xử lý thừa và báo cho người dùng
    if (list.empty()) {
        cout << ">> [THONG BAO] Danh sach goi cuoc hien tai dang trong!\n";
        return;
    }

    // Bước 2: Hiển thị menu phụ các tiêu chí sắp xếp cho người dùng chọn
    cout << "  1. Gia cuoc tang dan\n";
    cout << "  2. Gia cuoc giam dan\n";
    cout << "  3. Dung luong Data (MB) tang dan\n";
    cout << "  4. Ten goi cuoc (A -> Z)\n";
    cout << "  0. Quay lai\n";
    int subChoice = InputHelper::getInt("Nhap tieu chi sap xep (0-4): ", 0, 4);

    // Người dùng chọn 0 -> Hủy bỏ thao tác sắp xếp, quay về menu chính
    if (subChoice == 0) return;

    // Bước 3: Thực hiện sắp xếp tương ứng với từng tiêu chí được chọn
    switch (subChoice) {
        case 1:
            // Tiêu chí 1: Giá cước tăng dần
            // Lambda so sánh: Trả về true nếu phần tử 'a' có giá nhỏ hơn 'b'
            sort(list.begin(), list.end(), [](const Package& a, const Package& b) {
                return a.getPrice() < b.getPrice();
            });
            cout << "\n>> DANH SACH GOI CUOC (GIA CUOC TANG DAN):\n";
            break;

        case 2:
            // Tiêu chí 2: Giá cước giảm dần
            // Lambda so sánh: Trả về true nếu phần tử 'a' có giá lớn hơn 'b'
            sort(list.begin(), list.end(), [](const Package& a, const Package& b) {
                return a.getPrice() > b.getPrice();
            });
            cout << "\n>> DANH SACH GOI CUOC (GIA CUOC GIAM DAN):\n";
            break;

        case 3:
            // Tiêu chí 3: Dung lượng Data MB tăng dần
            // Lambda so sánh: Trả về true nếu 'a' có ít dung lượng MB hơn 'b'
            sort(list.begin(), list.end(), [](const Package& a, const Package& b) {
                return a.getDataMB() < b.getDataMB();
            });
            cout << "\n>> DANH SACH GOI CUOC (DATA MB TANG DAN):\n";
            break;

        case 4:
            // Tiêu chí 4: Tên gói cước theo thứ tự từ điển A -> Z
            // Lambda so sánh: So sánh chuỗi std::string theo thứ tự lexicographical
            sort(list.begin(), list.end(), [](const Package& a, const Package& b) {
                return a.getName() < b.getName();
            });
            cout << "\n>> DANH SACH GOI CUOC (TEN GOI A -> Z):\n";
            break;
    }

    // Bước 4: In tiêu đề cột và duyệt toàn bộ danh sách đã sắp xếp để in ra màn hình
    printHeader();
    for (const auto& pkg : list) {
        pkg.display();
    }
    cout << string(85, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " goi cuoc.\n";
}

// ============================================================================
// CHỨC NĂNG 5: CẬP NHẬT THÔNG TIN GÓI CƯỚC (UPDATE)
// - Mục đích: Cập nhật thông tin gói cước (Tên, Giá, Data, Thoại, SMS, Chu kỳ).
// - Quy tắc nghiệp vụ:
//   + Mã gói cước (ID) là khóa chính cố định, KHÔNG được phép thay đổi.
//   + Cho phép người dùng nhấn phím ENTER (để chuỗi rỗng) để GIỮ NGUYÊN giá trị cũ,
//     chỉ cập nhật những thông tin mà người dùng nhập giá trị mới.
//   + Sử dụng các hàm Setter để validate toàn bộ dữ liệu cập nhật.
//   + Sau khi cập nhật thành công trong RAM, gọi repo.saveToFile() để lưu xuống file.
// ============================================================================
void PackageMenu :: handleUpdate(Repository<Package>& repo) {
    cout << "\n---------------- CAP NHAT THONG TIN GOI CUOC ----------------\n";
    
    // Bước 1: Nhập mã gói cước cần sửa và kiểm tra tồn tại (cho phép nhập 0 để thoát)
    string id;
    Package* pkg = nullptr;
    while (true) {
        id = InputHelper :: getString("Nhap ma goi cuoc can sua (nhap 0 de thoat): ", false);
        if (id == "0") return;

        pkg = repo.findById(id);
        if (pkg == nullptr) {
            cout << ">> [THONG BAO] Khong tim thay goi cuoc co ma: " << id << ". Vui long thu lai!\n";
            continue;
        }
        break;
    }
    
    // Bước 2: Hiển thị thông tin hiện tại của gói cước để người dùng dễ quan sát
    cout << "\n>> Thong tin hien tai cua goi cuoc:\n";
    printHeader();
    pkg->display();
    cout << string(85, '-') << "\n";
    cout << "(Luu y: Nhan Enter de trong neu muon GIU NGUYEN thong tin cu. Ma goi cuoc khong duoc sua)\n\n";
    
    // Bước 3: Nhập dữ liệu mới dưới dạng chuỗi (cho phép rỗng: allowEmpty = true)
    string newName  = InputHelper :: getString("Nhap ten goi cuoc moi: ", true);
    string priceStr = InputHelper :: getString("Nhap gia cuoc moi (VND): ", true);
    string dataStr  = InputHelper :: getString("Nhap dung luong DataMB moi (MB): ", true);
    string voiceStr = InputHelper :: getString("Nhap thoi luong cuoc goi moi (phut): ", true);
    string smsStr   = InputHelper :: getString("Nhap so luong SMS moi: ", true);
    string daysStr  = InputHelper::getString("Nhap chu ky goi cuoc moi (ngay): ", true);
    
    // Bước 4 & 5: Kiểm tra tính hợp lệ qua Setter và ghi đồng bộ ra file
    try {
        // [FIX LOI 3] Dung ban sao: kiem tra tinh hop le cua TAT CA truong moi
        // truoc khi ghi vao doi tuong goc. Neu bat ky truong nao loi, khong co gi thay doi.
        Package updated = *pkg;
        if (!newName.empty()) {
            for (const auto& item : repo.getAll()) {
                if (item.getID() != id && item.getName() == newName) {
                    cout << "\n>> [THAT BAI] Ten goi cuoc [" << newName << "] da ton tai tren he thong!\n";
                    return;
                }
            }
            updated.setName(newName);
        }
        if (!priceStr.empty()) updated.setPrice(stod(priceStr));
        if (!dataStr.empty())  updated.setDataMB(stoi(dataStr));
        if (!voiceStr.empty()) updated.setVoiceMinutes(stoi(voiceStr));
        if (!smsStr.empty())   updated.setSMS(stoi(smsStr));
        if (!daysStr.empty())  updated.setDays(stoi(daysStr));

        // Tat ca setter deu thanh cong -> ghi ban cap nhat vao repo va file
        repo.update(id, updated);
        cout << "\n>> [THANH CONG] Da cap nhat thong tin goi cuoc [" << id << "] thanh cong!\n";
    }
    catch (const exception& e) {
        // Bat loi neu nguoi dung nhap so am hoac dinh dang khong hop le
        // Doi tuong goc trong repo KHONG bi thay doi vi ta lam viec tren ban sao
        cout << "\n>> [THAT BAI] Khong the cap nhat: " << e.what() << "\n";
    }
}

// ============================================================================
// CHỨC NĂNG 6: XÓA GÓI CƯỚC (DELETE)
// - Mục đích: Xóa một gói cước ra khỏi hệ thống nếu không còn ràng buộc.
// - Kỹ thuật & Ràng buộc toàn vẹn dữ liệu:
//   + Kiểm tra xem gói cước có đang được sử dụng trong hợp đồng nào không (Foreign Key).
//   + Nếu đang có hợp đồng sử dụng (contracts.txt chứa mã gói này) -> TUYỆT ĐỐI KHÔNG XÓA.
//   + Hỏi xác nhận người dùng (y/n) trước khi thực hiện hành động xóa.
//   + repo.remove() sẽ xóa khỏi vector và ghi đồng bộ lại file dữ liệu.
// ============================================================================
void PackageMenu :: handleDelete(Repository<Package>& repo) {
    cout << "\n---------------- XOA GOI CUOC ----------------\n";
    
    // Bước 1: Nhập mã gói cước và kiểm tra xem có tồn tại trong hệ thống không (nhập 0 để thoát)
    string id;
    Package* pkg = nullptr;
    while (true) {
        id = InputHelper :: getString("Nhap ma goi cuoc can xoa (nhap 0 de thoat): ", false);
        if (id == "0") return;

        pkg = repo.findById(id);
        if (pkg == nullptr) {
            cout << ">> [THONG BAO] Khong tim thay goi cuoc co ma: " << id << ". Vui long thu lai!\n";
            continue;
        }
        break;
    }
    
    // Bước 2: Hiển thị lại thông tin gói cước chuẩn bị xóa
    cout << "\n>> Thong tin hien tai cua goi cuoc:\n";
    printHeader();
    pkg->display();
    cout << string(85, '-') << "\n";
    
    // Bước 3: Kiểm tra ràng buộc toàn vẹn khóa ngoại (Foreign Key Integrity)
    // Một gói cước chỉ được xóa khi không có hợp đồng nào đang đăng ký gói đó.
    // [FIX LOI 1] Neu tai file hop dong that bai, DUNG thao tac xoa de an toan,
    //             khong am tham bo qua loi roi de nguoi dung xoa goi dang dung.
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
        if (contract.getPhonePlan() == id) { // getPhonePlan() lưu mã gói cước đăng ký
            isLinked = true;
            break;
        }
    }

    // Nếu tìm thấy gói cước này đang liên kết với hợp đồng -> Từ chối xóa
    if (isLinked) {
        cout << ">> [Tu choi xoa]: Goi cuoc [" << id 
             << "] dang ton tai trong hop dong khach hang, khong duoc phep xoa!\n";
        return;
    }

    // Bước 4: Hỏi xác nhận người dùng (xác nhận 2 bước an toàn)
    string confirm = InputHelper :: getString("Ban co chac chan muon xoa? (y/n): ", false);
    if (confirm == "y" || confirm == "Y") {
        repo.remove(id); // Thu hồi vùng nhớ và xóa phần tử khỏi vector, đồng thời cập nhật file
        cout << ">> [THANH CONG] Xoa thanh cong goi cuoc [" << id << "]!\n";
    } else {
        cout << ">> [THONG BAO] Thao tac xoa da duoc huy bo an toan.\n";
    }
}

// =====================================================================
// PHẦN 3: MENU CONSOLE ĐIỀU KHIỂN CHÍNH CỦA GÓI CƯỚC
// =====================================================================

void PackageMenu :: run() {
    // Khởi tạo Repository và nạp dữ liệu từ file packages.txt vào RAM khi mở menu
    // [FIX LOI 2] Nap du lieu tu file: neu that bai phai bao loi va thoat,
    // khong duoc de menu chay voi danh sach rong roi ghi de mat toan bo du lieu cu.
    Repository<Package> repo("data/packages.txt");
    try {
        repo.loadFromFile();
    } catch (const exception& e) {
        cout << "\n>> [LOI NGHIEM TRONG] Khong the tai du lieu goi cuoc: " << e.what() << "\n";
        cout << ">> Vui long kiem tra file data/packages.txt. Quay lai menu chinh...\n";
        return;
    }

    int choice;
    do {
        cout << "\n============ QUAN LY GOI CUOC ============\n";
        cout << "  1. Them goi cuoc moi\n";
        cout << "  2. Xem danh sach goi cuoc\n";
        cout << "  3. Tim kiem goi cuoc\n";
        cout << "  4. Sap xep danh sach goi cuoc\n";
        cout << "  5. Cap nhat thong tin goi cuoc\n";
        cout << "  6. Xoa goi cuoc\n";
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

// Hàm bao ngoài để main.cpp có thể gọi trực tiếp
void menuGoiCuoc() {
    PackageMenu::run();
}
