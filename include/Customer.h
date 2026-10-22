/**
 * ==============================================================================
 * BÀI TẬP LỚN: HỆ THỐNG QUẢN LÝ THUÊ BAO DI ĐỘNG (NHÓM 06 - ĐỀ TÀI 01)
 * Học phần: Kỹ thuật lập trình
 * Người thực hiện: Bùi Gia Khánh
 * MSSV: B24DCVT191
 * Module phụ trách: Entity.h (Lớp cơ sở trừu tượng)
 * Use Case: Quản lý Khách hàng (Customer)
 * ==============================================================================
 */

#pragma once
#include "Entity.h"
#include "Repository.h"
#include "Exceptions.h"
#include <string>

using namespace std;

// ==========================================
// 1. THỰC THỂ KHÁCH HÀNG (Entity / Model)
// ==========================================

class Customer : public Entity {
private:
    string FullName;
    string CCCD;
    string Address;
    string DateOfBirth;

public:
    Customer();
    Customer(string ID, string FullName, string CCCD, string Address, string Date);

    string getID() const override;
    string toString() const override;
    void fromString(const string& line) override;
    void getInformation() override;
    void display() const override;

    string getFullName() const;
    string getCCCD() const;
    string getAddress() const;
    string getDateOfBirth() const;

    void setFullName(const string& n);
    void setCCCD(const string& c);
    void setAddress(const string& a);
    void setDateOfBirth(const string& d);
};

// ===========================================
// 2. GIAO DIỆN ĐIỀU KHIỂN (Menu / Controller)
// ===========================================

class CustomerMenu {
public:
    static void run();

private:
    static void handleCreate(Repository<Customer>& repo);
    static void handleReadAll(Repository<Customer>& repo);
    static void handleSearch(Repository<Customer>& repo);
    static void handleSort(Repository<Customer>& repo);
    static void handleUpdate(Repository<Customer>& repo);
    static void handleDelete(Repository<Customer>& repo);
    static void printHeader();
};

// Khai báo hàm để main.cpp gọi trực tiếp:
void menuKhachHang();
