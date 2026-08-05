
/// \file error_storage.hpp
/// Contains definition of the error_storage class.

#pragma once

#include "dbgfmt/data/error.hpp"
#include "dbgfmt/data/storage.hpp"


namespace dbgfmt::data {


/// Storage implementation that always throws error with specified message
class error_storage: public storage {
public:
    /// Constructs error storage with specified error message
    explicit error_storage(std::string msg):
        err_msg_{std::move(msg)} {}

    /// Always throws error
    void read(void * data, size_t sz, uint64_t offset) override {
        throw error{err_msg_};
    }

    /// Always throws error
    void write(const void * data, size_t sz, uint64_t offset) override {
        throw error{err_msg_};
    }

private:
    std::string err_msg_;       ///< Error message;
};


}
