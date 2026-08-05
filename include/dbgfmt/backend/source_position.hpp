
/// \file source_position.hpp
/// Contains definition of the source_position class.

#pragma once

#include <filesystem>


namespace dbgfmt::backend {


/// Represents position in source code for formatting framework
class source_position {
public:
    /// Constructs source position with specified path to file and
    /// line number
    source_position(const std::filesystem::path & f = {}, unsigned int lnum = 0):
        file_{f}, line_{lnum} {}

    /// Returns true if source position is valid
    bool is_valid() const { return !file_.empty() ? true : false; }

    /// Returns source position file path
    auto & file() const { return file_; }

    /// Sets source position file path
    void set_file(const std::filesystem::path & p) { file_ = p; }

    /// Returns source position line number
    auto line() const { return line_; }

    /// Sets source position line number
    void set_line(unsigned int l) { line_ = l; }

    /// Returns true if source position is valid
    operator bool() const { return is_valid(); }

    /// Returns false if source position is not valid
    bool operator!() const { return (*this) ? false : true; }

private:
    std::filesystem::path file_;      ///< File path
    unsigned int line_;                 ///< Line number in file
};


}


