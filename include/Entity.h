#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
protected:
    std::string id;
public:
    Entity() : id("") {}
    Entity(const std::string& id) : id(id) {}
    virtual ~Entity() {}

    std::string getId() const { return id; }
    void setId(const std::string& newId) { id = newId; }

    virtual std::string toFileLine() const = 0;
    virtual bool fromFileLine(const std::string& line) = 0;
    virtual void displayRow() const = 0;
    virtual void inputNew(const std::string& newId) = 0;
    virtual void updateFields() = 0;
};

#endif