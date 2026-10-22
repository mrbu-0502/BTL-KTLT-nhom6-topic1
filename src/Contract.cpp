#include "Contract.h"
#include "Customer.h"
#include "Package.h"
#include "Sim.h"
#include "Date.h"
#include "InputHelper.h"
#include "Exceptions.h"
#include "Repository.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
// ============================================================================
// HAM BO TRO
// ============================================================================
static string generateNextContractID(Repository<Contract>& contractRepo) {
    int maxID = 0;
    for (const auto& item : contractRepo.getAll()) {
        int currentID = stoi(item.getContractID().substr(2));
        if (currentID > maxID) {
            maxID = currentID;
        }
    }
    ostringstream nextID;
    nextID << "HD" << setfill('0') << setw(2) << (maxID + 1);
    return nextID.str();
}
// ============================================================================
// CONSTRUCTOR
// ============================================================================
Contract::Contract() {}
Contract::Contract(const string& contractID, const string& customerID, const string& phoneNumber, const string& phonePlan, const string& rDate, const string& status) {
    setContractID(contractID);
    setCustomerID(customerID);
    setPhoneNumber(phoneNumber);
    setPhonePlan(phonePlan);
    setRDate(rDate);
    setStatus(status);
}
// ============================================================================
// GETTER/SETTER
// ============================================================================
string Contract::getContractID() const { return ContractID; }
string Contract::getCustomerID() const { return CustomerID; }
string Contract::getPhoneNumber() const { return PhoneNumber; }
string Contract::getPhonePlan() const { return PhonePlan; }
string Contract::getRDate() const { return RDate; }
string Contract::getStatus() const { return Status; }

