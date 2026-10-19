/**
 * ==============================================================================
 * Người phụ trách: Lương Đức Anh (B24DCVT006)
 * ==============================================================================
 */
#pragma once
#include "Entity.h"
#include <string>

using namespace std;

class Contract : public Entity {
private:
    string ContractID;
    string CustomerID;
    string PhoneNumber;
    string PhonePlan;
    string RDate;
    string Status;
public:
    Contract();
    Contract(const string& ContractID, const string& CustomerID, const string& PhoneNumber, const string& PhonePlan, const string& RDate, const string& Status);
    //Getter
    string getContractID() const;
    string getCustomerID() const;
    string getPhoneNumber() const;
    string getPhonePlan() const;
    string getRDate() const;
    string getStatus() const;

    //Setter
    void setContractID(const string& value);
    void setCustomerID(const string& value);
    void setPhoneNumber(const string& value);
    void setPhonePlan(const string& value);
    void setRDate(const string &value);
    void setStatus(const string &value);

    string getID() const override;                
    string toString() const override;
    void fromString(const string& line) override;

    void getInformation() override;
    void display() const override;
};
