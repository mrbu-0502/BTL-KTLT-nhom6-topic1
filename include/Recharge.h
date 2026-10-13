#pragma once
#include "Entity.h"
#include <string>

using namespace std;

class Recharge : public Entity {
private:
    string maPhieu;
    string soSim;
    long long menhGia;
    string ngayNap;
    string hinhThuc; 
public:
    Recharge();
    Recharge(const string& maPhieu, const string& soSim, long long menhGia, const string& ngayNap, const string& hinhThuc);
    
    // Getter 
    string getmaPhieu() const;
    string getsoSim() const;
    long long getmenhGia() const;
    string getngayNap() const;
    string gethinhThuc() const;
    
    // Setter
    void setmaPhieu(string value);
    void setsoSim(string value);
    void setmenhGia(long long value);
    void setngayNap(string value);
    void sethinhThuc(string value);

    string getID() const override;                
    string toString() const override;
    void fromString(const string& line) override;
};