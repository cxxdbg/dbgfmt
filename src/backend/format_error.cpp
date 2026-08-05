
/// \file format_error.cpp
/// Conains implementation of format_error class.

#include "dbgfmt/backend/format_error.hpp"


namespace dbgfmt::backend {


format_error::format_error(const char * msg):
msg_(msg != nullptr ? msg : std::string()) {
}


format_error::format_error(const std::string & msg):
msg_(msg) {
}


format_error::~format_error() {
}


const char * format_error::what() const noexcept {
    return msg_.c_str();
}


}
