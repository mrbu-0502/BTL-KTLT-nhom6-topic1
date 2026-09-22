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
     * TODO: Viết hàm static int getInt(const std::string& prompt, 
     *                                 int minVal = std::numeric_limits<int>::min(), 
     *                                 int maxVal = std::numeric_limits<int>::max())
     * Mục đích:
     *   - In ra câu nhắc (prompt).
     *   - Nhập vào một số nguyên an toàn, có kiểm tra giới hạn trong đoạn [minVal, maxVal].
     *   - Nếu người dùng nhập chữ hoặc số ngoài khoảng, báo lỗi và bắt nhập lại (không bị lặp vô tận).
     * 
     * Gợi ý các bước thực hiện:
     *   - Dùng vòng lặp while(true):
     *       + In câu nhắc prompt ra màn hình: std::cout << prompt;
     *       + Đọc số nguyên: if (std::cin >> value) {
     *             Kiểm tra điều kiện: if (value >= minVal && value <= maxVal) {
     *                 Dọn sạch bộ đệm còn thừa sau số (clearBuffer());
     *                 return value;
     *             } else {
     *                 In thông báo lỗi ngoài khoảng [minVal, maxVal].
     *             }
     *         } else {
     *             Người dùng nhập chữ -> cin bị lỗi.
     *             std::cout << "[Loi] Vui long chi nhap so nguyen hop le!\n";
     *             Xóa cờ lỗi và dọn bộ đệm (clearBuffer()).
     *         }
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
     *  Viết hàm static double getDouble(const std::string& prompt, 
     *                                       double minVal = 0.0, 
     *                                       double maxVal = std::numeric_limits<double>::max())
     * Mục đích:
     *   - Dùng để nhập các trường tiền tệ, cước phí (GiaCuoc, CuocThang, TongTien, CuocPhi...).
     *   - Đảm bảo người dùng nhập đúng số thực và >= minVal (thường cước phí >= 0).
     * 
     * Gợi ý các bước thực hiện:
     *   - Tương tự như hàm getInt():
     *       + Vòng lặp while(true).
     *       + In prompt, kiểm tra `std::cin >> value`.
     *       + Nếu hợp lệ và thỏa mãn minVal <= value <= maxVal thì dọn bộ đệm và return.
     *       + Nếu không hợp lệ, xóa cờ lỗi, dọn bộ đệm và yêu cầu nhập lại.
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
     * Gợi ý các bước thực hiện:
     *   - Dùng vòng lặp while(true):
     *       + In prompt ra màn hình.
     *       + Đọc chuỗi bằng `std::getline(std::cin, str);`
     *       + (Tùy chọn) Viết thêm hàm trim() để cắt khoảng trắng đầu/cuối của str.
     *       + Nếu str rỗng và allowEmpty == false:
     *             In ra: "[Loi] Du lieu khong duoc de trong! Vui long nhap lai.\n";
     *         Ngược lại:
     *             return str;
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
     * TODO: Viết hàm static bool getConfirm(const std::string& prompt)
     * Mục đích:
     *   - Phục vụ chức năng XÓA (Delete): "yêu cầu xác nhận (y/n)" theo đúng yêu cầu Đề bài.
     *   - Trả về true nếu chọn 'y' hoặc 'Y', false nếu chọn 'n' hoặc 'N'.
     * 
     * Gợi ý các bước thực hiện:
     *   - In prompt ra màn hình (ví dụ: "Ban co chac chan muon xoa khong? (y/n): ").
     *   - Dùng vòng lặp:
     *       + Đọc 1 ký tự hoặc 1 chuỗi.
     *       + Nếu người dùng nhập 'y' hoặc 'Y' -> return true.
     *       + Nếu người dùng nhập 'n' hoặc 'N' -> return false.
     *       + Nếu nhập ký tự khác -> In cảnh báo "Vui long chi nhap 'y' hoac 'n'" và yêu cầu nhập lại.
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
     * Gợi ý:
     *   - In prompt kèm giá trị cũ: std::cout << prompt << " [" << oldValue << "]: ";
     *   - Đọc một dòng bằng std::getline(std::cin, line);
     *   - Nếu line rỗng (người dùng chỉ bấm Enter) -> return oldValue.
     *   - Ngược lại -> return line mới nhập.
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
     * Gợi ý:
     *   - In prompt kèm giá trị cũ.
     *   - Đọc bằng std::getline vào chuỗi line.
     *   - Nếu line rỗng -> return oldValue.
     *   - Nếu không rỗng -> chuyển chuỗi line sang số (dùng std::stod hoặc stringstream),
     *     kiểm tra hợp lệ rồi return số mới.
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


