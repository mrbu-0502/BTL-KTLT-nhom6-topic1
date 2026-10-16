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
    Customer(string FullName, string CCCD, string Address, string Date);

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
    void setCCCD(const string& c);;
    void setAddress(const string& a);
    void setDateOfBirth(const string& d);
};

// ===========================================
// 2. GIAO DIỆN ĐIỀU KHIỂN (Menu / Controller)
// ===========================================

class CustomerMenu{
public:
    static void run(Repository<Customer>& Repo);
private:
    static void handleCreate(Repository<Customer>& Repo);
    static void handleReadAll(Repository<Customer>& Repo);
    static void handleSearch(Repository<Customer>& Repo);
    static void handleUpdate(Repository<Customer>& Repo);
    static void handleDelete(Repository<Customer>& Repo);
    static void printHeader();
};