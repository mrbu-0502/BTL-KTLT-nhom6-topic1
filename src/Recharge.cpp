#include "Recharge.h"
#include "Sim.h"
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
// Sinh ma ID Phieu nap tien
static string generateNextRechargetID(Repository<Recharge>& repo)  {
    int maxID = 0;
    for (const auto& item : repo.getAll()) {
        int currentID = stoi(item.getRefID().substr(2));
        if (currentID > maxID) {
            maxID = currentID;
        }
    }
    ostringstream nextID;
    nextID << "RC" << setfill('0') << setw(2) << (maxID + 1);
    return nextID.str();
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
    setPayment(InputHelper::getString("Hinh thuc (TienMat|ChuyenKhoan|ViDienTu): "));
}
void Recharge::display() const {
    cout << left  << setw(10) << getRefID()       << " | "
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
    if (value != "TienMat" && value != "ChuyenKhoan" && value != "ViDienTu") {
        throw InvalidDataException("Chi chap nhan: TienMat, ChuyenKhoan, ViDienTu!");
    }
    Payment = value;
}
// ============================================================================
// CHECK
// ============================================================================
static bool CheckPhoneNumberStatus(Repository<Sim>& simRepo, const string& phoneNumber) {
    Sim* sim = simRepo.findById(phoneNumber);
    if(sim == nullptr)
        throw InvalidDataException("So sim " + phoneNumber + " khong ton tai!");
    if (sim->getTrangThai() != "HoatDong")  
        throw InvalidDataException("So sim " + phoneNumber + " khong o trang thai hoat dong!");
    return true;
}
// Kiem tra xem ngay nap tien co lon hon ngay tao hop dong khong
static bool checkRechargeDate(Repository<Sim>& simRepo, const string& phoneNumber, Date d) {
    Sim* sim = simRepo.findById(phoneNumber);
    Date activated;
    activated.input_string(sim->getNgayKichHoat());
    if (d.compare(activated) < 0)
        throw InvalidDataException("Ngay nap phai tu ngay kich hoat " + activated.output() + " tro di!");
    if (d.compare(activated) > 0)
        throw InvalidDataException("Ngay nap khong duoc o tuong lai!");
    return true;
}
// ============================================================================
// IN TIEU DE
// ============================================================================
static void printTitleRecharge() {
    cout << '\n' << string(80, '-') << '\n';
    cout << left  << setw(10) << "Ma phieu"       << " | "
         << setw(13) << "So SIM"                  << " | "
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
static void addRecharge(Repository<Recharge>& rechargeRepo, Repository<Sim>& simRepo){
    Recharge item;
    string ID = generateNextRechargetID(rechargeRepo);
    cout << "\n--- THEM PHIEU NAP TIEN (Nhap 0 o bat ky buoc nao de huy) ---\n";
    // 1. Ma Phieu nap tu sinh
    cout << "Ma RC (tu dong sinh): " << ID << '\n';
    item.setRefID(ID);
    while(true){
        try{
            string text = InputHelper::getString("Nhap so sim (0 de quay lai): ", false);
            if(text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setPhoneNumber(text);
            if(CheckPhoneNumberStatus(simRepo, text)){
                break;
            }
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
            if(checkRechargeDate(simRepo,item.getPhoneNumber(),rechargeDate)) 
                break;
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Hinh thuc (TienMat|ChuyenKhoan|ViDienTu| 0 de quay lai): ");
            if(text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setPayment(text);
            break;
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    rechargeRepo.add(item);
    rechargeRepo.saveToFile();
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
static void updateRecharge(Repository<Recharge>& rechargeRepo, Repository<Sim>& simRepo){
    Recharge* found;
    string code;
    while(true){
        try{
            code = InputHelper::getString("Nhap ma phieu can sua (RC + so | 0 de quay lai): ", false);
            if(code == "0")  {return;} 
            Recharge candidate;
            candidate.setRefID(code);
        }
        catch(const exception& error){
            cout << "Loi: " << error.what() << '\n';
            continue;
        }
        found = rechargeRepo.findById(code);
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
            Recharge temp = update;
            temp.setPhoneNumber(text);
            if(CheckPhoneNumberStatus(simRepo, text)){
                update.setPhoneNumber(text);
                break;
            }
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
        string text = InputHelper::getString("Ngay nap (DD/MM/YYYY | 0 de giu nguyen): ", false);
        if(text == "0") break; 
        try {
            Recharge candidate = update;
            candidate.setRDate(text);
            Date rechargeDate;
            rechargeDate.input_string(candidate.getRDate());
            if(checkRechargeDate(simRepo, candidate.getPhoneNumber(), rechargeDate)) {
                update = candidate;
                break;
            }
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    while(true){
        try{
            string text = InputHelper::getString("Hinh thuc (TienMat|ChuyenKhoan|ViDienTu|0 de giu nguyen): ");
            if(text == "0") break;
            update.setPayment(text);
            break;
        }
        catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    rechargeRepo.update(code, update);
    rechargeRepo.saveToFile();
    cout << "Cap nhat phieu nap tien thanh cong!\n";
    printTitleRecharge();
    update.display();
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
    Repository<Recharge> rechargeRepo("data/recharges.txt");
    Repository<Sim> simRepo("data/sims.txt");
     try {
        rechargeRepo.loadFromFile();
        simRepo.loadFromFile();
    } 
    catch (const exception& e) {
        cout << "Canh bao: " << e.what() << '\n';
    }
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
            case 1: listRecharge(rechargeRepo); break;
            case 2: addRecharge(rechargeRepo, simRepo); break;
            case 3: findIDRecharge(rechargeRepo); break;
            case 4: updateRecharge(rechargeRepo, simRepo); break;
            case 5: removeRecharge(rechargeRepo); break;
            case 0: 
                break;
        }
    } while (choice != 0);
}

