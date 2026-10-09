#pragma once
#include "Entity.h"
#include "Repository.h"
#include <string>

using namespace std;

// ==============================================
// 1: Khai báo thực thể gói cước (Entity / Model)
// ==============================================

class Package : public Entity{
private:
    string PackageName;
    double Price;
    int DataMB;
    int VoiceMinutes;
    int SMS;
    int Days;

public:
    Package();
    Package(string PackageID, string PackageName, double Price, int DataMB, int VoiceMinutes, int SMS, int Days);
    
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

class PackageMenu{
public:
    static void run(Repository<Package>& Repo);
private:
    static void handleCreate(Repository<Package>& Repo);
    static void handleReadALL(Repository<Package>& Repo);
    static void handleSearch(Repository<Package>& Repo);
    static void handleSort(Repository<Package>& Repo);
    static void handleUpdate(Repository<Package>& Repo);
    static void handleDelete(Repository<Package>& Repo);
    static void printHeader();
};