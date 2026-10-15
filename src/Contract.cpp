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
Contract::Contract(){}
Contract::Contract(const string& ContractID, const string& CustomerID, const string& PhoneNumber, const string& PhonePlan, const string& Date, const string& Status){
    setContractID(ContractID);
    setCustomerID(CustomerID);
    setPhoneNumber(PhoneNumber);
    setPhonePlan(PhonePlan);
    setRDate(RDate);
    setStatus(Status);
}
// ============================================================================
// GETTER/SETTER
// ============================================================================
string Contract::getContractID() const {return ContractID;}
string Contract::getCustomerID() const {return CustomerID;}
string Contract::getPhoneNumber() const {return PhoneNumber;}
string Contract::getPhonePlan() const {return PhonePlan;}
string Contract::getRDate() const {return RDate;}
string Contract::getStatus() const {return Status;}

void Contract::setContractID(const string& value){
    if(value.size() < 3 || value.substr(0, 2) != "HD")
        throw InvalidDataException("Ma hop dong phai bat dau bang HD va co so theo sau!");
    for(int i = 2; i < value.size(); i++){
        if(value[i] < '0' || value[i] > '9')
            throw InvalidDataException("Sau HD chi duoc nhap chu so!");
    }
    ContractID = value;
}
void Contract::setCustomerID(const string& value){
    if(value.size() < 3 || value.substr(0, 2) != "KH")
        throw InvalidDataException("Ma hop dong phai bat dau bang KH va co so theo sau!");
    for(int i = 2; i < value.size(); i++){
        if(value[i] < '0' || value[i] > '9')
            throw InvalidDataException("Sau KH chi duoc nhap chu so!");
    }
    CustomerID = value;
}
void Contract::setPhoneNumber(const string &value){
    if(!InputHelper::checkPhoneNumber(value))
        throw InvalidDataException("SDT phai gom 10 chu so va bat dau bang 0!");
    PhoneNumber = value;
}
void Contract::setPhonePlan(const string& value){
    if(value.size() < 3 || value.substr(0, 2) != "PKG")
        throw InvalidDataException("Goi cuoc phai bat dau bang PKG va co so theo sau!");
    for(int i = 2; i < value.size(); i++){
        if(value[i] < '0' || value[i] > '9')
            throw InvalidDataException("Sau PKG chi duoc nhap chu so!");
    }
    PhonePlan = value;
}
void Contract::setRDate(const string& value) {
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
void Contract::setStatus(const string& value){
    if(value != "Hieu luc" && value != "Het han" && value != "Da huy")
        throw InvalidDataException("Trang thai phai la Hieu luc, Het han hoac Da huy!");
    Status = value;
}
// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
string Contract::getID() const{
    return ContractID;
}
string Contract::toString() const{
    ostringstream out;
    out << ContractID << "|" << CustomerID << "|" << PhoneNumber << "|"<< PhonePlan<< "|" << RDate << "|" << Status;
    return out.str();
}
void Contract::fromString(const string& line) {
    stringstream ss(line);

    getline(ss, ContractID, '|');
    getline(ss, CustomerID, '|');
    getline(ss, PhoneNumber, '|');
    getline(ss, PhonePlan, '|');
    getline(ss, RDate, '|');
    getline(ss, Status, '|');
}
void Contract::display() const {
    cout << left
         << setw(10) << ContractID
         << setw(10) << CustomerID
         << setw(15) << PhoneNumber
         << setw(12) << PhonePlan
         << setw(15) << RDate
         << Status << '\n';
}
void Contract::getInformation() {
    setContractID(InputHelper::getString("Ma hop dong (HD + so): ", false));
    setCustomerID(InputHelper::getString("Ma khach hang (KH + so): ", false));
    setPhoneNumber(InputHelper::getString("So dien thoai: ", false));
    setPhonePlan(InputHelper::getString("Ma goi cuoc (PKG + so): ", false));
    setRDate(InputHelper::getString("Ngay dang ky (DD/MM/YYYY): ", false));
    setStatus(InputHelper::getString( "Trang thai (Hieu luc/Het han/Da huy): ", false));
}
// ============================================================================
// IN TIEU DE
// ============================================================================
static void printTitleContract() {
    cout << '\n' << string(80, '-') << '\n';

    cout << left
         << setw(10) << "Ma HD"
         << setw(10) << "Ma KH"
         << setw(15) << "So dien thoai"
         << setw(12) << "Ma goi"
         << setw(15) << "Ngay dang ky"
         << "Trang thai" << '\n';

    cout << string(80, '-') << '\n';
}
// ============================================================================
// CHECK
// ============================================================================
static bool checkPackageID(const string& packageID) {
    ifstream file("data/packages.txt");
    if (!file.is_open()) {
        throw FileException("data/packages.txt");
    }
    string line;
    while (getline(file, line)) {
        string id;
        stringstream ss(line);
        getline(ss, id, '|');

        if (id == packageID) {
            return true;
        }
    }

    return false;
}
// ============================================================================
// IN DANH SACH HOP DONG
// ============================================================================
static void listContract(Repository<Contract >& repo){
    vector<Contract> items = repo.getAll();
    printTitleContract();
    for(const auto& item : items){
        item.display();
    }
    cout << "Tong: " << items.size() << " ban ghi.\n";
}
// ============================================================================
// MENU
// ============================================================================
void menuHopDong(){
    Repository<Contract > repo("data/contracts.txt");
    repo.loadFromFile();

    int choice = -1;
    do {
        cout << "\n========================================\n";
        cout << "             HOP DONG DANG KY\n";
        cout << "========================================\n";
        cout << "1. Xem danh sach hop dong\n";
        cout << "2. Them hop dong\n";
        cout << "3. Tim hop dong theo ma\n";
        cout << "4. Cap nhat hop dong\n";
        cout << "5. Xoa hop dong\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        choice = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (choice) {
            case 1: listContract(repo); break;
            // case 2: addContract(repo); break;
            // case 3: findContract(repo); break;
            // case 4: updateContract(repo); break;
            // case 5: removeContract(repo); break;
            case 0: 
                break;
        }
    } while (choice != 0);
}