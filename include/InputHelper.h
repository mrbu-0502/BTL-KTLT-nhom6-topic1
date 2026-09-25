/**
 * ==============================================================================
 * MODULE BASE - INPUT HELPER (TIỆN ÍCH NHẬP LIỆU AN TOÀN TRÊN CONSOLE)
 * Người phụ trách: Nguyễn Văn Cường (B24DCVT053)
 * Mục đích:
 *   - Xử lý nhập số, nhập chuỗi an toàn, chống trôi lệnh (cin / getline).
 *   - Không bị treo hoặc văng ứng dụng khi người dùng nhập sai kiểu dữ liệu.
 * ==============================================================================
 * 
 */

#pragma once
#include <iostream>
#include <string>
#include <limits>
using namespace std;


class InputHelper {
public:
    // ==========================================================================
    // 1. DỌN BỘ NHỚ ĐỆM (CLEAR BUFFER)
    // ==========================================================================
    /**
     *  Viết hàm static void clearBuffer()
     * Mục đích: Xóa sạch các ký tự còn sót lại trong bộ đệm bàn phím (đặc biệt là '\n').
     * Khi nào dùng: Gọi sau khi dùng 'std::cin >> biến_số' trước khi chuyển sang 'std::getline'.
     * 
     * Gợi ý các bước thực hiện:
     *   - Bước 1: Gọi std::cin.clear() để xóa cờ lỗi (nếu có).
     *   - Bước 2: Dùng std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')
     *             để bỏ qua toàn bộ ký tự trong hàng đợi bàn phím đến khi gặp ký tự xuống dòng '\n'.
     */
    static void XoaBoDem() {
        cin.clear();
        cin.ignore();
    }


    // ==========================================================================
    // 2. NHẬP SỐ NGUYÊN AN TOÀN (GET INT)
    // ==========================================================================
    /**
     * Mục đích:
     *   - In ra câu nhắc (prompt).
     *   - Nhập vào một số nguyên an toàn, có kiểm tra giới hạn trong đoạn [minVal, maxVal].
     *   - Nếu người dùng nhập chữ hoặc số ngoài khoảng, báo lỗi và bắt nhập lại (không bị lặp vô tận).
     * 
     */
    static int getInt(const string& CauThongBao, int minVal = numeric_limits<int>::min(), int maxVal = numeric_limits<int>::max()) {
        while(true){
            cout << CauThongBao;
            int value;
            if(cin >> value) {
                if(value <= maxVal && value >= minVal){
                    XoaBoDem();
                    return value;
                }
            }
        }
    }


    // ==========================================================================
    // 3. NHẬP SỐ THỰC AN TOÀN (GET DOUBLE)
    // ==========================================================================
    /**
     * Mục đích:
     *   - Dùng để nhập các trường tiền tệ, cước phí (GiaCuoc, CuocThang, TongTien, CuocPhi...).
     *   - Đảm bảo người dùng nhập đúng số thực và >= minVal (thường cước phí >= 0).
     * 
     */
    static double getDouble(const string& CauThongBao, double minVal = 0.0, double maxVal = numeric_limits<double>::max()) {
        while(true){
            cout << CauThongBao;
            double value;
            if(cin >> value){
                if(value <= maxVal && value >= minVal){
                    XoaBoDem();
                    return value;
                }
            }
        }
    }


    // ==========================================================================
    // 4. NHẬP CHUỖI AN TOÀN - CHỐNG TRÔI LỆNH (GET STRING)
    // ==========================================================================
    /**
     *  Viết hàm static std::string getString(const std::string& prompt, bool allowEmpty = false)
     * Mục đích:
     *   - Nhập một chuỗi ký tự (có thể chứa khoảng trắng như Họ tên, Địa chỉ, Nội dung khiếu nại).
     *   - Chống hoàn toàn hiện tượng trôi lệnh khi trước đó có lệnh cin >>.
     *   - Nếu allowEmpty == false: không cho phép người dùng chỉ ấn Enter bỏ trống.
     * 
     */
    static string getString(const string& CauThongBao, bool allowEmpty) {
        while(true){
            cout << CauThongBao;
            string s;
            getline(cin, s);
            if(s.empty() && allowEmpty == false){
                cout << "[Loi] Du lieu khong duoc bo trong! Vui long nhap lai." << endl;
            } 
            return s;
        }
    }


    // ==========================================================================
    // 5. NHẬP XÁC NHẬN CÓ / KHÔNG (GET CONFIRM Y/N)
    // ==========================================================================
    /**
     * Viết hàm static bool getConfirm(const std::string& prompt)
     * Mục đích:
     *   - Phục vụ chức năng XÓA (Delete): "yêu cầu xác nhận (y/n)" theo đúng yêu cầu Đề bài.
     *   - Trả về true nếu chọn 'y' hoặc 'Y', false nếu chọn 'n' hoặc 'N'.
     * 
     */
    static bool getConfirm(const string& CauThongBao) {
        cout << CauThongBao;

        while(true){
            char confirm;
            if(cin >> confirm){
                if(confirm == 'Y' || confirm == 'y') return true;
                if(confirm == 'N' || confirm == 'n') return false;

                cout << "Vui long chi nhap y/n";
            }
        }
    }


    // ==========================================================================
    // 6. TIỆN ÍCH HỖ TRỢ CHỨC NĂNG SỬA (UPDATE - GIỮ NGUYÊN GIÁ TRỊ CŨ KHI BỎ TRỐNG)
    // ==========================================================================
    /**
     * Theo yêu cầu đề bài: "cho phép sửa từng trường (bỏ trống nghĩa là giữ nguyên)".
     * 
     * Viết hàm static std::string getStringOrDefault(const std::string& prompt, const std::string& oldValue)
     */
    static std::string getStringOrDefault(const string& CauThongBao, const string& oldstring) {
        cout << CauThongBao << oldstring<< endl;
        string newstring;
        getline(cin, newstring);
        if(newstring.empty()) return oldstring;
        else return newstring;
    }

    /**
     *  Viết hàm static double getDoubleOrDefault(const std::string& prompt, double oldValue)

     */
    static double getDoubleOrDefault(const string& CauThongBao, double oldValue) {
        cout << CauThongBao << oldValue << endl;
        string newstring;
        double newvalue;
        getline(cin, newstring);
        if(newstring.empty()) return oldValue;
        else {
            newvalue = stod(newstring);
            if(newvalue >= 0.0 && newvalue <= numeric_limits<double>::max()) return newvalue;
        }
    }

    /**
     * Viết hàm static int getIntOrDefault(const std::string& prompt, int oldValue)
     * Gợi ý: Tương tự getDoubleOrDefault, nhưng dùng std::stoi để ép kiểu.
     */
    static int getIntOrDefault(const string& CauThongBao, int oldValue) {
        cout << CauThongBao << oldValue << endl;
        string newstring;
        int newvalue;
        getline(cin, newstring);
        if(newstring.empty()) return oldValue;
        else {
            newvalue = stoi(newstring);
            if(newvalue >= 0 && newvalue <= numeric_limits<int>::max()) return newvalue;
        }
    }
};


