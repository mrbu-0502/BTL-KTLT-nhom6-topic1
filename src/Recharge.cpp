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
// HAM BO TRO
// ============================================================================
// Sinh ma ID Hop dong
static string generateNextRechargetID(Repository<Recharge>& repo) {
    int count = 1;
    while (true) {
        ostringstream nextID;
        nextID << "RC" << setfill('0') << setw(2) << count;
        if (repo.findById(nextID.str()) == nullptr) {
            return nextID.str();
        }
        count++;
    }
}
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
    Date rechargeDate;
    rechargeDate.input(value);
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
static bool CheckPhoneNumberStatus(const string& phoneNumber) {
    string filename = "data/contracts.txt";
    ifstream file(filename);
      if (!file.is_open()) {
        throw FileException(filename);
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string contractID, customerID, phone;
        string phonePlan, registrationDate, status;
        if (!(getline(ss, contractID, '|') &&
              getline(ss, customerID, '|') &&
              getline(ss, phone, '|') &&
              getline(ss, phonePlan, '|') &&
              getline(ss, registrationDate, '|') &&
              getline(ss, status, '|'))) {
            continue;
        }
        if (phone == phoneNumber) {
            if (!status.empty() && status.back() == '\r') {
                status.pop_back();
            }
            if(status == "Da huy")
                return false;
            return true;
        }
    }
    throw InvalidDataException(
        "So sim " + phoneNumber + " chua co hop dong dang ky!");
}
// Kiem tra xem ngay nap tien co lon hon ngay tao hop dong khong
static bool checkRechargeDate(Repository<Contract>& contractRepo,const string& PhoneNumber, Date rechargeDate) {
    Date temp;
    for (const auto& item : contractRepo.getAll()) {
        if (PhoneNumber == item.getPhoneNumber()) {
            temp.input_string(item.getRDate());
            break;
        }
    }
    if (rechargeDate.compare(temp) > 0) {
        return true;
    }
    throw InvalidDataException("Ngay nap tien phai lon hon ngay tao hop dong!");
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
static void addRecharge(Repository<Recharge>& repo, Repository<Contract>& contractRepo){
    Recharge item;
    string ID = generateNextRechargetID(repo);
    cout << "\n--- THEM PHIEU NAP TIEN (Nhap 0 o bat ky buoc nao de huy) ---\n";
    // 1. Ma Phieu nap tu sinh
    cout << "Ma RC (tu dong sinh): " << ID << '\n';
    item.setRefID(ID);
    while(true){
        try{
            string text = InputHelper::getString("Nhap so sim (0 de quay lai): ", false);
            if(text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setPhoneNumber(text);
            if(!CheckPhoneNumberStatus(text)){
                 throw InvalidDataException("Hop dong cua so dien thoai o trang thai Da huy, khong the nap tien!");
            }
            break;
        }
        catch (const exception& error) {                                                                                                                                                                                                                           
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            int value = InputHelper::getInt("Menh gia (VND | 0 de quay lai): ", false);
            if(value == 0) { cout << "Da huy thao tac them.\n"; return; }
            item.setPraValue(value);
            break;                                                                                                                                                                  
        }                                                       
        catch (const exception& error) {
             cout << "Loi: " << error.what() << '\n';
        }
    }
    while (true) {
        try {
            string text = InputHelper::getString("Ngay nap (DD/MM/YYYY | 0 de giu nguyen): ", false);
            if(text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setRDate(text);
            Date rechargeDate;
            rechargeDate.input_string(item.getRDate());
            if(checkRechargeDate(contractRepo,item.getPhoneNumber(),rechargeDate)) break;
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Hinh thuc (Tien mat|Chuyen khoan|Vi dien tu | 0 de quay lai): ");
            if(text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setPayment(text);
            break;
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    repo.add(item);
    repo.saveToFile();
    cout << "Nap tien thanh cong!\n";
    printTitleRecharge();
    item.display();
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
        catch (const exception& error) {
            cout << "Loi: " << error.what() << " Vui long nhap lai.\n";
        }
    }
}
// ============================================================================
// CAP NHAT MA PHIEU
// ============================================================================
static void updateRecharge(Repository<Recharge>& repo, Repository<Contract>& contractRepo){
    Recharge* found;
    string code;
    while(true){
        try{
            code = InputHelper::getString("Nhap ma phieu can sua (RC + so | 0 de quay lai): ", false);
            if(code == "0")  return; 
            Recharge candidate;
            candidate.setRefID(code);
        }
        catch(const exception& error){
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
            string text = InputHelper::getString("Nhap so dien thoai moi (10 so | 0 de de giu nguyen): ", false);
            if(text == "0") break;
            update.setPhoneNumber(text);
            if(!CheckPhoneNumberStatus(text)){
                throw InvalidDataException("Hop dong cua so dien thoai o trang thai Da huy, khong the nap tien!");
            }
            break;   
        }
        catch(const exception& error){
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            int value = InputHelper::getInt("Nhap menh gia moi (VND | 0 de giu nguyen): ", false);
            if(value == 0) break;
            update.setPraValue(value);
            break;
        }
        catch(const exception& error){
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while (true) {
        try {
            string text = InputHelper::getString("Ngay nap (DD/MM/YYYY | 0 de giu nguyen): ", false);
            if(text == "0") break; 
            update.setRDate(text);
            Date rechargeDate;
            rechargeDate.input_string(update.getRDate());
            if(checkRechargeDate(contractRepo,update.getPhoneNumber(),rechargeDate)) break;
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Hinh thuc (Tien mat|Chuyen khoan|Vi dien tu|0 de giu nguyen): ");
            if(text == "0") break;
            update.setPayment(text);
            break;
        }
        catch (const exception& error) {
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
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        } 
    }
}
void menuNapTien(){
    Repository<Recharge> repo("data/recharges.txt");
    repo.loadFromFile();
    Repository<Contract> contractRepo("data/contracts.txt");
    contractRepo.loadFromFile();
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
            case 2: addRecharge(repo, contractRepo); break;
            case 3: findIDRecharge(repo); break;
            case 4: updateRecharge(repo, contractRepo); break;
            case 5: removeRecharge(repo); break;
            case 0: 
                break;
        }
    } while (choice != 0);
}

