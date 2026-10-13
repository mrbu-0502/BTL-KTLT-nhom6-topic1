/**
 * ==============================================================================
 * Người phụ trách: Lương Đức Anh (B24DCVT006)
 * ==============================================================================
 */
#pragma once
#include "Entity.h"
#include <string>

using namespace std;

class Recharge : public Entity {
private:
    string Ref;
    string PhoneNumber;
    long long PraValue;
    string Date;
    string Payment; 
public:
    Recharge();
    Recharge(const string& Ref, const string& PhoneNumber, long long PraValue, const string& Date, const string& Payment);
    
    // Getter 
    string getRef() const;
    string getPhoneNumber() const;
    long long getPraValue() const;
    string getDate() const;
    string getPayment() const;
    
    // Setter
    void setRef(string value);
    void setPhoneNumber(string value);
    void setPraValue(long long value);
    void setDate(string value);
    void setPayment(string value);

    string getID() const override;                
    string toString() const override;
    void fromString(const string& line) override;
};