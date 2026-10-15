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
    setRefID(InputHelper::getString("Ma Phieu (RC + so): ",false));
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

    if (!temp.empty()) PraValue = stoi(temp);
    else PraValue = 0;
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
bool checkPhoneNumberContract(const string& phoneNumber) {
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
// ============================================================================
// IN TIEU DE
// ============================================================================
static void printTitleRecharge() {
    cout << '\n' << string(80, '-') << '\n';
    cout << left  << setw(10) << "Ma phieu"       << " | "
         << setw(13) << "So SIM"                 << " | "
         << right << setw(14) << "Menh gia (VND)" << " | "
         << left  << setw(13) << "Ngay nap"       << " | "
         << "Hinh thuc" << '\n';
    cout << string(80, '-') << '\n';
}
// ============================================================================
// IN DANH SACH
// ============================================================================
static void listRecharge(Repository<Recharge>& repo) {
    vector<Recharge> items = repo.getAll();
    printTitleRecharge();
    for (const auto& item : items)
        item.display();
    cout << "Tong: " << items.size() << " ban ghi.\n";
}
// ============================================================================
// THEM MA PHIEU
// ============================================================================
static void addRecharge(Repository<Recharge>& repo){
    Recharge item;
    while(true){
        try{
            string text = InputHelper::getString("Ma phieu (RC + so): ",false);
            Recharge* found = repo.findById(text);
            if(found != nullptr)
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
            item.setPhoneNumber(text);
            if(!checkPhoneNumberContract(text)){
                 throw InvalidDataException("So dien thoai chua co hop dong dang ky!");
            }
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
    repo.saveToFile();
    cout << "Nap tien thanh cong!\n";
}
// ============================================================================
// TIM THEO MA PHIEU
// ============================================================================
static void findIDRecharge(Repository<Recharge>& repo){
    while (true) {
        string text = InputHelper::getString("Nhap ma phieu can tim (0 de quay lai): ", false);
        if (text == "0") return;
        try {
            Recharge candidate;
            candidate.setRefID(text);
            Recharge* item = repo.findById(text);
            if (item == nullptr) {
                cout << "Khong tim thay ma " << text << ". Vui long nhap lai.\n";
                continue;
            }
            printTitleRecharge();
            item->display();
            return;
        }
        catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << " Vui long nhap lai.\n";
        }
    }
}
// ============================================================================
// CAP NHAT MA PHIEU
// ============================================================================
static void updateRecharge(Repository<Recharge>& repo){
    Recharge* found;
    string code;
    while(true){
        try{
            code = InputHelper::getString("Nhap ma phieu can sua (RC + so): ", false);
            Recharge candidate;
            candidate.setRefID(code);
        }
        catch(const InvalidDataException& error){
            cout << "Loi: " << error.what() << '\n';
            continue;
        }
        found = repo.findById(code);
        if(found != nullptr) break;
        cout << "Khong tim thay ma " << code << ". Vui long nhap lai.\n";
    }
    printTitleRecharge();
    found->display();
    
    Recharge update = *found;
    while(true){
        try{
            string text = InputHelper::getString("Nhap so dien thoai moi: ", false);
            update.setPhoneNumber(text);
            if(!checkPhoneNumberContract(text)){
                 throw InvalidDataException("So dien thoai chua co hop dong dang ky!");
            }
            break;   
        }
        catch(const InvalidDataException& error){
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            int value = InputHelper::getInt("Nhap menh gia moi (VND): ", false);
            update.setPraValue(value);
            break;
        }
        catch(const InvalidDataException& error){
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Ngay nap (DD/MM/YYYY): ", false);
            update.setRDate(text);
            break;
        }
        catch(const InvalidDataException& error){
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Hinh thuc (Tien mat|Chuyen khoan|Vi dien tu): ");
            update.setPayment(text);
            break;
        }
        catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    repo.update(code, update);
    cout << "Cap nhat phieu nap tien thanh cong!\n";
    printTitleRecharge();
    repo.saveToFile();
    repo.findById(code)->display();
}
// ============================================================================
// XOA MA PHIEU
// ============================================================================
static void removeRecharge(Repository<Recharge>& repo) {
    while (true) {
        string text = InputHelper::getString("Nhap ma phieu can xoa (RC + so hoac nhap 0 de quay lai): ", false);
        if (text == "0") return;
        try {
            Recharge candidate;
            candidate.setRefID(text); 
            Recharge* item = repo.findById(text);
            if (item == nullptr) {
                cout << "Khong tim thay phieu " << text << ". Vui long nhap lai.\n";
                continue;
            }
            printTitleRecharge();
            item->display();
            if (!InputHelper::getConfirm("Ban co chac muon xoa phieu nay? (y/n): ")) {
                cout << "Da huy thao tac xoa.\n";
                return;
            }
            repo.remove(text);
            repo.saveToFile();
            cout << "Da xoa phieu nap tien " << text << ".\n";
            return;
        } catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
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
            case 2: addRecharge(repo); break;
            case 3: findIDRecharge(repo); break;
            case 4: updateRecharge(repo); break;
            case 5: removeRecharge(repo); break;
            case 0: 
                break;
        }
    } while (choice != 0);
}

