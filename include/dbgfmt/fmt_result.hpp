
/// \file formatted_value.hpp
/// Contains definition of formatted_value class.

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "ref_counted.hpp"
#include "string.hpp"
#include "source_position.hpp"
#include "format_error.hpp"


namespace dbgfmt {


class fmt_result;
class fmt_result_ref;
using fmt_result_up = std::unique_ptr<fmt_result>;
class named_fmt_result;

class context;


/// Interface for bridging formatting result with external code
class fmt_result_bridge: virtual public ref_counted {
public:
    /// Returns value of the value field
    virtual ref_counted_string * br_get_val() const = 0;

    /// Returns value of the type field
    virtual ref_counted_string * br_get_type() const = 0;

    /// Returns source position of definition of object that is contained in address
    /// specied by value
    virtual void br_get_val_pos(ref_counted_string ** file, unsigned int * line) const = 0;

    /// Returns source position of definition of value type
    virtual void br_get_type_pos(ref_counted_string ** file, unsigned int * line) const = 0;

    /// Returns format time in milliseconds
    virtual unsigned long br_get_format_time() const = 0;

    /// Sets format time in milliseconds
    virtual void br_set_format_time(unsigned long msec) = 0;

    /// Returns memory address of value
    virtual uint64_t br_address() const = 0;

    /// Sets memory address of value
    virtual void br_set_address(uint64_t addr) = 0;

    /// Returns number of child nodes
    virtual size_t br_get_childs_size() const = 0;

    /// Returns named of child node at formatting result
    /// with specified index
    virtual void br_get_child_at(size_t idx,
                                 ref_counted_string ** name,
                                 ref_counted_string ** pos_file,
                                 unsigned int * pos_line,
                                 fmt_result_bridge ** res) const = 0;
};


/// Implementation of result of formatting. Should be used as base class
/// for all results of formatting.
class fmt_result: virtual public fmt_result_bridge,
                  public ref_counted_impl {
public:
    /// Constructor
    fmt_result() = default;

    /// Destructor, destroys object
    virtual ~fmt_result() = default;

    /// Returns value of the value field
    virtual std::wstring val() const = 0;

    /// Returns value of type field
    virtual std::wstring type() const = 0;

    /// Returns source position of definition of object that is contained in address
    /// specied by value
    virtual source_position val_pos() const = 0;

    /// Returns source position of definition of value type
    virtual source_position type_pos() const = 0;

    /// Returns address of formatted value
    uint64_t address() const { return addr_; }

    /// Sets address of formatted value
    void set_address(uint64_t addr) { addr_ = addr; }

    /// Returns number of child nodes
    virtual size_t childs_size() const = 0;

    /// Returns child value at specified index
    virtual named_fmt_result child_at(size_t idx) const = 0;

    /// Returns format time in milliseconds
    unsigned long format_time() const  { return fmt_time_; }

    /// Sets format time in milliseconds
    void set_format_time(unsigned long t) { fmt_time_ = t; }


    // bridge interface

    /// Returns value of the value field
    ref_counted_string * br_get_val() const override {
        auto v = make_string(val());
        return v.do_return();
    }

    /// Returns value of the type field
    ref_counted_string * br_get_type() const override {
        auto tp = make_string(type());
        return tp.do_return();
    }

    /// Returns source position of definition of object that is contained in address
    /// specied by value
    void br_get_val_pos(ref_counted_string ** file, unsigned int * line) const override {
        auto p = val_pos();
        if (!p.is_valid()) {
            *file = nullptr;
            *line = 0;
            return;
        }

        *file = make_string(p.file().wstring()).do_return();
        *line = p.line();
    }

    /// Returns source position of definition of value type
    void br_get_type_pos(ref_counted_string ** file, unsigned int * line) const override {
        auto p = type_pos();
        if (!p.is_valid()) {
            *file = nullptr;
            *line = 0;
            return;
        }

        *file = make_string(p.file().wstring()).do_return();
        *line = p.line();
    }

    /// Returns format time in milliseconds
    unsigned long br_get_format_time() const override {
        return format_time();
    }

    /// Sets format time in milliseconds
    void br_set_format_time(unsigned long msec) override {
        set_format_time(msec);
    }

    /// Returns address of formatted value
    uint64_t br_address() const override {
        return address();
    }

    /// Sets address of formatted value
    void br_set_address(uint64_t addr) override {
        set_address(addr);
    }

    /// Returns number of child nodes
    size_t br_get_childs_size() const override {
        return childs_size();
    }

    /// Returns child node at specified index
    void br_get_child_at(size_t idx,
                         ref_counted_string ** name,
                         ref_counted_string ** pos_file,
                         unsigned int * pos_line,
                         fmt_result_bridge ** res) const override;

private:
    unsigned long fmt_time_ = 0;            ///< Format time
    uint64_t addr_ = UINT64_MAX;            ///< Value address
};


using fmt_result_ptr = boost::intrusive_ptr<fmt_result_bridge>;
class fmt_result_ref;
fmt_result_ref acquire_fmt_result(fmt_result_bridge *);


/// Represents wrapper around intrusive pointer to fmt_result bridge interface
class fmt_result_ref: public ref_counted_ref_base<fmt_result_bridge> {
    friend fmt_result_ref acquire_fmt_result(fmt_result_bridge *);

public:
    /// Default constructor. Constructs invalid reference
    fmt_result_ref() = default;

