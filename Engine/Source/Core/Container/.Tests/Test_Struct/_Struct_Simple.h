#pragma once
#include <string>

namespace UnitTest {
struct _Test_Simple 
{
    int id;
    std::string name;

    _Test_Simple() 
        : id(0)
        , name("default") 
    {}

    _Test_Simple(int i, const std::string& n) 
        : id(i)
        , name(n) 
    {}

    bool operator==(const _Test_Simple& other) const 
    {
        return id == other.id && name == other.name;
    }

    bool operator<(const _Test_Simple& other) const 
    {
        return id < other.id;
    }
};
}