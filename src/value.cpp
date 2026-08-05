
/// \file value.cpp
/// Contains implementation of value class.

#include "dbgfmt/value.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include "dbgfmt/backend/array_type_impl.hpp"
//#include "backend/array_value_impl.hpp"
//#include "backend/bool_value_impl.hpp"
#include "dbgfmt/backend/builtin_type_impl.hpp"
#include "dbgfmt/backend/enum_type_impl.hpp"
//#include "backend/enum_value_impl.hpp"
//#include "backend/error_value_impl.hpp"
//#include "backend/float_complex_value_impl.hpp"
//#include "backend/float_value_impl.hpp"
#include "dbgfmt/backend/forward.hpp"
//#include "backend/int_complex_value_impl.hpp"
//#include "backend/integer_value_impl.hpp"
#include "dbgfmt/backend/pointer_type_impl.hpp"
//#include "backend/pointer_value_impl.hpp"
#include "dbgfmt/backend/record_type_impl.hpp"
//#include "backend/record_value_impl.hpp"
#include "dbgfmt/backend/typedef_type_impl.hpp"
//#include "backend/typedef_value_impl.hpp"
#include "dbgfmt/backend/value_impl.hpp"
//#include "backend/wchar_value_impl.hpp"
#include <cassert>


namespace dbgfmt {


value::value(const backend::const_value_impl_sp & imp):
impl_(imp) {
}


value::~value() {
}


bool value::is_valid() const {
    return impl_ ? true : false;
}


value_type value::type() const {
    assert(is_valid() && "invalid value");
    return impl_->type();
}


bool value::is_bool() const {
    return impl_->is_bool();
}


bool value::is_array() const {
    return impl_->is_array();
}


bool value::is_vector() const {
    return impl_->is_vector();
}


bool value::is_signed() const {
    return impl_->is_signed();
}


bool value::is_unsigned() const {
    return impl_->is_unsigned();
}


bool value::is_float() const {
    return impl_->is_float();
}


bool value::is_pointer() const {
    return impl_->is_pointer();
}


bool value::is_record() const {
    return impl_->is_record();
}


bool value::is_wchar() const {
    return impl_->is_wchar();
}


bool value::is_enum() const {
    return impl_->is_enum();
}


bool value::is_error() const {
    return false;
}


bool value::is_float_complex() const {
    return impl_->is_float_complex();
}


bool value::is_signed_complex() const {
    return impl_->is_signed_complex();
}


bool value::is_unsigned_complex() const {
    return impl_->is_unsigned_complex();
}


bool value::is_mem_ptr() const {
    return impl_->is_mem_ptr();
}


bool value::is_nullptr_t() const {
    return impl_->is_nullptr_t();
}


std::string value::type_name() const {
    return impl_->type_name();
}


std::wstring value::wtype_name() const {
    return impl_->wtype_name();
}


std::string value::qual_type_name() const {
    return impl_->qual_type_name();
}

std::wstring value::wqual_type_name() const {
    return impl_->wqual_type_name();
}


std::size_t value::size() const {
    return impl_->size();
}


value value::cast(const value_type & type) const {
    return value{impl_->cast(type.impl())};
}


value value::ptr() const {
    return value{impl_->ptr()};
}


void value::assign(const value & val) const {
    impl_->assign(*val.impl_);
}


const value & value::operator<<(const value & val) const {
    assign(val);
    return *this;
}


bool value::as_bool() const {
    return impl_->as_bool();
}


void value::write_bool(bool v) const {
    impl_->write_bool(v);
}


const value & value::operator<<(bool v) const {
    write_bool(v);
    return *this;
}


std::int64_t value::as_int64() const {
    return impl_->as_int64();
}


void value::write_int64(int64_t v) const {
    impl_->write_int64(v);
}


const value & value::operator<<(char v) const {
    impl_->write_int64(v);
    return *this;
}


const value & value::operator<<(short v) const {
    impl_->write_int64(v);
    return *this;
}


const value & value::operator<<(int v) const {
    impl_->write_int64(v);
    return *this;
}


const value & value::operator<<(long v) const {
    impl_->write_int64(v);
    return *this;
}


const value & value::operator<<(long long v) const {
    impl_->write_int64(v);
    return *this;
}


std::uint64_t value::as_uint64() const {
    return impl_->as_uint64();
}


void value::write_uint64(uint64_t v) const {
    impl_->write_uint64(v);
}


const value & value::operator<<(unsigned char v) const {
    write_uint64(v);
    return *this;
}


const value & value::operator<<(unsigned short v) const {
    write_uint64(v);
    return *this;
}


const value & value::operator<<(unsigned int v) const {
    write_uint64(v);
    return *this;
}


const value & value::operator<<(unsigned long v) const {
    write_uint64(v);
    return *this;
}


const value & value::operator<<(unsigned long long v) const {
    write_uint64(v);
    return *this;
}


long double value::as_long_double() const {
    return impl_->as_long_double();
}


void value::write_long_double(long double v) const {
    impl_->write_long_double(v);
}


const value & value::operator<<(float v) const {
    write_long_double(v);
    return *this;
}


const value & value::operator<<(double v) const {
    write_long_double(v);
    return *this;
}


const value & value::operator<<(long double v) const {
    write_long_double(v);
    return *this;
}


std::size_t value::enum_items_size() const {
    return impl_->enum_items_size();
}


std::string value::enum_item_name(std::size_t index) const {
    return impl_->enum_item_name(index);
}


std::int64_t value::enum_item_value(std::size_t index) const {
    return impl_->enum_item_value(index);
}


bool value::has_address() const {
    return impl_->has_address();
}


std::uint64_t value::address() const {
    return impl_->address();
}


std::size_t value::array_size() const {
    return impl_->array_size();
}


value value::array_at(std::size_t i) const {
    return value{impl_->array_at(i)};
}


std::size_t value::vector_size() const {
    return impl_->vector_size();
}


value value::vector_at(std::size_t i) const {
    return value{impl_->vector_at(i)};
}


const value value::operator[](std::size_t i) const {
    if (is_array()) {
        return array_at(i);
    } else if (is_vector()) {
        return vector_at(i);
    } else {
        throw format_error("value is not an array or vector");
    }
}


bool value::can_dereference() const {
    return impl_->can_dereference();
}


value value::dereference(bool resolve_dyn_type) const {
    return {impl_->dereference(resolve_dyn_type)};
}


const value value::operator*() const {
    return dereference(true);
}


value value::add(std::int64_t val) const {
    return {impl_->add(val)};
}


const value value::operator+(std::int64_t val) const {
    return add(val);
}


const value value::operator-(std::int64_t val) const {
    return add(-val);
}


std::int64_t value::difference(const value & v) const {
    return impl_->difference(v.impl_.get());
}


std::int64_t value::operator-(const value & v) const {
    return difference(v);
}


std::string value::function_name() const {
    return impl_->function_name();
}


std::wstring value::wfunction_name() const {
    auto fname = function_name();
    return std::wstring(fname.begin(), fname.end());
}


std::uint64_t value::function_offset() const {
    return impl_->function_offset();
}


std::string value::symbol_name() const {
    return impl_->symbol_name();
}


std::wstring value::wsymbol_name() const {
    auto nm = symbol_name();
    return std::wstring(nm.begin(), nm.end());
}


std::uint64_t value::symbol_offset() const {
    return impl_->symbol_offset();
}


source_position value::pointee_def_pos() const {
    return impl_->pointee_def_pos();
}


std::string value::template_name() const {
    return impl_->template_name();
}


std::size_t value::bases_size() const {
    return impl_->bases_size();
}


value value::base_at(std::size_t i) const {
    return value{impl_->base_at(i)};
}


std::size_t value::fields_size() const {
    return impl_->fields_size();
}


std::string value::field_name_at(std::size_t i) const {
    return impl_->field_name_at(i);
}

std::wstring value::wfield_name_at(std::size_t i) const {
    auto nm = field_name_at(i);
    return std::wstring{nm.begin(), nm.end()};
}


source_position value::field_def_pos_at(size_t i) const {
    return impl_->field_def_pos_at(i);
}


value value::field_at(std::size_t i) const {
    return value{impl_->field_at(i)};
}


value value::field_pointer_at(std::size_t i) const {
    assert(false && "NYI");
//    return value(impl_as_record().field_pointer_at(i));
    return {};
}


bool value::has_field(const std::string & nm) const {
    return impl_->has_field(nm);
}


value value::field(const std::string & nm) const {
    return value{impl_->field(nm)};
}


value value::mem_fun_ptr(const std::string & nm) const {
    return value{impl_->mem_fun_ptr(nm)};
}


std::vector<std::string> value::mem_funcs() const {
    return impl_->mem_funcs();
}


value value::try_all_field(const std::string & nm) const {
    return {impl_->try_all_field(nm)};
}


value value::all_field(const std::string & nm) const {
    return {impl_->all_field(nm)};
}


value value::all_field_pointer(const std::string &nm) const {
    return {impl_->all_field_pointer(nm)};
}


const value value::operator[](const std::string & nm) const {
    return all_field(nm);
}


std::wstring value::err_msg() const {
    assert(false && "NYI");
    return {};
//    std::shared_ptr<const error_value_impl> eval =
//            std::dynamic_pointer_cast<const error_value_impl>(impl_);
//    if (!eval) {
//        throw format_error("value is not an error");
//    }

//    return eval->err_msg();
}


long double value::real_part_as_long_double() const {
    return impl_->real_part_as_long_double();
}


long double value::img_part_as_long_double() const {
    return impl_->img_part_as_long_double();
}


void value::write_real_part_long_double(long double v) const {
    impl_->write_real_part_long_double(v);
}


void value::write_img_part_long_double(long double v) const {
    impl_->write_img_part_long_double(v);
}


int64_t value::real_part_as_int64() const {
    return impl_->real_part_as_int64();
}


int64_t value::img_part_as_int64() const {
    return impl_->img_part_as_int64();
}


void value::write_real_part_int64(int64_t v) const {
    impl_->write_real_part_int64(v);
}


void value::write_img_part_int64(int64_t v) const {
    impl_->write_img_part_int64(v);
}


uint64_t value::real_part_as_uint64() const {
    return impl_->real_part_as_uint64();
}


uint64_t value::img_part_as_uint64() const {
    return impl_->img_part_as_uint64();
}


void value::write_real_part_uint64(uint64_t v) const {
    impl_->write_real_part_uint64(v);
}


void value::write_img_part_uint64(uint64_t v) const {
    impl_->write_img_part_uint64(v);
}


std::string value::mem_var_ptr_field_name_and_def_pos(source_position & def_pos) const {
    return impl_->mem_var_ptr_field_name_and_def_pos(def_pos);
}


}
