#include "Recharge.h"
#include "Contract.h"
#include "Date.h"
#include "InputHelper.h"
#include "Exceptions.h"
#include "Repository.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
// ============================================================================
// CONSTRUCTOR
// ============================================================================
Recharge::Recharge() : PraValue(0) {}
Recharge::Recharge(const string& RefID, const string& PhoneNumber, int PraValue, const string& RDate, const string& Payment){
    setRefID(RefID);
    setPhoneNumber(PhoneNumber);
    setPraValue(PraValue);
    setRDate(RDate);
    setPayment(Payment);
}
// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
string Recharge::getID() const{
    return RefID;
}
void Recharge::getInformation(){
    setRefID(InputHelper::getString("Ma Phieu (RCxxx): ",false));
    setPhoneNumber(InputHelper::getString("So Sim: ", false));
    setPraValue(InputHelper::getInt("Menh gia (VND): ", false));
    setRDate(InputHelper::getString("Ngay nap (DD/MM/YYYY): ", false));
    setPayment(InputHelper::getString("Hinh thuc (Tien mat|Chuyen khoan|Vi dien tu): "));
}
void Recharge::display() const {
    cout << left  << setw(10) << getRefID()      << " | "
         << setw(13) << getPhoneNumber()          << " | "
         << right << setw(14) << getPraValue()    << " | "
         << left  << setw(13) << getRDate()       << " | "
         << getPayment() << '\n';
}
string Recharge::toString() const{
    ostringstream out;
    out << RefID << "|" << PhoneNumber << "|" << PraValue << "|"<< RDate << "|" << Payment;
    return out.str();
}
void Recharge::fromString(const string& line) {
    stringstream ss(line);
    string temp;

    getline(ss, RefID, '|');
    getline(ss, PhoneNumber, '|');
    getline(ss, temp, '|');
    getline(ss, RDate, '|');
    getline(ss, Payment, '|');

    if (!temp.empty()) PraValue = stod(temp);
    else  PraValue = 0;
}
// ============================================================================
// GETTER & SETTER
// ============================================================================
string Recharge::getRefID() const{return RefID;}
string Recharge::getPhoneNumber() const{return PhoneNumber;}
int Recharge::getPraValue() const{return PraValue;}
string Recharge::getRDate()const {return RDate;}
string Recharge::getPayment()const {return Payment;}

