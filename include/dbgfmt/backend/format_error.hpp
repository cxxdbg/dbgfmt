
/// \file format_error.hpp
/// Contains definition of format_error class.

#pragma once

#include <exception>
#include <string>


namespace dbgfmt::backend {


/// \class format_error
/// Exception class for handling formatting errors
class format_error: public std::exception {
public:
    /// Constructor, makes format error from null terminated
    /// string which can be null for empty message
    format_error(const char * msg);

    /// Constructor, makes format error with specified description
    format_error(const std::string & msg);

    /// Destructor, destroys object
    virtual ~format_error();

    /// Returns error message
    virtual const char * what() const noexcept;

private:
    std::string msg_;           ///< Error message
};


}


