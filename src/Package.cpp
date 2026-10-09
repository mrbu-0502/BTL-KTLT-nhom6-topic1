#include "Package.h"
#include "InputHelper.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ PACKAGE
// =====================================================================

// Cài đặt hàm khởi tạo 
Package :: Package()
        : Entity(""), Price(0), DataMB(0), VoiceMinutes(0), SMS(0), Days(30) {}
Package :: Package(string ID, string name, double price, int data, int voice, int SMS, int days)
        : Entity(ID), PackageName(name), Price(price), DataMB(data), VoiceMinutes(voice), Days(days) {}

// Cài đặt các hàm thuần ảo 
string Package :: getID() const{
    return ID;
}

string Package :: toString() const{
    stringstream ss;
    ss << ID << "|" << PackageName << "|" 
       << fixed << setprecision(0) <<  Price << "|" 
       << DataMB << "|" << VoiceMinutes << "|" << SMS << "|" << Days;
    return ss.str();
}

void Package :: fromString(const string& line){
    stringstream ss(line);
    string PriceStr;
    string DataMBStr;
    string VoiceMinutesStr;
    string SMSStr;
    string DaysStr;

    getline(ss, ID, '|');
    getline(ss, PackageName, '|');
    getline(ss, PriceStr, '|');
    getline(ss, DataMBStr, '|');
    getline(ss, VoiceMinutesStr, '|');
    getline(ss, SMSStr, '|');
    getline(ss, DaysStr, '|');

    if(!PriceStr.empty()){
        Price = stod(PriceStr);
    }
    if(!DataMBStr.empty()){
        DataMB = stoi(DataMBStr);
    }
    if(!VoiceMinutesStr.empty()){
        VoiceMinutes = stoi(VoiceMinutesStr);
    } 
    if(!SMSStr.empty()){
        SMS = stoi(SMSStr);
    }
    if(!DaysStr.empty()){
        Days = stoi(DaysStr);
    }
}

void Package :: getInformation() const{
    PackageName  = InputHelper :: getString("Nhập tên gói cước: ", false);
    Price        = InputHelper :: getDouble("Nhập giá cước VNĐ: ", 0);
    DataMB       = InputHelper::getInt("Nhập dung lượng DataMB (MB): ", 0);
    VoiceMinutes = InputHelper::getInt("Nhập thời lượng cuộc gọi: ", 0);
    SMS          = InputHelper::getInt("Nhập số lượng SMS: ", 0);
    Days         = InputHelper::getInt("Nhập chu kỳ gói cước: ", 1);
}

void Package :: display() const{
    cout << left << setw(10) << PackageID 
         << setw(30) << PackageName << right
         << setw(10) << (long long)Price << " Đ"
         << setw(10) << DataMB << " MB"
         << setw(10) << VoiceMinutes << " P"
         << setw(10) << SMS << " tin"
         << setw(10) << Days << " ngay" << endl;
}

// Cài đặt getter
string Package :: getName() const{
    return PackageName;
} 
double Package :: getPrice() const{
    return Price;
}
int Package :: getDataMB() const{
    return DataMB;
}
int Package :: getVoiceMinutes() const{
    return VoiceMinutes;
}
int Package :: getSMS() const{
    return SMS;
}
int Package :: getDays() const{
    return Days;
}

// Cài đặt setter
void Package :: setName(const string& n){
    if (!n.empty()) {
        PackageName = n;
    }
}
void Package :: setPrice(double p){
    if(p < 0.0){
        cout << "Số tiền không thể âm, vui lòng nhập lại \n";
    }
    else{
        Price = p;
    }
}
void Package :: setDataMB(int d){
    if(d < 0){
        cout << "Dung lượng không thể âm, vui lòng nhập lại \n";
    }
    else{
        DataMB = d;
    }
}
void Package :: setVoiceMinutes(int v){
    if(v < 0){
        cout << "Thoại phút không thể âm, vui lòng nhập lại \n";
    }
    else{
        VoiceMinutes = v;
    }
}
void Package :: setSMS(int s){
    if(s < 0){
        cout << "SMS không thể âm, vui lòng nhập lại \n";
    }
    else{
        SMS = s;
    }
}
void Package :: setDays(int d){
    if(d <= 0){
        cout << "Ngày không thể âm, vui lòng nhập lại \n";
    }
    else{
        Days = d;
    }
}

// =====================================================================
// PHẦN 2: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA PACKAGEMENU
// =====================================================================

// Bước 1: Quản lý danh sách và File (đọc dữ liệu từ file văn bản vào bộ nhớ RAM (load) và ghi dữ liệu từ bộ nhớ RAM xuống file văn bản (save))

// 1. loadFile(): Đọc từng dòng từ file và nạp vào vector

void Repository<Package> :: load(){
    items.clear();            //Yêu cầu đối tượng item thực thi hàm clear() của chính nó. Xóa sạch dữ liệu cũ trong RAM (tự thu hồi bộ nhớ, không cần delete!)
    ifstream file(filePath); 
    if(!file.is_open()){
        return;               // Nếu file chưa tồn tại thì thoát ra
    }
    string line;
    while(getline(file, line)){
        if(line.empty() || line[0] == "#"){
            continue;         // Bỏ qua dòng trống hoặc dòng chú thích có dấu '#'
        }
        Package pkg;          // 1. Tạo một đối tượng bình thường (không cần 'new')
        pkg.fromString(line); // 2. Bóc tách dữ liệu vào đối tượng qua dấu chấm '.'
        items.push_back(pkg); // 3. Đẩy thẳng vào vector
    }
    file.close();
}

// 2. save(): Ghi toàn bộ dữ liệu từ vector xuống file

void Repository<Package> :: save() const{
    ofstream file(filePath);
    if(!file.is_open()){
        return;
    }
    for(const auto& pkg: items){
        file << pkg.toString() << "\n";
    }
    file.close();
}

// Bước 2: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA PACKAGEMENU

void PackageMenu::printHeader() {
    cout << string(82, '-') << "\n";
    cout << left  << setw(10) << "Ma Goi"
         << setw(22) << "Ten Goi Cuoc"
         << right << setw(12) << "Gia Cuoc"
         << setw(9)  << "Data"
         << setw(9)  << "Thoai"
         << setw(9)  << "SMS"
         << setw(11) << "Chu Ky" << "\n";
    cout << string(82, '-') << "\n";
}