void Recharge::setRefID(const string& value){
    if (value.size() < 3 || value.substr(0, 2) != "RC") {
        throw InvalidDataException("Ma phieu phai bat dau bang RC va co so theo sau!");
    }
    for (int i = 2; i < value.size(); ++i) {
        if (value[i] < '0' || value[i] > '9') {
            throw InvalidDataException("Sau RC chi duoc nhap chu so!");
        }
    }
    RefID = value;
}
void Recharge::setPhoneNumber(const string& value){
    if(!InputHelper::checkPhoneNumber(value))
        throw InvalidDataException("SDT phai gom 10 chu so va bat dau bang 0!");
    PhoneNumber = value;
}
void Recharge::setPraValue(int value){
    if (value <= 0) {
        throw InvalidDataException("So tien phai lon hon 0!");
    }
    PraValue = value;
}
void Recharge::setRDate(const string& value) {
    int day, month, year;
    char slash1, slash2;
    stringstream ss(value);

    if (!(ss >> day >> slash1 >> month >> slash2 >> year) ||
        slash1 != '/' || slash2 != '/') {
        throw InvalidDataException("Ngay nap phai co dang DD/MM/YYYY!");
    }
    ss >> ws;
    if (!ss.eof()) {
        throw InvalidDataException("Ngay nap co ky tu thua!");
    }
    Date rechargeDate(day, month, year);
    if (!rechargeDate.valid()) {
        throw InvalidDataException("Ngay nap khong hop le!");
    }

    RDate = rechargeDate.output();
}
void Recharge::setPayment(const string& value) {
    if (value != "Tien mat" && value != "Chuyen khoan" && value != "Vi dien tu") {
        throw InvalidDataException("Chi chap nhan: Tien mat, Chuyen khoan, Vi dien tu!");
    }
    Payment = value;
}
// ============================================================================
// CHECK
// ============================================================================
bool checkPhoneNumber(const string& phoneNumber) {
    ifstream file("data/contracts.txt");

    string line;
    while (getline(file, line)) {
        string contractID, customerID, phone;
        stringstream ss(line);

        if (getline(ss, contractID, '|') &&
            getline(ss, customerID, '|') &&
            getline(ss, phone, '|') &&
            phone == phoneNumber) {
            return true;
        }
    }
    return false;
}
bool checkRefID(const string& RefID, Repository<Recharge>& repo) {
    return repo.findById(RefID) == nullptr;
}
static void printTitleRecharge() {
    cout << '\n' << string(80, '-') << '\n';
    cout << left  << setw(10) << "Ma phieu"       << " | "
         << setw(13) << "So SIM"                 << " | "
         << right << setw(14) << "Menh gia (VND)" << " | "
         << left  << setw(13) << "Ngay nap"       << " | "
         << "Hinh thuc" << '\n';
    cout << string(80, '-') << '\n';
}
static void listRecharge(Repository<Recharge>& repo) {
    vector<Recharge> items = repo.getAll();
    printTitleRecharge();
    for (const auto& item : items)
        item.display();
    cout << "Tong: " << items.size() << " ban ghi.\n";
}
static void addRecharge(Repository<Recharge>& repo){
    Recharge item;
    while(true){
        try{
            string text = InputHelper::getString("Ma Phieu (RCxxx): ",false);
            if(!checkRefID(text,repo))
                throw InvalidDataException("Ma phieu da ton tai!");
            item.setRefID(text);
            break;
        }   
        catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("So Sim: ", false);
            if(!(InputHelper::checkPhoneNumber(text))){
                throw InvalidDataException("SDT phai gom 10 chu so va bat dau bang 0!");
            }
            if(!checkPhoneNumber(text)){
                 throw InvalidDataException("So dien thoai chua co hop dong dang ky!");
            }
            item.setPhoneNumber(text);
            break;
        }
        catch (const InvalidDataException& error) {                                                                                                                                                                                                                           
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            int value = InputHelper::getInt("Menh gia (VND): ", false);
            item.setPraValue(value);
            break;                                                                                                                                                                  
        }                                                       
        catch (const InvalidDataException& error) {
             cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Ngay nap (DD/MM/YYYY): ", false);
            item.setRDate(text);
            break;
        }
        catch (const InvalidDataException& error) {
        cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Hinh thuc (Tien mat|Chuyen khoan|Vi dien tu): ");
            item.setPayment(text);
            break;
        }
        catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    repo.add(item);
    cout << "Nap tien thanh cong!\n";
}
static void FindIDRecharge(Repository<Recharge>& repo){
    while (true) {
        string code = InputHelper::getString("Nhap ma phieu can tim (0 de quay lai): ", false);
        if (code == "0") return;
        try {
            Recharge candidate;
            candidate.setRefID(code);  // Kiem tra dinh dang RC + chu so
            Recharge* item = repo.findById(candidate.getID());
            if (item == nullptr) {
                cout << "Khong tim thay ma " << code
                     << ". Vui long nhap lai.\n";
                continue;
            }
            printTitleRecharge();
            item->display();
            return;
        }
        catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what()
                 << " Vui long nhap lai.\n";
        }
    }
}
void menuNapTien(){
    Repository<Recharge> repo("data/recharges.txt");
    repo.loadFromFile();

    int choice = -1;
    do {
        cout << "\n========================================\n";
        cout << "             PHIEU NAP TIEN\n";
        cout << "========================================\n";
        cout << "1. Xem danh sach phieu nap tien\n";
        cout << "2. Them phieu nap tien\n";
        cout << "3. Tim phieu nap tien theo ma\n";
        cout << "4. Cap nhat phieu nap tien\n";
        cout << "5. Xoa phieu nap tien\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        choice = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (choice) {
            case 1: listRecharge(repo); break;
            case 2: addRecharge(repo);  break;
            case 3: FindIDRecharge(repo);   break;
            case 4:
                cout << "Tinh nang dang cap nhat.\n";
                break;
            case 5:
                cout << "Tinh nang dang cap nhat.\n";
                break;
            case 0: 
                break;
        }
    } while (choice != 0);
}

