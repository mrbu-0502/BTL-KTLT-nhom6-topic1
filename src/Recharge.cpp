#include "Recharge.h"
#include "InputHelper.h"
#include "Date.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Recharge::Recharge() : price(0) {}
Recharge::Recharge(const string& RefID, const string& PhoneNumber, long long PraValue, const string& Date, const string& Payment){
    setref(RefID);
    setphone(PhoneNumber);
    setpra(PraValue);
    setdate(Date);
    setpayment(Payment);
}

string Recharge::getRefID(){return RefID;}
string Recharge::getPhoneNumber(){return PhoneNumber;}
string Recharge::getPraValue(){return PraValue;}
string Recharge::getDate(){return Date;}
string Recharge::getPayment(){return Payment;}

/*void Recharge::setRefID(string value){

}*/

void Recharge::setPhoneNumber(const string& value){
    PhoneNumber = value;
}