void Contract::setContractID(const string& value) {
    if (value.size() < 3 || value.substr(0, 2) != "HD")
        throw InvalidDataException("Ma hop dong phai bat dau bang HD va co so theo sau!");
    for (int i = 2; i < value.size(); i++) {
        if (value[i] < '0' || value[i] > '9')
            throw InvalidDataException("Sau HD chi duoc nhap chu so!");
    }
    ContractID = value;
}
void Contract::setCustomerID(const string& value) {
    if (value.size() < 3 || value.substr(0, 2) != "KH")
        throw InvalidDataException("Ma khach hang phai bat dau bang KH va co so theo sau!");
    for (int i = 2; i < value.size(); i++) {
        if (value[i] < '0' || value[i] > '9')
            throw InvalidDataException("Sau KH chi duoc nhap chu so!");
    }
    CustomerID = value;
}
void Contract::setPhoneNumber(const string& value) {
    if (!InputHelper::checkPhoneNumber(value))
        throw InvalidDataException("SDT phai gom 10 chu so va bat dau bang 0!");
    PhoneNumber = value;
}
void Contract::setPhonePlan(const string& value) {
    if (value.size() < 4 || value.substr(0, 3) != "PKG")
        throw InvalidDataException("Goi cuoc phai bat dau bang PKG va co so theo sau!");
    for (int i = 3; i < value.size(); i++) {
        if (value[i] < '0' || value[i] > '9')
            throw InvalidDataException("Sau PKG chi duoc nhap chu so!");
    }
    PhonePlan = value;
}
void Contract::setRDate(const string& value) {
    Date contractDate;
    contractDate.input(value);
    RDate = contractDate.output();
}
void Contract::setStatus(const string& value) {
    if (value != "HieuLuc" && value != "HetHan" && value != "DaHuy")
        throw InvalidDataException("Trang thai phai la HieuLuc, HetHan hoac DaHuy!");
    Status = value;
}
// ============================================================================
// KE THUA TU ENTITY
// ============================================================================
string Contract::getID() const {
    return ContractID;
}
string Contract::toString() const {
    ostringstream out;
    out << ContractID << "|" << CustomerID << "|" << PhoneNumber << "|"
        << PhonePlan << "|" << RDate << "|" << Status;
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
         << setw(10) << ContractID << " | "
         << setw(10) << CustomerID << " | "
         << setw(15) << PhoneNumber << " | "
         << setw(12) << PhonePlan << " | "
         << setw(15) << RDate << " | "
         << Status << '\n';
}
void Contract::getInformation() {
    setContractID(InputHelper::getString("Ma hop dong (HD + so): ", false));
    setCustomerID(InputHelper::getString("Ma khach hang (KH + so): ", false));
    setPhoneNumber(InputHelper::getString("So dien thoai: ", false));
    setPhonePlan(InputHelper::getString("Ma goi cuoc (PKG + so): ", false));
    setRDate(InputHelper::getString("Ngay dang ky (DD/MM/YYYY): ", false));
    setStatus(InputHelper::getString("Trang thai (HieuLuc/HetHan/DaHuy): ", false));
}
// ============================================================================
// IN TIEU DE 
// ============================================================================
static void printTitleContract() {
    cout << '\n' << string(80, '-') << '\n';
    cout << left
         << setw(10) << "Ma HD" << " | "
         << setw(10) << "Ma KH" << " | "
         << setw(15) << "So dien thoai" << " | "
         << setw(12) << "Ma goi" << " | "
         << setw(15) << "Ngay dang ky" << " | "
         << "Trang thai" << '\n';
    cout << string(80, '-') << '\n';
}
static void listContract(Repository<Contract>& repo) {
    vector<Contract> items = repo.getAll();
    printTitleContract();
    for (const auto& item : items) {
        item.display();
    }
    cout << string(80, '-') << '\n';
    cout << "Tong: " << items.size() << " ban ghi.\n";
}
// ============================================================================
// THEM HOP DONG MOI
// ============================================================================
static void addContract(Repository<Contract>& contractRepo, Repository<Customer>& customerRepo, Repository<Package>& packageRepo, Repository<Sim>& simRepo) {
    Contract item;
    string ID = generateNextContractID(contractRepo);
    cout << "\n--- THEM HOP DONG MOI (Nhap 0 o bat ky buoc nao de huy) ---\n";
    // 1. Ma Hop dong tu sinh
    cout << "Ma HD (tu dong sinh): " << ID << '\n';
    item.setContractID(ID);
    // 2. Nhap Ma Khach Hang
    while (true) {
        try {
            string text = InputHelper::getString("Nhap ma khach hang (KH + so | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            // Kiem tra Ma khach hang
            item.setCustomerID(text);
            if(customerRepo.findById(text) == nullptr)
                throw InvalidDataException("Ma " + text + " khong ton tai!");
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 3. Nhap So SIM (Cho phep tai su dung SIM cua hop dong "DaHuy")
    while (true) {
        try {
            string text = InputHelper::getString("Nhap so sim (10 so | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setPhoneNumber(text);
            // Kiem tra logic So dien thoai
            Sim* sim = simRepo.findById(text);
            if(sim == nullptr){
                throw InvalidDataException("So sim " + text + " khong ton tai trong kho Sim!");
            }
            if(sim->getTrangThai() == "BiKhoa")
                 throw InvalidDataException("So sim " + text + " dang bi khoa (Locked), khong the dang ky!");
            for(const auto& temp : contractRepo.getAll()){
                if(temp.getPhoneNumber() == text && temp.getStatus() != "DaHuy"){
                    throw InvalidDataException("So dien thoai dang duoc " + temp.getCustomerID() + " su dung o hop dong " + temp.getContractID() + " (" + temp.getStatus() + ")!");
                }
            }
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 4. Nhap Goi Cuoc
    while (true) {
        try {
            string text = InputHelper::getString("Nhap ma goi cuoc (PKG + so | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }

            item.setPhonePlan(text);
            if (packageRepo.findById(text) == nullptr) {
                throw InvalidDataException("Goi cuoc " + text + " khong ton tai!");
            }
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 5. Nhap Ngay Dang Ky
    while (true) {
        try {
            string text = InputHelper::getString("Ngay dang ky (DD/MM/YYYY | 0 de quay lai): ", false);
            if (text == "0") { cout << "DaHuy thao tac them.\n"; return; }

            item.setRDate(text);
            break;
        } catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 6. Trang thai: Mac dinh hop dong moi tao phai la "HieuLuc"
    item.setStatus("HieuLuc");

    contractRepo.add(item);
    contractRepo.saveToFile();
    cout << "--> Dang ky hop dong thanh cong (Trang thai mac dinh: HieuLuc)!\n";
    printTitleContract();
    item.display();
}
// ============================================================================
// TIM KIEM HOP DONG DA NANG (THEO MA HD, MA KH, HOAC SDT)
// ============================================================================
static void findContract(Repository<Contract>& repo) {
    while (true) {
        string keyword = InputHelper::getString("Nhap Ma HD, Ma KH hoac So dien thoai can tim (0 de quay lai): ", false);
        if (keyword == "0") return;
        vector<Contract> results;
        for (const auto& item : repo.getAll()) {
            if (item.getContractID() == keyword ||item.getCustomerID() == keyword || item.getPhoneNumber() == keyword) {
                results.push_back(item);
            }
        }
        if (results.empty()) {
            cout << "Khong tim thay hop dong nao khop voi '" << keyword << "'. Vui long thu lai.\n";
            continue;
        }
        printTitleContract();
        for (const auto& item : results) {
            item.display();
        }
        cout << "Tim thay " << results.size() << " ket qua.\n";
        return;
    }
}
// ============================================================================
// CAP NHAT HOP DONG
// ============================================================================
static void updateContract(Repository<Contract>& contractRepo, Repository<Customer>& customerRepo, Repository<Package>& packageRepo, Repository<Sim>& simRepo) {
    Contract* found = nullptr;
    string code;
    while (true) {
        code = InputHelper::getString("Nhap ma hop dong can cap nhat (HD + so hoac nhap 0 de quay lai): ", false);
        if (code == "0") return;
        try {
            Contract candidate;
            candidate.setContractID(code);
        } catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
            continue;
        }
        found = contractRepo.findById(code);
        if (found != nullptr) break;
        cout << "Khong tim thay ma " << code << ". Vui long nhap lai.\n";
    }

    printTitleContract();
    found->display();

    if (found->getStatus() == "DaHuy") {
        cout << "Loi nghiep vu: Hop dong " << code << " da o trang thai 'DaHuy', khong the chinh sua!\n";
        cout << "(Neu khach hang muon su dung lai dich vu, vui long tao hop dong moi).\n";
        return;
    }
    Contract update = *found;
    // 1. Cap nhat Ma Khach Hang
    while (true) {
        string text = InputHelper::getString("Nhap ma khach hang moi (KH + so hoac nhan 0 de giu nguyen): ", false);
        Contract temp = update;
        if (text == "0") break;
        try {
            temp.setCustomerID(text);
            if (customerRepo.findById(text) == nullptr)
                throw InvalidDataException("Ma " + text + " khong ton tai!");
            update.setCustomerID(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 2. Cap nhat so dien thoai
    while (true) {
        string text = InputHelper::getString("Nhap so dien thoai moi (hoac nhan 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            Contract temp = update;
            temp.setPhoneNumber(text);
            //Kiem tra logic So dien thoai
            Sim* sim = simRepo.findById(text);
            if(sim == nullptr){
                throw InvalidDataException("So sim " + text + " khong ton tai trong kho Sim!");
            }
            if(sim->getTrangThai() == "BiKhoa")
                 throw InvalidDataException("So sim " + text + " dang bi khoa (Locked), khong the dang ky!");
            for(const auto& tmp : contractRepo.getAll()){
                if(tmp.getPhoneNumber() == text && tmp.getContractID() != code && tmp.getStatus() != "DaHuy"){
                    throw InvalidDataException("So dien thoai dang duoc " + tmp.getCustomerID() + " su dung o hop dong " + tmp.getContractID() + " (" + tmp.getStatus() + ")!");
                }
            }
            update.setPhoneNumber(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 3. Cap nhat Ma Goi Cuoc
    while (true) {
        string text = InputHelper::getString("Nhap ma goi cuoc moi (PKG + so hoac nhan 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            Contract temp = update;
            temp.setPhonePlan(text);
            // Kiem tra xem package co ton tai khong
            if (packageRepo.findById(text) == nullptr)
                throw InvalidDataException("Ma goi cuoc " + text + " khong ton tai!");
            update.setPhonePlan(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 4. Cap nhat Ngay Dang Ky
    while (true) {
        string text = InputHelper::getString("Ngay dang ky moi (DD/MM/YYYY hoac nhan 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            update.setRDate(text);
            break;
        } catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 5. Cap nhat Trang Thai
    while (true) {
        string text = InputHelper::getString("Trang thai moi (HieuLuc|HetHan|DaHuy hoac nhan 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            update.setStatus(text);
            break;
        } catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    contractRepo.update(code, update);
    contractRepo.saveToFile();
    cout << "--> Cap nhat hop dong thanh cong!\n";
    printTitleContract();
    contractRepo.findById(code)->display();
}
// ============================================================================
// XOA HOP DONG
// ============================================================================
static void removeContract(Repository<Contract>& contractRepo) {
    while (true) {
        string text = InputHelper::getString("Nhap ma hop dong can xoa (HD + so hoac nhan 0 de quay lai): ", false);
        if (text == "0") return;
        try {
            Contract candidate;
            candidate.setContractID(text);
            Contract* found = contractRepo.findById(text);
            if (found == nullptr) {
                cout << "Khong tim thay ma " << text << ". Vui long nhap lai.\n";
                continue;
            }
            printTitleContract();
            found->display();
            // Khong cho phep xoa Hop Dong dang HieuLuc
            if (found->getStatus() == "HieuLuc") {
                cout << "Loi nghiep vu: Khong the xoa hop dong dang 'HieuLuc'!\n";
                cout << "Vui long cap nhat trang thai sang 'DaHuy' hoac 'HetHan' truoc khi xoa.\n";
                return;
            }
            if (!InputHelper::getConfirm("Ban co chac muon xoa vinh vien hop dong nay? (y/n): ")) {
                cout << "Da huy thao tac xoa.\n";
                return;
            }
            contractRepo.remove(text);
            contractRepo.saveToFile();
            cout << "--> Da xoa hop dong " << text << " khoi he thong.\n";
            return;
        } catch (const InvalidDataException& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
}
// ============================================================================
// MENU
// ============================================================================
void menuHopDong() {
    Repository<Contract> contractRepo("data/contracts.txt");
    Repository<Customer> customerRepo("data/customers.txt");
    Repository<Package>  packageRepo("data/packages.txt");
    Repository<Sim>      simRepo("data/sims.txt");
    try{
        contractRepo.loadFromFile();
        customerRepo.loadFromFile();
        packageRepo.loadFromFile();
        simRepo.loadFromFile();
    } catch (const exception& e) {
        cout << "Canh bao: " << e.what() << '\n';
        return;
    }

    int choice = -1;
    do {
        cout << "\n========================================\n";
        cout << "            HOP DONG DANG KY\n";
        cout << "========================================\n";
        cout << "1. Xem danh sach hop dong\n";
        cout << "2. Them hop dong moi\n";
        cout << "3. Tim kiem hop dong (Ma HD / Ma KH / SDT)\n";
        cout << "4. Cap nhat hop dong\n";
        cout << "5. Xoa hop dong\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        choice = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (choice) {
            case 1: listContract(contractRepo); break;
            case 2: addContract(contractRepo, customerRepo, packageRepo, simRepo); break;
            case 3: findContract(contractRepo); break;
            case 4: updateContract(contractRepo, customerRepo, packageRepo, simRepo); break;
            case 5: removeContract(contractRepo); break;
            case 0: break;
        }
    } while (choice != 0);
}