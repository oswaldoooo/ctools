#pragma once
#include <exception>
#include <string>
class NoElement : public std::exception {
private:
    std::string core;

public:
    NoElement() = delete;
    NoElement(std::string& input)
        : core(input)
    {
    }
    NoElement(const char* input)
        : core(input)
    {
    }
    const char* what() const noexcept { return core.c_str(); }
};
namespace ctools {
class error {
public:
    virtual bool isempty() = 0;
    virtual std::string what() = 0;
    virtual void clear() = 0;
};
class logic_error : public error {
private:
    std::string core;
    friend logic_error& operator<<(logic_error&, const char*);

public:
    void write(char* data) noexcept
    {
        core += data;
    }
    void join(logic_error err)
    {
        core += err.core;
    }
    bool isempty() override
    {
        return core.size() == 0;
    }
    std::string what() override
    {
        return core;
    }
    void clear() override
    {
        core.clear();
    }
};
logic_error& operator<<(logic_error& origin, const char* data)
{
    origin.core += data;
    return origin;
}

}