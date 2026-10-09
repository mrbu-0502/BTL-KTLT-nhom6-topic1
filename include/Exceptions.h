#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// Lop co so: moi loi cua du an deu ke thua lop nay
// -> chi can catch (const BaseException&) la bat duoc tat ca
class BaseException : public std::exception {
protected:
    std::string msg;
public:
    explicit BaseException(const std::string& m) : msg(m) {}
    virtual ~BaseException() noexcept {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Them ban ghi co ma da ton tai
class DuplicateIdException : public BaseException {
public:
    explicit DuplicateIdException(const std::string& id)
        : BaseException("Ma da ton tai: " + id) {}
};

// Tim / sua / xoa ma khong ton tai
class NotFoundException : public BaseException {
public:
    explicit NotFoundException(const std::string& id)
        : BaseException("Khong tim thay ma: " + id) {}
};

// Du lieu nhap sai (rong, am, ngay khong hop le, sai dinh dang dong file...)
class InvalidDataException : public BaseException {
public:
    explicit InvalidDataException(const std::string& detail)
        : BaseException("Du lieu khong hop le: " + detail) {}
};

// Khong mo duoc / khong ghi duoc file .txt
class FileException : public BaseException {
public:
    explicit FileException(const std::string& path)
        : BaseException("Loi doc/ghi file: " + path) {}
};

// Khoa ngoai khong ton tai. Vi du: Bill co SoSim chua co trong sims.txt
// Dung: throw ForeignKeyException("SoSim", "0987654321");
class ForeignKeyException : public BaseException {
public:
    ForeignKeyException(const std::string& field, const std::string& value)
        : BaseException("Khoa ngoai khong ton tai: " + field + " = " + value) {}
};

// Khong cho xoa vi ban ghi dang duoc thuc the khac tham chieu
// Dung: throw ReferencedException("SIM", "0987654321", "Bill");
class ReferencedException : public BaseException {
public:
    ReferencedException(const std::string& entity, const std::string& id,
                        const std::string& usedBy)
        : BaseException("Khong the xoa " + entity + " " + id +
                        " vi dang duoc " + usedBy + " tham chieu") {}
};

#endif
