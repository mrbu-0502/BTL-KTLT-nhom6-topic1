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
    string RefID;
    string PhoneNumber;
    int PraValue;
    string RDate;
    string Payment; 
public:
    Recharge();
    Recharge(const string& RefID, const string& PhoneNumber, int PraValue, const string& RDate, const string& Payment);
    
    // Getter 
    string getRefID() const;
    string getPhoneNumber() const;
    int getPraValue() const;
    string getRDate() const;
    string getPayment() const;
    
    // Setter
    void setRefID(const string& value);
    void setPhoneNumber(const string& value);
    void setPraValue(int value);
    void setRDate(const string& value);
    void setPayment(const string& value);

    string getID() const override;                
    string toString() const override;
    void fromString(const string& line) override;

    void getInformation() override;
    void display() const override;
};