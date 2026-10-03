#pragma once
#include <string>

using namespace std;

class Entity {
public: 
    // Destructor ảo, Đảm bảo giải phóng bộ nhớ an toàn khi hủy qua con trỏ đa hình
    virtual ~Entity() = default; 

    // Hàm chỉ lấy mã định danh duy nhất ở đầu của đối tượng
    virtual string getId() const = 0;

    // Hàm chuyển dữ liệu đối tượng thành các khoảng ngăn cách bởi "|" để truyền xuống file
    virtual string toString() const = 0;

    // Hàm đọc dữ liệu từ text (ngăn cách bởi "|") để nạp vào đối tượng 
    virtual void fromString(const string& line) = 0;

};