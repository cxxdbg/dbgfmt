
/// \file error.hpp
/// Contains definition of the error class.

#pragma once

#include <stdexcept>
#include <string>


namespace dbgfmt::data {


/// Data reading/writing exception
class error: public std::runtime_error {
public:
    /// Constructs error with specified message
    error(const std::string & msg): std::runtime_error{msg} {}
};


}
