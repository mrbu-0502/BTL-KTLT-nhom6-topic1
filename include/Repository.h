/**
 * ==============================================================================
 * MODULE BASE - Repository (Template chung lưu trữ và nạp/ghi file)
 * Người phụ trách: Lương Đức Anh (B24DCVT006)
 * ==============================================================================
 * 
 */

#pragma once
#include "Entity.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

template <class T>
class Repository {
private:
    vector<T> records;
    string fileName;

public:
    Repository(string file) {
        fileName = file;
    }

    vector<T> getAll() {
        return records;
    }

    T* findById(string id) {
        for (int i = 0; i < records.size(); i++) {
            if (records[i].getId() == id) {
                return &records[i];
            }
        }
        return nullptr;
    }

    void add(T item) {
        if (findById(item.getId()) != nullptr) {
            throw invalid_argument("Loi: Ma dinh danh da ton tai!");
        }
        // Giữ bản cũ để khôi phục nếu thao tác thất bại.
        vector<T> oldRecords = records;
        try {
            records.push_back(item);
            saveToFile();
        } catch (...) {
            records.swap(oldRecords);
            throw;
        }
    }

    void update(string id, T newItem) {
        // Mã định danh được giữ nguyên khi sửa.
        if (newItem.getId() != id) {
            throw invalid_argument("Loi: Khong duoc thay doi ma dinh danh!");
        }

        for (int i = 0; i < records.size(); i++) {
            if (records[i].getId() == id) {
                vector<T> oldRecords = records;
                try {
                    records[i] = newItem;
                    saveToFile();
                } catch (...) {
                    records.swap(oldRecords);
                    throw;
                }
                return;
            }
        }
        throw invalid_argument("Loi: Khong tim thay ma de cap nhat!");
    }

    void remove(string id) {
        for (int i = 0; i < records.size(); i++) {
            if (records[i].getId() == id) {
                vector<T> oldRecords = records;
                try {
                    records.erase(records.begin() + i);
                    saveToFile();
                } catch (...) {
                    records.swap(oldRecords);
                    throw;
                }
                return;
            }
        }
        throw invalid_argument("Loi: Khong tim thay ma de xoa!");
    }

    void saveToFile() {
        ofstream file(fileName);
        if (!file.is_open()) {
            throw runtime_error("Loi: Khong the mo file de ghi!");
        }

        for (int i = 0; i < records.size(); i++) {
            file << records[i].toString() << "\n";
        }
        file.close();
        if (file.fail()) {
            throw runtime_error("Loi: Ghi hoac dong file that bai!");
        }
    }

    void loadFromFile() {
        // Giữ bản cũ để khôi phục nếu thao tác thất bại.
        vector<T> oldRecords = records;

        try {
            records.clear();
            ifstream file(fileName);

            if (!file.is_open()) {
                // Tạo file nếu thiếu, không xóa nội dung file đã có.
                ofstream newFile(fileName, ios::app);
                if (!newFile.is_open()) {
                    throw runtime_error("Loi: Khong the mo hoac tao file!");
                }
                newFile.close();
                if (newFile.fail()) {
                    throw runtime_error("Loi: Khong the dong file moi!");
                }
                // Xóa trạng thái lỗi trước khi mở lại.
                file.clear();
                file.open(fileName);
                if (!file.is_open()) {
                    throw runtime_error("Loi: Khong the mo file de doc!");
                }
            }

            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;

                T item;
                item.fromString(line);
                if (findById(item.getId()) != nullptr) {
                    throw invalid_argument("Loi: Ma dinh danh bi trung trong file!");
                }
                records.push_back(item);
            }
            // Đọc đến cuối file là bình thường; lỗi đọc thì báo ra ngoài.
            if (file.bad() || (file.fail() && !file.eof())) {
                throw runtime_error("Loi: Doc file that bai!");
            }
            file.close();
        } catch (...) {
            records.swap(oldRecords);
            throw;
        }
    }
};


