#pragma once
#include <iostream>
#include <exception>

class NOTARESISTOR : virtual public std::exception
{

protected:
    std::string error_message; ///< Error message

public:
    explicit NOTARESISTOR(const std::string &msg) : error_message(msg)
    {
    }

    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }
};
