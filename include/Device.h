#pragma once
#include <iostream>
#include <string>
#include "Entity.h" // Nhung class Entity that cua nhom vao day

using namespace std;

// =========================================================
// CLASS DEVICE (Ke thua Entity)
// =========================================================
class Device : public Entity {
private:
    string a; // a: Ma IMEI
    string b; // b: Ten thiet bi

public:
    // 1. Khoi tao
    Device() { 
        a = ""; 
        b = ""; 
    }
    
    Device(string x, string y) { 
        a = x; 
        b = y; 
    }

    // 2. Getter / Setter truy xuat an toan
    string getA() const { return a; }
    void setA(string x) { a = x; }
    
    string getB() const { return b; }
    void setB(string y) { b = y; }

    // 3. Ghi de (override) cac ham tu class Entity theo chuan OOP cua nhom
    void setId(const string& x) override {
        ID = x;
        a = x; // Dung luon ID lam ma IMEI
    }

    void getDetail() const override {
        cout << "IMEI: " << a << " | Ten thiet bi: " << b << endl;
    }

    string toString() const override {
        // Tra ve chuoi phan cach boi dau | de chuan bi ghi ra file text devices.txt
        return a + "|" + b; 
    }

    // 4. Ham nhap thu cong co ban
    void nhap() {
        cout << "Nhap ma IMEI: ";
        getline(cin, a);
        cout << "Nhap ten thiet bi: ";
        getline(cin, b);
    }
};
