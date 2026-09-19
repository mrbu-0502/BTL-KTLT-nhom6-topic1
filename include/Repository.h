#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

template <typename T>
class Repository {
private:
    std::string filePath;
    std::vector<T> items;

public:
    Repository(const std::string& path) : filePath(path) {}

    void loadFromFile() {
        items.clear();
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::ofstream newFile(filePath);
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            T obj;
            if (obj.fromFileLine(line)) {
                items.push_back(obj);
            }
        }
        file.close();
    }

    void saveToFile() const {
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << ">> Khong the ghi file vao " << filePath << "\n";
            return;
        }
        for (const auto& item : items) {
            file << item.toFileLine() << "\n";
        }
        file.close();
    }

    bool add(const T& item) {
        if (exists(item.getId())) return false;
        items.push_back(item);
        saveToFile();
        return true;
    }

    T* findById(const std::string& id) {
        for (auto& item : items) {
            if (item.getId() == id) return &item;
        }
        return nullptr;
    }

    bool exists(const std::string& id) const {
        for (const auto& item : items) {
            if (item.getId() == id) return true;
        }
        return false;
    }

    std::vector<T>& getAll() { return items; }

    bool remove(const std::string& id) {
        auto it = std::remove_if(items.begin(), items.end(), [&](const T& item) {
            return item.getId() == id;
        });
        if (it != items.end()) {
            items.erase(it, items.end());
            saveToFile();
            return true;
        }
        return false;
    }
};

#endif