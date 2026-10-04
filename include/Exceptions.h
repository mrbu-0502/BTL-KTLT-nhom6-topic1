#include <iostream>
#include <string>
#include "Exceptions.h" // Nhúng file ngoại lệ bạn vừa tạo

using namespace std;

// 1. Hàm thêm SIM (Nơi có nguy cơ phát sinh lỗi)
void themSimMoi(string soSim) {
    // Giả sử hệ thống đã có sẵn số 0987654321
    if (soSim == "0987654321") {
        // NÉM LỖI: Lập tức dừng mọi việc và ném ra cái lỗi trùng ID
        throw DuplicateIdException(soSim); 
    }
    
    // Nếu không ném lỗi (tức là số mới tinh), code sẽ chạy tiếp xuống đây
    cout << "Da them SIM " << soSim << " thanh cong vao he thong!\n";
}

// 2. Chạy thử chương trình
int main() {
    cout << "--- BAT DAU THEM SIM ---\n";
    
    try {
        // Khối TRY: Nơi chứa những câu lệnh "có rủi ro" bị lỗi
        themSimMoi("0987654321"); // Thử thêm số đã tồn tại
        
        // Dòng dưới này sẽ KHÔNG được chạy vì dòng trên đã ném lỗi
        cout << "Ghi vao file txt...\n"; 
        
    } catch (const BaseException& e) {
        // Khối CATCH: Nơi "chụp" lấy cái lỗi vừa bị ném ra
        // Bất kể là lỗi trùng ID, hay lỗi file, nó đều rớt vào đây
        cout << "!!! MAY QUA KHONG BI VANG CHUONG TRINH !!!\n";
        cout << "He thong bao: " << e.what() << "\n";
    }
    
    cout << "--- CHUONG TRINH VAN CHAY TIEP BINH THUONG ---\n";
    return 0;
}