#include "Device.h"      // Nhap ban ve Device tu thu muc include
#include "InputHelper.h" // Nhap cong cu ho tro nhap lieu an toan cua Cuong

using namespace std;

// =========================================================================
// PHAN 1: KHOI TAO THIET BI (CONSTRUCTOR)
// Dung de tao ra doi tuong ngay khi chuong trinh vua chay
// =========================================================================

// Ham khoi tao mac dinh (Chay khi tao mot thiet bi moi tinh ma chua co thong tin)
Device::Device() {
    a = ""; // De trong ma IMEI
    b = ""; // De trong ten thiet bi
}

// Ham khoi tao co tham so (Chay khi muon tao thiet bi va gan luon du lieu x, y vao)
Device::Device(string x, string y) {
    a = x;  // Gan chuoi x vao bien a (Ma IMEI)
    b = y;  // Gan chuoi y vao bien b (Ten thiet bi)
    ID = x; // Trong do an nay, ID he thong chinh la ma IMEI
}

// =========================================================================
// PHAN 2: CAC HAM GET/SET DE LAY HOAC SUA DU LIEU (DUNG DE BAO MAT)
// =========================================================================

// Ham getA: Goi ham nay khi muon lay ma IMEI ra de xem
string Device::getA() const { 
    return a; 
}

// Ham setA: Goi ham nay khi muon sua ma IMEI thanh mot ma khac (x)
void Device::setA(string x) { 
    a = x;  // Sua ma IMEI
    ID = x; // Cap nhat luon ID cho dong bo voi ma IMEI moi
}

// Ham getB: Goi ham nay khi muon lay ten thiet bi ra de xem
string Device::getB() const { 
    return b; 
}

// Ham setB: Goi ham nay khi muon sua ten thiet bi thanh ten khac (y)
void Device::setB(string y) { 
    b = y; 
}

// =========================================================================
// PHAN 3: GHI DE CAC HAM CUA CLASS CHA "ENTITY" DE CHUAN HOA VOI NHOM
// =========================================================================

// 1. Ham cai dat ID (Ham nay do nhom truong Khanh yeu cau co)
void Device::setID(const string& value) {
    ID = value; // Gan ID chung cua he thong
    a = value;  // Dong bo ma IMEI (a) giong y het ID do
}

// 2. Ham nhap du lieu tu ban phim (Thay the cho viec dung cin >> gay troi lenh)
void Device::getInformation() {
    // Dung InputHelper::getString de man hinh dung lai cho minh nhap an toan
    a = InputHelper::getString("Nhap ma IMEI: ");
    ID = a; // Nhap IMEI xong thi cho bien ID bang luon ma IMEI vua nhap
    
    // Tiep tuc cho nhap ten may
    b = InputHelper::getString("Nhap ten thiet bi: ");
}

// 3. Ham in thong tin ra man hinh console cho dep
void Device::display() const {
    // In ra theo mau: "IMEI: 123456 | Ten thiet bi: Samsung S24"
    cout << "IMEI: " << a << " | Ten thiet bi: " << b << endl;
}

// 4. Ham ghep 2 bien a va b thanh 1 chuoi lien nhau de luu vao file
string Device::toString() const {
    // Dung dau "+" de noi chuoi. Ket qua se ra dang: "123456789|iPhone 15"
    return a + "|" + b;
}

// 5. Ham boc tach chuoi doc duoc tu file text de nhet lai vao may tinh
// Tham so "line" la 1 dong text no doc duoc. Vi du line = "86123|Oppo"
void Device::fromString(const string& line) {
    
    // Buoc 1: Tim xem cai dau gach dung '|' no nam o vi tri thu may
    int vitri = line.find('|');

    // Buoc 2: Cat tu dau (vi tri 0) lay dung so luong chu cai bang 'vitri', roi gan vao a
    // Voi vi du "86123|Oppo", no se cat lay chu "86123" roi luu vao bien a (Ma IMEI)
    a = line.substr(0, vitri);

    // Buoc 3: Cat tu ngay sau dau '|' (tuc la vitri + 1) cho den het, roi gan vao b
    // No se cat lay chu "Oppo" roi luu vao bien b (Ten thiet bi)
    b = line.substr(vitri + 1);

    // Buoc 4: Lay ma IMEI (a) vua tach duoc gan cho bien ID cua he thong
    ID = a; 
}
