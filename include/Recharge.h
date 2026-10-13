/**
 * ==============================================================================
 * Người phụ trách: Lương Đức Anh (B24DCVT006)
 * ==============================================================================
 * 
 */
#pragma once
#include "Entity.h"
#include <string>

using namespace std;

class Recharge : public Entity {
private:
    string ref;
    string phone;
    long long pravalue;
    string date;
    string payment; 
public:
    Recharge();
    Recharge(const string& ref, const string& phone, long long pravalue, const string& date, const string& payment);
    
    // Getter 
    string getref() const;
    string getphone() const;
    long long getpravalue() const;
    string getdate() const;
    string getpayment() const;
    
    // Setter
    void setref(string value);
    void setphone(string value);
    void setpravalue(long long value);
    void setdate(string value);
    void setpayment(string value);

    string getID() const override;                
    string toString() const override;
    void fromString(const string& line) override;
};
