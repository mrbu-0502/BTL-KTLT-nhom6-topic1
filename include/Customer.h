#pragma once
#include "Entity.h"
#include "Repository.h"
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

// Trong class CustomerMenu:
class CustomerMenu {
public:
    static void run(); // Bỏ tham số Repository<Customer>& Repo
    // ... các hàm handleCreate, handleReadAll... giữ nguyên
};

// Khai báo hàm để main.cpp gọi trực tiếp:
void menuKhachHang();