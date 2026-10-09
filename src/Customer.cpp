#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <stdexcept>
#include "Customer.h"
#include "InputHelper.h"
#include "Date.h"

using namespace std;

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ PACKAGE
// =====================================================================

Customer :: Customer()
         : Entity(""), CCCD(""), Address(""), Phone(""), Date("") {}

Customer :: Customer(string ID, string name, string cccd, string address, string phone, string date)
         : Entity(ID), FullName(name), CCCD(cccd), Address(address), Phone(phone), Date(date) {}


string Customer :: getID() const{
    return ID;
}

string Customer :: toString() const{
    stringstream ss;
    ss << ID << "|" << FullName << "|" << CCCD << "|" 
       << Address << "|" << Phone << "|" << Date;
    return ss.str();
}

void Customer :: fromString(const string& line){
    stringstream ss(line);
    getline(ss, ID, '|');
    getline(ss, FullName, '|');
    getline(ss, CCCD, '|');
    getline(ss, Address, '|');
    getline(ss, Phone, '|');
    getline(ss, Date, '|');
}

void Customer :: getInformation(){
    FullName  = InputHelper :: getString("Nhập tên khách hàng: ", false);
    CCCD      = InputHelper :: getString("Nhập căn cước công dân của khách hàng: ", false);
    Address   = InputHelper :: getString("Nhập địa chỉ khách hàng: ", 0);
    Phone     = InputHelper :: getString("Nhập số điện thoại khách hàng: ", 0);
    Date      = InputHelper :: getString("Nhập ngày sinh của khách hàng: ", 0);
}

void Customer :: display() const{
    cout << left << setw(10) << CustomerID 
         << setw(30) << FullName << right
         << setw(12) << CCCD <<
         << setw(15) << Address <<
         << setw(10) << Phone <<
         << setw(10) << Date << "\n";
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
string Customer :: getPhone() const{
    return Phone;
}
string Customer :: getDate() const{
    return Date;
}

void Customer :: setFullName(const string& n){
    if(n.empty()){
        throw invalid_argument("Tên khách hàng không được để trống !");
    }
    this -> FullName = n;
}
void Customer :: setCCCD(const string& c){
    if (c.empty()) {
        throw invalid_argument("CCCD khong duoc de trong!");
    }
    if (c.length() != 12) {
        throw invalid_argument("CCCD phai dung 12 chu so!");
    }
    for(int i = 0; i < c.length(); i++){
        if(!isdigit(c[i])){
            throw invalid_argument("Căn cước công dân không hợp lệ !");
        }
    
    this -> CCCD = c;
}

void Customer :: setAddress(const string& a){
    if(a.empty()){
        throw invalid_argument("Dia chi khong duoc de trong !");
    }
    if(a.find('|') != string :: npos){
        throw invalid_argument("Dia chi khong duoc chua ky tu dac biet '|' !");
    }
    this -> Address = a;
}

void Customer :: setPhone(const string& p){
    if(p.empty()){
        throw invalid_argument("Số điện thoại không được để trống !");
    }
    if(p.length() != 10){
        throw invalid_argument("So dien thoai phai co dung 10 chu so!");
    }
    if (p[0] != '0') {
        throw invalid_argument("So dien thoai phai bat dau bang so 0!");
    }
    for(int i = 0; i < p.length(); i++){
        if(!isdigit(p[i])){
            throw invalid_argument("Căn cước công dân không hợp lệ !");
        }
    }
    this -> Phone = p;
}

void Customer::setDate(const string& d) {
    if (d.empty()) {
        throw invalid_argument("Ngay sinh khong duoc de trong!");
    }
    int ngay, thang, nam;
    char c1 = 0, c2 = 0;
    stringstream ss(d)
    if (!(ss >> ngay >> c1 >> thang >> c2 >> nam) || c1 != '/' || c2 != '/') {
        throw invalid_argument("Ngay sinh phai dung dinh dang dd/mm/yyyy!");
    }
    date d(ngay, thang, nam);
    if (!d.valid()) {
        throw invalid_argument("Ngay sinh khong hop le hoac vuot qua nam 2026!");
    }
    this->Date = d.output();
}

// =====================================================================
// PHẦN 2: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA PACKAGEMENU
// =====================================================================

// Bước 1: Quản lý danh sách và File (đọc dữ liệu từ file văn bản vào bộ nhớ RAM (load) và ghi dữ liệu từ bộ nhớ RAM xuống file văn bản (save))

// 1. loadFile(): Đọc từng dòng từ file và nạp vào vector

void Repository<Customer> :: load(){
    items.clear();
    ifstream file(filePath);
    if(!file.is_open()){
        return;
    }
    string line;
    while(getline(file, line)){
        if(line.empty() || line[0] == "#"){
            continue;
        }
        Customer ctr;
        ctr.fromString(line);
        items.push_back(ctr);
    }
    file.close();
}

void Repository<Customer> :: save() const{
    ofstream file(filePath);
    if(!file.is_open){
        return;
    }
    for(const auto& ctr : items){
        file << ctr.toString() << "\n";
    }
    file.close();
}

// Bước 2: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA CUSTOMERMENU

void CustomerMenu::printHeader() {
    cout << string(88, '-') << "\n";
    cout << left  << setw(10) << "Ma KH"
         << setw(22) << "HO TEN"
         << setw(14) << "CCCD"
         << setw(18) << "DIA CHI"
         << setw(12) << "SDT"
         << setw(12) << "NGAY SINH" << "\n";
    cout << string(88, '-') << "\n";
}