    /// Returns value of the value field
    std::wstring val() const {
        string_ref v = acquire_string(get()->br_get_val());
        return v.read();
    }

    /// Returns value of type field
    std::wstring type() const {
        string_ref tp = acquire_string(get()->br_get_type());
        return tp.read();
    }

    /// Returns source position of definition of object that is contained in address
    /// specied by value
    source_position val_pos() const {
        ref_counted_string * br_file = nullptr;
        unsigned int line = 0;
        get()->br_get_val_pos(&br_file, &line);
        auto file = acquire_string(br_file);

        if (!file) {
            return {};
        }

        return {std::filesystem::path{file.read()}, line};
    }

    /// Returns source position of definition of value type
    source_position type_pos() const {
        ref_counted_string * br_file = nullptr;
        unsigned int line = 0;
        get()->br_get_type_pos(&br_file, &line);
        auto file = acquire_string(br_file);

        if (!file) {
            return {};
        }

        return {std::filesystem::path{file.read()}, line};
    }

    /// Returns format time in milliseconds
    unsigned long format_time() const {
        return get()->br_get_format_time();
    }

    /// Sets format time in milliseconds
    void set_format_time(unsigned long msec) const {
        get()->br_set_format_time(msec);
    }

    /// Returns address of formatted value
    uint64_t address() const {
        return get()->br_address();
    }

    /// Sets address of formatted value
    void set_address(uint64_t addr) const {
        get()->br_set_address(addr);
    }

    /// Returns number of child nodes
    size_t childs_size() const { return get()->br_get_childs_size(); }

    /// Returns child value at specified index. Calls br_get_child_at
    /// and converts format_error exception to formatted values
    inline named_fmt_result child_at(size_t idx) const;

    /// Returns pointer to this object. For compatibility with old code
    auto operator->() const { return this; }

private:
    /// Acquires pointer to formatting result. Does not increase reference counter.
    explicit fmt_result_ref(fmt_result_bridge * res):
        ref_counted_ref_base<fmt_result_bridge>{res} {}
};


/// Pair of reference to formatting result and it's name
class named_fmt_result {
public:
    named_fmt_result() = default;
    named_fmt_result(std::wstring nm,
                     const source_position & p,
                     fmt_result_ref r):
        name_{nm}, pos_{p}, res_{r} {}

    auto name() const { return name_; }
    auto res() const { return res_; }
    auto val() const { return res_.val(); }
    auto type() const { return res_.type(); }
    auto childs_size() const { return res_.childs_size(); }
    auto child_at(size_t idx) const { return res_.child_at(idx); }

    auto & pos() const { return pos_; }
    auto val_pos() const { return res_.val_pos(); }
    auto type_pos() const { return res_.type_pos(); }

    auto format_time() const { return res_.format_time(); }
    auto address() const { return res_.address(); }

    /// Returns pointer to this. For compatibility with old code
    auto operator->() const { return this; }

    /// Returns true if result is valid
    operator bool() const { return res_ ? true : false; }

    /// Returns true if result is not valid
    bool operator!() const { return res_ ? false : true; }

private:
    std::wstring name_;
    source_position pos_;
    fmt_result_ref res_;
};


inline named_fmt_result fmt_result_ref::child_at(size_t idx) const {
    // executing br_get_child_at
    ref_counted_string * br_name = nullptr;
    ref_counted_string * br_pos_file = nullptr;
    unsigned int br_pos_line = 0;
    fmt_result_bridge * br_res = nullptr;
    get()->br_get_child_at(idx, &br_name, &br_pos_file, &br_pos_line, &br_res);
    assert(br_name != nullptr && "Returned null name");
    assert(br_res != nullptr && "Returned null res");

    // acquiring pointers to name, pos file name, and result
    auto nm_ref = acquire_string(br_name);
    auto pos_file_ref = acquire_string(br_pos_file);
    auto res_ref = acquire_fmt_result(br_res);

    // converting to named_fmt_result

    source_position pos;
    if (pos_file_ref) {
        pos.set_file(pos_file_ref.read());
        pos.set_line(br_pos_line);
    }

    return named_fmt_result{nm_ref.read(), pos, res_ref};
}



/// Acquires ownership of a fmt result. Does not change reference
/// counter. It should be greather than zero.
inline fmt_result_ref acquire_fmt_result(fmt_result_bridge * res) {
    return fmt_result_ref{res};
}


/// Creates fmt result ref from unique pointer to fmt result
template <typename T>
fmt_result_ref make_ref(std::unique_ptr<T> && res) {
    res->add_ref();
    return acquire_fmt_result(res.release());
}


/// Creates formatting result of specified type and assigns it to
/// formatting result reference
template <typename T, typename ... Args>
fmt_result_ref make_fmt_result(Args && ...args) {
    auto res = std::make_unique<T>(std::forward<Args>(args)...);
    return make_ref(std::move(res));
}


/// Creates formatting result of specified type, assigns it to
/// fmt result reference, and wraps it into named_fmt_result
template <typename T, typename ... Args>
named_fmt_result make_named_fmt_result(std::wstring nm,
                                       const source_position & pos,
                                       Args && ... args) {
    return named_fmt_result{nm, pos, make_fmt_result<T>(std::forward<Args>(args)...)};
}


/// Creates error fmt result with specified error message
fmt_result_ref make_error_fmt_result(const std::wstring & msg);

/// Creates error fmt result from format_error instance
fmt_result_ref make_error_fmt_result(const format_error & err);


}


