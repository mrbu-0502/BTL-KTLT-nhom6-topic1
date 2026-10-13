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
    string Date;
    string Status;
public:
    Contract();
    Contract(const string& ContractID, const string& CustomerID, const string& PhoneNumber, const string& PhonePlan, const string& Date, const string& Status);

    string getContractID() const;
    string getCustomerID() const;
    string getPhoneNumber() const;
    string getPhonePlan() const;
    string getDate() const;
    string getStatus() const;

    string getID() const override;                
    string toString() const override;
    void fromString(const string& line) override;
};
