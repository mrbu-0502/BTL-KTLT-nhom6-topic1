#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <iostream>
#include <string>
#include <limits>

class InputHelper {
public:
    static int getInt(const std::string& prompt, int minVal = 0, int maxVal = 1000000000) {
        int val;
        while (true) {
            std::cout << prompt;
            if (std::cin >> val && val >= minVal && val <= maxVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
            std::cout << ">> Gia tri khong hop le! Vui long nhap so nguyen tu " << minVal << " den " << maxVal << ":\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    static double getDouble(const std::string& prompt, double minVal = 0.0) {
        double val;
        while (true) {
            std::cout << prompt;
            if (std::cin >> val && val >= minVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
            std::cout << ">> Gia tri khong hop le! Vui long nhap so thuc >= " << minVal << ":\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    static std::string getString(const std::string& prompt, bool allowEmpty = false) {
        std::string s;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, s);
            if (allowEmpty || !s.empty()) return s;
            std::cout << ">> Khong duoc de trong du lieu! Vui long nhap lai:\n";
        }
    }
};

#endif