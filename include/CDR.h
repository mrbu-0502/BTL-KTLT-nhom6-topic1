#pragma once
#include <iostream>
#include <string>
#include "Entity.h" 
// #include "Date.h" // Neu cau muon dung class Date cua cau vao day thi mo // ra

using namespace std;

class CDR : public Entity {
private:
    // 6 bien tuong ung voi 6 cot trong file cdrs.txt
    string a; // Ma CDR (VD: CDR01)
    string b; // So dien thoai goi (VD: 0981000000)
    string c; // So dien thoai nhan (VD: 0912345678)
    string d; // Thoi diem goi (VD: 01/01/2024 08:30:15)
    string e; // Thoi luong tinh bang giay (VD: 120)
    string f; // Cuoc phi (VD: 0)

public:
    // Ham khoi tao
    CDR();

    // Cac ham Get/Set co ban de lay va sua du lieu (Toi chi khai bao ten)
    string getA() const; void setA(string x);
    string getB() const; void setB(string x);
    string getC() const; void setC(string x);
    string getD() const; void setD(string x);
    string getE() const; void setE(string x);
    string getF() const; void setF(string x);

    // 5 ham BAT BUOC phai ghi de tu class cha Entity
    void setID(const string& value) override;
    void getInformation() override;
    void display() const override;
    string toString() const override;
    void fromString(const string& line) override;
};
