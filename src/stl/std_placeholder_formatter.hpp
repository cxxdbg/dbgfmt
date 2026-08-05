
/// \file std_placeholder_formatter.hpp
/// Contains definition of the std_placeholder_formatter class.

#pragma once


#include "../simple_formatter.hpp"
#include "dbgfmt/format_error.hpp"
#include <sstream>


namespace dbgfmt {


/// Base class for formattes for STL bind placeholders. Contains common logic.
class std_placeholder_formatter: public simple_formatter {
private:
    /// Writes value string representation to output stream
    void write(const value & val, std::wostream & str) const override {
        std::istringstream numstr{val.type().template_param_name(0)};
        int num;
        numstr >> num;

        if (!numstr) {
            std::ostringstream msg;
            msg << "Can't parse number from template parameter '"
                << val.type().template_param_name(0) << "'";
            throw format_error{msg.str()};
        }

        str << "_" << num;
    }
};


}
