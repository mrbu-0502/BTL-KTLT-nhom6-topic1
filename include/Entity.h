#pragma once
#include <iostream>
#include <string>

using namespace std;

class Entity 
{
protected:
    // Thuộc tính mã định danh  
    string ID;

public:
    // Constructor không có tham số: Cho phép tạo đối tượng rỗng  
    Entity() = default;

    // Constructor khởi tạo mã ID: Cho phép lớp con gọi khởi tạo mã ID nhanh
    explicit Entity(const string& id) : ID(id) {}

    // getter cho mã định danh
    virtual string getID() const
    {
        return ID;
    }
    // setter cho mã định danh 
    virtual void setID(const string& value)
    {
        ID = value;
    }
    // Destructor ảo, Đảm bảo giải phóng bộ nhớ an toàn khi hủy qua con trỏ đa hình
    virtual ~Entity() = default; 

    // Hàm thuần ảo: Nhập thông tin đối tượng từ bàn phím console
    virtual void getInformation() = 0;

    // Hàm thuần ảo: In thông tin đối tượng dạng hàng/bảng trên console
    virtual void display() const = 0;

    // Hàm thuần ảo: Chuyển dữ liệu đối tượng thành chuỗi nối bởi dấu "|" để ghi xuống file text
    virtual string toString() const = 0;

    // Hàm thuần ảo: Tách dữ liệu từ 1 dòng text (ngăn cách bởi "|") để nạp vào đối tượng
    virtual void fromString(const string& line) = 0;
};