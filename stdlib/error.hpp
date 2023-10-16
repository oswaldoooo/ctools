#pragma once
#include <exception>
#include <string>
#include <cstring>
#include <cstddef>
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
private:
public:
    virtual const char* errorf() const = 0;
    virtual ~error() { }
};
template <typename T>
struct Ans {
    error* err;
    T val;
};
class logic_error : public error {
private:
    char* core = NULL;

public:
    logic_error() = delete;
    logic_error(const char* src)
    {
        size_t length = strlen(src);
        core = new char[length + 1];
        core[length] = 0;
        memmove(core, src, length);
    }
    logic_error(const std::string& src)
    {
        size_t length = src.length();
        core = new char[length + 1];
        core[length] = 0;
        memmove(core, src.c_str(), length);
    }
    logic_error(logic_error&) = delete;
    logic_error(logic_error&& src)
    {
        if (src.core != NULL) {
            if (this->core != NULL) delete[] this->core;
            this->core = src.core;
            src.core = NULL;
        }
    }
    ~logic_error()
    {
        if (core != NULL) delete[] core;
    }
    const char* errorf() const override
    {
        return core;
    }
};
class str_error : public error {
private:
    char* core = NULL;
    size_t maxsize = 0;

public:
    str_error() = delete;
    str_error(size_t size)
        : maxsize(size)
    {
        core = new char[size];
        memset(core, 0, size);
    }
    str_error(str_error&& src)
    {
        if (this->core != NULL) delete[] core;
        this->core = src.core;
        this->maxsize = src.maxsize;
        if (src.core != NULL) {
            delete[] src.core;
            src.core = NULL;
            src.maxsize = 0;
        }
    }
    str_error(error& err)
    {
        if (this->core != NULL) delete[] core;
        maxsize = (err.errorf() == NULL) ? 0 : strlen(err.errorf());
        if (maxsize > 0) {
            this->core = new char[maxsize + 1];
            this->core[maxsize] = 0;
            memmove(this->core, err.errorf(), maxsize);
        } else {
            this->core = NULL;
        }
    }
    bool join(const char* v)
    {
        size_t length = strlen(v);
        if (maxsize < strlen(core) + length) return false;
        strncat(core, v, strlen(v));
        return true;
    }
    ~str_error()
    {
        if (core != NULL) delete[] core;
    }
    const char* errorf() const override
    {
        return core;
    }
};
inline error* errors_join(error** errinput, size_t len)
{
    size_t length = 0;
    if (len < 1) return NULL;
    for (size_t i = 0; i < len; i++) length += (errinput[i] == NULL || errinput[i]->errorf() == NULL) ? 0 : strlen(errinput[i]->errorf());
    str_error* ans = new str_error(length);
    for (size_t i = 0; i < len; i++) {
        if (errinput[i] != NULL && errinput[i]->errorf() != NULL) {
            if (!ans->join(errinput[i]->errorf())) break;
        }
    }
    return ans;
}
}
