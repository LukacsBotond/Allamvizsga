#pragma once
#include <iostream>
#include <exception>

class NOTSUPPOSEDTOREACHTHIS : virtual public std::exception
{

protected:
    int error_number;          ///< Error number
    int error_offset;          ///< Error offset
    std::string error_message; ///< Error message

public:
    NOTSUPPOSEDTOREACHTHIS(const std::string &msg) : error_message(msg)
    {
    }
    /*
    explicit Except(const std::string &msg, int err_num, int err_off) : error_number(err_num),
                                                                        error_offset(err_off),
                                                                        error_message(msg)
    {
    }
    */

    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }
    /*
    virtual int getErrorNumber() const throw()
    {
        return error_number;
    }

    virtual int getErrorOffset() const throw()
    {
        return error_offset;
    }
    */
};
