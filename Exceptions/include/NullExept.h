#pragma once
#include <iostream>
#include <exception>

class NULLEXCEPT : virtual public std::exception
{

protected:
    int error_number;          ///< Error number
    int error_offset;          ///< Error offset
    std::string error_message; ///< Error message

public:
    NULLEXCEPT(const std::string &msg) : error_message(msg)
    {
    }
    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }
};
