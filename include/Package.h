#pragma once
#include "Entity.h"
#include "Repository.h"
#include <string>

using namespace std;

// ==============================================
// 1: Khai báo thực thể gói cước (Entity / Model)
// ==============================================

class Package : public Entity {
private:
    string PackageName;
    double Price;
    int DataMB;
    int VoiceMinutes;
    int SMS;
    int Days;

public:
    Package();
    Package(string ID, string name, double price, int data, int voice, int sms, int days);
    
    string getID() const override;
    string toString() const override;
    void fromString(const string& line) override;
    void getInformation() override;
    void display() const override;
    
    string getName() const;
    double getPrice() const;
    int getDataMB() const;
    int getVoiceMinutes() const;
    int getSMS() const;
    int getDays() const;

    void setName(const string& n);
    void setPrice(double p);
    void setDataMB(int d);
    void setVoiceMinutes(int v);
    void setSMS(int s);
    void setDays(int d);
};

// =====================================
// 2: Giao diện điều khiển menu console
// =====================================

class PackageMenu {
public:
    static void run(Repository<Package>& repo);

private:
    static void handleCreate(Repository<Package>& repo);
    static void handleReadAll(Repository<Package>& repo);
    static void handleSearch(Repository<Package>& repo);
    static void handleUpdate(Repository<Package>& repo);
    static void handleDelete(Repository<Package>& repo);
    static void printHeader();
};