#pragma once

#include <exception>
#include <vector>
#include <string>
#include <cstdio>
#include <cstddef>

class TimeException : public std::exception
{
public:
    TimeException(const std::string &msg) : _msg(msg) {}
    ~TimeException() {};

    const char *what() const noexcept override
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};