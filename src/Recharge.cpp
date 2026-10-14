#include "Recharge.h"
#include "Contract.h"
#include "InputHelper.h"
#include "Date.h"
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
    cout << left << setw(30) << getRefID() << " | " 
         << setw(20) << getPhoneNumber() << " | " 
         << setw(18) << getPraValue() << " | " 
         << setw(20) << getRDate() << " | " 
         << setw(20) << getPayment() << " |\n";
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
void Recharge::setRDate(const string& value){
    RDate = value;
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
bool checkPhoneNumber(const string& PhoneNumber){
    Repository<Contract> repoContract("data/Contract.txt");
    repoContract.loadFromFile();
    vector<Contract> list = repoContract.getAll();

    for(int i = 0; i < list.size(); i++){
        if(list[i].getPhoneNumber() == PhoneNumber){
            return true;
        }
    }
    return false;
}
bool checkRefID(const string& RefID, Repository<Recharge>& repo) {
    return repo.findById(RefID) == nullptr;
}
static void printTitleRecharge() {
    cout << "Ma phieu | So SIM | Menh gia (VND) | Ngay nap (DD/MM/YYYY) | Hinh thuc (Tien mat/Chuyen khoan/Vi dien tu)\n";
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
                throw InvalidDataException("Ma phieu da ton tai");
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

void menuRecharge(){
    Repository<Recharge> repo("data/recharges.txt");
    repo.loadFromFile();

    int choice = -1;
    do {
        cout << "\n========================================\n";
        cout << "     PHIEU NAP TIEN        ";
        cout << "========================================\n";
        cout << "1. Xem danh sach loai hinh\n";
        cout << "2. Them moi loai hinh\n";
        cout << "3. Tim kiem loai hinh theo ma\n";
        cout << "4. Cap nhat loai hinh\n";
        cout << "5. Xoa loai hinh\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        choice = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (choice) {
            case 1: 
                listRecharge(repo);  
                break;
            case 2:
                addRecharge(repo);
                break;
            case 3:
                cout << "Tinh nang dang cap nhat.\n";
                break;
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

