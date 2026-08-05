
/// \file value_impl_common.cpp
/// Contains implementation of the value_impl_common class.

#include "dbgfmt/value_impl_common.hpp"
#include "log.hpp"
#include "dbgfmt/simple_value_impl.hpp"
#include "dbgfmt/temp_value_impl.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include "data/error_storage.hpp"
#include "dbgfmt/backend/array_type_impl.hpp"
#include "dbgfmt/backend/builtin_type_impl.hpp"
#include "dbgfmt/backend/enum_type_impl.hpp"
#include "dbgfmt/backend/function_type_impl.hpp"
#include "dbgfmt/backend/mem_ptr_type_impl.hpp"
#include "dbgfmt/backend/pointer_type_impl.hpp"
#include "dbgfmt/backend/record_type_impl.hpp"
#include "dbgfmt/backend/typedef_type_impl.hpp"
#include "dbgfmt/backend/type_context_impl.hpp"
#include "dbgfmt/backend/value_context_impl.hpp"
#include "dbgfmt/backend/vector_type_impl.hpp"
#include <cxxtn/type_name_parser.hpp>


namespace dbgfmt {


const backend::type_impl * value_impl_common::type() const {
    return raw_type()->untypedef();
}


bool value_impl_common::is_bool() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    return bt->kind() == backend::builtin_type_impl::kind_t::bool_;
}


bool value_impl_common::is_array() const {
    return type()->cast<backend::array_type_impl>() != nullptr;
}


bool value_impl_common::is_vector() const {
    return type()->cast<backend::vector_type_impl>() != nullptr;
}


bool value_impl_common::is_signed() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    switch (bt->kind()) {
    case backend::builtin_type_impl::kind_t::char_:
    case backend::builtin_type_impl::kind_t::short_:
    case backend::builtin_type_impl::kind_t::int_:
    case backend::builtin_type_impl::kind_t::long_:
    case backend::builtin_type_impl::kind_t::long_long_:
        return true;
    default:
        return false;
    }
}


bool value_impl_common::is_unsigned() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    switch (bt->kind()) {
    case backend::builtin_type_impl::kind_t::unsigned_char_:
    case backend::builtin_type_impl::kind_t::unsigned_short_:
    case backend::builtin_type_impl::kind_t::unsigned_int_:
    case backend::builtin_type_impl::kind_t::unsigned_long_:
    case backend::builtin_type_impl::kind_t::unsigned_long_long_:
        return true;
    default:
        return false;
    }
}


bool value_impl_common::is_float() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    switch (bt->kind()) {
    case backend::builtin_type_impl::kind_t::float_:
    case backend::builtin_type_impl::kind_t::double_:
    case backend::builtin_type_impl::kind_t::long_double_:
        return true;
    default:
        return false;
    }
}


bool value_impl_common::is_pointer() const {
    return type()->cast<backend::pointer_type_impl>() != nullptr;
}


bool value_impl_common::is_record() const {
    return type()->cast<backend::record_type_impl>() != nullptr;
}


bool value_impl_common::is_wchar() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    return bt->kind() == backend::builtin_type_impl::kind_t::wchar_t_;
}


bool value_impl_common::is_enum() const {
    return type()->cast<backend::enum_type_impl>() != nullptr;
}


bool value_impl_common::is_float_complex() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    switch (bt->kind()) {
    case backend::builtin_type_impl::kind_t::complex_float_:
    case backend::builtin_type_impl::kind_t::complex_double_:
    case backend::builtin_type_impl::kind_t::complex_long_double_:
        return true;
    default:
        return false;
    }

}


bool value_impl_common::is_signed_complex() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    switch (bt->kind()) {
    case backend::builtin_type_impl::kind_t::complex_char_:
    case backend::builtin_type_impl::kind_t::complex_short_:
    case backend::builtin_type_impl::kind_t::complex_int_:
    case backend::builtin_type_impl::kind_t::complex_long_:
    case backend::builtin_type_impl::kind_t::complex_long_long_:
        return true;
    default:
        return false;
    }
}


bool value_impl_common::is_unsigned_complex() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    switch (bt->kind()) {
    case backend::builtin_type_impl::kind_t::complex_unsigned_char_:
    case backend::builtin_type_impl::kind_t::complex_unsigned_short_:
    case backend::builtin_type_impl::kind_t::complex_unsigned_int_:
    case backend::builtin_type_impl::kind_t::complex_unsigned_long_:
    case backend::builtin_type_impl::kind_t::complex_unsigned_long_long_:
        return true;
    default:
        return false;
    }
}


bool value_impl_common::is_mem_ptr() const {
    return type()->cast<backend::mem_ptr_type_impl>() ? true : false;
}


bool value_impl_common::is_nullptr_t() const {
    auto bt = type()->cast<backend::builtin_type_impl>();
    if (!bt) {
        return false;
    }

    return bt->kind() == backend::builtin_type_impl::kind_t::nullptr_t_;
}


std::string value_impl_common::type_name() const {
    return type()->name();
}


std::wstring value_impl_common::wtype_name() const {
    const auto & tname = type_name();
    return {tname.begin(), tname.end()};
}


std::string value_impl_common::qual_type_name() const {
    return type()->qual_name();
}


std::wstring value_impl_common::wqual_type_name() const {
    const auto & tname = qual_type_name();
    return {tname.begin(), tname.end()};
}


backend::value_impl_sp value_impl_common::cast(const backend::type_impl * type) const {
    return create_offs_value(type, 0);
}


backend::value_impl_sp value_impl_common::ptr() const {
    auto res = std::make_shared<temp_value_impl>(*ctx(), type()->pointer_type(), memory(), endianness());
    res->write_uint64(offset());
    return res;
}


void value_impl_common::assign(const backend::value_impl & val) const {
    if (size() != val.size()) {
        std::ostringstream msg;
        msg << "can't assign values of different sizes: size of source type '"
            << val.qual_type_name() << "' is " << val.size()
            << ", size of destination type '" << qual_type_name() << "' is " << size();
        throw format_error{msg.str()};
    }

    try {
        std::vector<char> bytes(size());
        val.data().read_raw(bytes.data(), size());
        data().write_raw(bytes.data(), size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


bool value_impl_common::as_bool() const {
    try {
        // checking that value type is boolean
        auto btype = type()->cast<backend::builtin_type_impl>();
        if (!btype || btype->kind() != backend::builtin_type_impl::kind_t::bool_) {
            throw format_error("value is not boolean");
        }

        // reading integer representing boolean value
        auto val = data().read_unsigned(btype->size());
        return val != 0;
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_bool(bool v) const {
    try {
        // checking that value type is boolean
        auto btype = type()->cast<backend::builtin_type_impl>();
        if (!btype || btype->kind() != backend::builtin_type_impl::kind_t::bool_) {
            throw format_error("value is not boolean");
        }

        // writing integer representing boolean value
        data().write_unsigned(v ? 1 : 0, btype->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


std::int64_t value_impl_common::as_int64() const {
    try {
        return data().read_signed(type()->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_int64(int64_t val) const {
    try {
        data().write_signed(val, type()->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


std::uint64_t value_impl_common::as_uint64() const {
    try {
        return data().read_unsigned(type()->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_uint64(uint64_t val) const {
    try {
        data().write_unsigned(val, type()->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


long double value_impl_common::as_long_double() const {
    try {
        return data().read_float(type()->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_long_double(long double val) const {
    try {
        return data().write_float(val, type()->size());
    }
    catch (std::exception & err) {
        throw format_error{err.what()};
    }
}


std::size_t value_impl_common::enum_items_size() const {
    auto etype = type()->cast<backend::enum_type_impl>();
    if (!etype) {
        throw format_error("value is not an enum");
    }

    return etype->items_size();
}


std::string value_impl_common::enum_item_name(std::size_t index) const {
    auto etype = type()->cast<backend::enum_type_impl>();
    if (!etype) {
        throw format_error("value is not an enum");
    }

    return etype->item_name(index);
}


std::int64_t value_impl_common::enum_item_value(std::size_t index) const {
    auto etype = type()->cast<backend::enum_type_impl>();
    if (!etype) {
        throw format_error("value is not an enum");
    }

    return etype->item_value(index);
}


bool value_impl_common::has_address() const {
    return &storage() == &memory();
}


std::uint64_t value_impl_common::address() const {
    if (!has_address()) {
        throw format_error{"can't get address of value that is not in memory"};
    }

    return data().offset();
}


std::size_t value_impl_common::array_size() const {
    auto atype = type()->cast<backend::array_type_impl>();
    if (!atype) {
        throw format_error("value is not an array");
    }

    return atype->array_size();
}


backend::value_impl_sp value_impl_common::array_at(std::size_t i) const {
    auto atype = type()->cast<backend::array_type_impl>();
    if (!atype) {
        throw format_error("value is not an array");
    }

    auto offset = static_cast<int64_t>(atype->base()->size() * i);
    return create_offs_value(atype->base(), offset);
}


std::size_t value_impl_common::vector_size() const {
    auto vtype = type()->cast<backend::vector_type_impl>();
    if (!vtype) {
        throw format_error("value is not a vector");
    }

    return vtype->vector_size();
}


backend::value_impl_sp value_impl_common::vector_at(std::size_t i) const {
    auto vtype = type()->cast<backend::vector_type_impl>();
    if (!vtype) {
        throw format_error("value is not a vector");
    }

    auto offset = static_cast<int64_t>(vtype->base()->size() * i);
    return create_offs_value(vtype->base(), offset);
}


bool value_impl_common::can_dereference() const {
    return type_as_pointer()->can_dereference();
}


backend::value_impl_sp value_impl_common::dereference(bool resolve_dyn_type) const {
    auto base_type = type_as_pointer()->base();
    auto addr = as_uint64();

    if (resolve_dyn_type) {
        // trying resolve dynamic type for pointer to record

        auto ut_base_type = base_type->untypedef();
        if (dynamic_cast<const backend::record_type_impl*>(ut_base_type)) {
            auto dyn_this_type = dynamic_type_at_addr(addr, type(), type()->size());
            auto dyn_this_ptr_type = dynamic_cast<const backend::pointer_type_impl*>(dyn_this_type);
            assert(dyn_this_ptr_type && "dynamic_type_at_addr returned not pointer");
            base_type = dyn_this_ptr_type->base();
        }
    }

    data::accessor res_data{memory(), data().endian(), addr};
    return std::make_shared<simple_value_impl>(*ctx(), base_type, res_data);
}


backend::value_impl_sp value_impl_common::add(std::int64_t val) const {
    auto this_type = type_as_pointer();
    auto base_type = this_type->base();
    auto size = base_type->size();

    if (size == 0) {
        std::ostringstream msg;
        msg << "Can't add/sub pointers to type '"
            << base_type->name() << "' that does not have size";
        throw format_error{msg.str()};
    }

    auto offs = static_cast<int64_t>(static_cast<uint64_t>(size)) * val;
    auto new_addr = as_uint64() + offs;
    auto new_val = std::make_shared<temp_value_impl>(*ctx(), this_type, memory(), endianness());
    new_val->data().write_unsigned(new_addr, this_type->size());
    return new_val;
}


std::int64_t value_impl_common::difference(const backend::value_impl * v) const {
    auto this_type = type_as_pointer();

    auto v_type = v->type()->cast<backend::pointer_type_impl>();
    if (!v_type) {
        std::ostringstream msg;
        msg << "type '" << v->type()->name() << "' is not a pointer";
        throw format_error{msg.str()};
    }

    if (this_type != v_type) {
        std::ostringstream msg;
        msg << "can't calc difference between pointers of differrent type '"
            << this_type->name() << "' and '" << v->type()->name() << "'";
        throw format_error{msg.str()};
    }

    auto base_type = this_type->base();
    if (base_type->size() == 0) {
        std::ostringstream msg;
        msg << "Can't calc difference between pointers to type '"
            << base_type->name() << "' that does not have size";
        throw format_error{msg.str()};
    }

    auto this_val = as_uint64();
    auto v_val = v->as_uint64();

    if (this_val >= v_val) {
        return static_cast<int64_t>((this_val - v_val) / base_type->size());
    } else {
        return -static_cast<int64_t>((v_val - this_val) / base_type->size());
    }
}


std::string value_impl_common::function_name() const {
    // detecting function name only for pointers
    if (!type()->cast<backend::pointer_type_impl>()) {
        return {};
    }

    auto obj = ctx()->obj_at_addr(as_uint64());
    if (!obj) {
        return {};
    }

    if (!obj->type()->cast<backend::function_type_impl>()) {
        return {};
    }

    return obj->name();
}


std::uint64_t value_impl_common::function_offset() const {
    // detecting function name only for pointers
    if (!type()->cast<backend::pointer_type_impl>()) {
        return {};
    }

    auto addr = as_uint64();
    auto obj = ctx()->obj_at_addr(addr);
    if (!obj) {
        return {};
    }

    if (!obj->type()->cast<backend::function_type_impl>()) {
        return {};
    }

    assert(obj->addr() <= addr && "invalid address of object");
    return addr - obj->addr();
}


std::string value_impl_common::symbol_name() const {
    auto sym = ctx()->sym_at_addr(as_uint64());
    if (!sym) {
        return {};
    }

    return sym->name();
}


std::uint64_t value_impl_common::symbol_offset() const {
    auto addr = as_uint64();
    auto sym = ctx()->sym_at_addr(addr);
    if (!sym) {
        return {};
    }

    assert(sym->addr() <= addr && "invalid symbol address");
    return addr - sym->addr();
}


source_position value_impl_common::pointee_def_pos() const {
    auto obj = ctx()->obj_at_addr(as_uint64());
    if (!obj) {
        return {};
    }

    return obj->def_pos();
}


std::string value_impl_common::template_name() const {
    return type_as_record()->template_name();
}


std::size_t value_impl_common::bases_size() const {
    return type_as_record()->bases_size();
}


backend::value_impl_sp value_impl_common::base_at(std::size_t i) const {
    auto type = type_as_record()->base_type_at(i);
    auto offs = static_cast<int64_t>(type_as_record()->base_offset_at(i, offset()) / 8);
    return create_offs_value(type, offs);
}


std::size_t value_impl_common::fields_size() const {
    return type_as_record()->fields_size();
}


std::string value_impl_common::field_name_at(std::size_t i) const {
    return type_as_record()->field_name_at(i);
}


std::wstring value_impl_common::wfield_name_at(std::size_t i) const {
    auto nm = field_name_at(i);
    return std::wstring{nm.begin(), nm.end()};
}


source_position value_impl_common::field_def_pos_at(size_t i) const {
    return type_as_record()->field_def_pos_at(i);
}


backend::value_impl_sp value_impl_common::field_at(std::size_t i) const {
    auto rtype = type_as_record();
    const backend::type_impl * fld_type = rtype->field_type_at(i);
    auto offs = rtype->field_offset_at(i);
    auto sz = rtype->field_size_at(i);
    return create_field_val(fld_type, offs, sz, {});
}


bool value_impl_common::has_field(const std::string & nm) const {
    return type_as_record()->find_field(nm, nullptr, nullptr, nullptr, false) != nullptr;
}


backend::value_impl_sp value_impl_common::field(const std::string & nm) const {
    uint64_t fld_offset = 0;
    uint64_t fld_size = 0;
    source_position fld_def_pos;
    auto fld_type = type_as_record()->find_field(nm,
                                                 &fld_offset,
                                                 &fld_size,
                                                 &fld_def_pos,
                                                 false);

    if (fld_type) {
        std::ostringstream msg;
        msg << "Can't find field with name '" << nm << "' in record type "
            << type()->name();
        throw format_error{msg.str()};
    }

    return create_field_val(fld_type, fld_offset, fld_size, fld_def_pos);
}


backend::value_impl_sp value_impl_common::mem_fun_ptr(const std::string & nm) const {
    // getting function type
    auto ft = type_as_record()->mem_fun_type(nm);
    if (ft == nullptr) {
        return {};
    }

    // getting function address
    auto addr = type_as_record()->mem_fun_addr(nm);
    if (addr == 0) {
        return {};
    }

    // constructing temporary value object containing address to function
    auto fptr_type = ft->pointer_type();
    auto ptr = std::make_shared<temp_value_impl>(*ctx(), fptr_type, memory(), endianness());
    ptr->data().write_unsigned(addr, fptr_type->size());
    return ptr;
}


std::vector<std::string> value_impl_common::mem_funcs() const {
    return type_as_record()->mem_funcs();
}


backend::value_impl_sp value_impl_common::try_all_field(const std::string & nm) const {
    uint64_t fld_offset = 0;
    uint64_t fld_size = 0;
    source_position fld_def_pos;
    auto fld_type = type_as_record()->find_field(nm,
                                                 &fld_offset,
                                                 &fld_size,
                                                 &fld_def_pos, true);
    if (!fld_type) {
        return {};
    }

    return create_field_val(fld_type, fld_offset, fld_size, fld_def_pos);
}


backend::value_impl_sp value_impl_common::all_field(const std::string & nm) const {
    auto res = try_all_field(nm);
    if (!res) {
        std::ostringstream msg;
        msg << "Can't find field with name '" << nm << "' in record type '"
            << type()->name() << "' and its base classes";
        throw format_error{msg.str()};
    }

    return res;
}


backend::value_impl_sp value_impl_common::all_field_pointer(const std::string & nm) const {
    uint64_t fld_offset = 0;
    uint64_t fld_size = 0;
    auto fld_type = type_as_record()->find_field(nm, &fld_offset, &fld_size, nullptr, true);
    if (!fld_type) {
        std::ostringstream msg;
        msg << "Can't find field '" << nm << "' in type '"
            << type()->name() << "'";
        throw format_error{msg.str()};
    }

    // checking for bitfield
    if (fld_offset % 8 != 0 || fld_size % 8 != 0) {
        std::ostringstream msg;
        msg << "Can't get pointer to bitfield '" << nm << "' in type '"
            << type()->name() << "'";
        throw format_error{msg.str()};
    }

    uint64_t fld_addr = address() + (fld_offset / 8);
    auto fld_ptr_type = fld_type->pointer_type();
    auto res = std::make_shared<temp_value_impl>(*ctx(), fld_ptr_type, memory(), endianness());
    res->data().write_unsigned(fld_addr, fld_ptr_type->size());
    return res;
}


long double value_impl_common::real_part_as_long_double() const {
    if (!is_float_complex()) {
        throw format_error{"value is not a float complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().read_float(sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


long double value_impl_common::img_part_as_long_double() const {
    if (!is_float_complex()) {
        throw format_error{"value is not a float complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().read_float(sz, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_real_part_long_double(long double v) const {
    if (!is_float_complex()) {
        throw format_error{"value is not a float complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().write_float(v, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_img_part_long_double(long double v) const {
    if (!is_float_complex()) {
        throw format_error{"value is not a float complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().write_float(v, sz, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


int64_t value_impl_common::real_part_as_int64() const {
    if (!is_signed_complex()) {
        throw format_error{"value is not a signed complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().read_signed(sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


int64_t value_impl_common::img_part_as_int64() const {
    if (!is_signed_complex()) {
        throw format_error{"value is not a signed complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().read_signed(sz, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_real_part_int64(int64_t v) const {
    if (!is_signed_complex()) {
        throw format_error{"value is not a signed complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().write_signed(v, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_img_part_int64(int64_t v) const {
    if (!is_signed_complex()) {
        throw format_error{"value is not a signed complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().write_signed(v, sz, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


uint64_t value_impl_common::real_part_as_uint64() const {
    if (!is_unsigned_complex()) {
        throw format_error{"value is not a unsigned complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().read_unsigned(sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


uint64_t value_impl_common::img_part_as_uint64() const {
    if (!is_unsigned_complex()) {
        throw format_error{"value is not a unsigned complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().read_unsigned(sz, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_real_part_uint64(uint64_t v) const {
    if (!is_unsigned_complex()) {
        throw format_error{"value is not a unsigned complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().write_unsigned(v, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


void value_impl_common::write_img_part_uint64(uint64_t v) const {
    if (!is_unsigned_complex()) {
        throw format_error{"value is not a unsigned complex"};
    }

    auto sz = type()->size() / 2;

    try {
        return data().write_unsigned(v, sz, sz);
    }
    catch(std::exception & err) {
        throw format_error{err.what()};
    }
}


std::string
value_impl_common::mem_var_ptr_field_name_and_def_pos(source_position & def_pos) const {
    auto mptr_type = type()->cast<backend::mem_ptr_type_impl>();
    if (!mptr_type) {
        std::ostringstream msg;
        msg << "type '" << type_name() << "' is not a pointer to member";
        throw format_error{msg.str()};
    }

    // reading field offset
    auto offs = as_uint64() * 8;

    // looking for field with read offset
    auto obj_type = mptr_type->obj_type();
    for (size_t i = 0, e = obj_type->fields_size(); i < e; ++i) {
        auto f_offs = obj_type->field_offset_at(i);
        if (f_offs == offs) {
            // found field at offset

            def_pos = obj_type->field_def_pos_at(i);

            std::ostringstream res;
            res << obj_type->name() << "::" << obj_type->field_name_at(i);
            return res.str();
        } else if (f_offs > offs) {
            break;
        }
    }

    // field with read offset not found
    return {};
}


const backend::record_type_impl * value_impl_common::type_as_record() const {
    auto rtype = type()->cast<backend::record_type_impl>();
    if (!rtype) {
        throw format_error{"type is not a record"};
    }

    return rtype;
}


const backend::pointer_type_impl * value_impl_common::type_as_pointer() const {
    auto ptype = type()->cast<backend::pointer_type_impl>();
    if (!ptype) {
        std::ostringstream msg;
        msg << "type '" << type()->name() << "' is not a pointer";
        throw format_error{msg.str()};
    }

    return ptype;
}


backend::value_impl_sp value_impl_common::create_field_val(const backend::type_impl * type,
                                                  uint64_t bit_offset,
                                                  uint64_t bit_size,
                                                  const source_position & def_pos) const {
    if (bit_offset % 8 == 0 && bit_size == type->size() * 8) {
        return create_offs_value(type, bit_offset / 8, def_pos);
    }

    // creating temporary value for bitfield

    auto bt_type = type->cast<backend::builtin_type_impl>();
    if (!bt_type) {
        throw dbgfmt::format_error{"bitfield has non builtin type"};
    }

    if (type->size() > 8) {
        throw dbgfmt::format_error{"bitfield size is greater than 8 bytes"};
    }

    try {
        auto storage = std::make_shared<data::vector_storage>(type->size());
        auto byte_offs = bit_offset / 8;
        auto bit_offs_in_byte = bit_offset % 8;

        switch (bt_type->kind()) {
        case backend::builtin_type_impl::kind_t::unsigned_char_:
        case backend::builtin_type_impl::kind_t::unsigned_short_:
        case backend::builtin_type_impl::kind_t::unsigned_int_:
        case backend::builtin_type_impl::kind_t::unsigned_long_:
        case backend::builtin_type_impl::kind_t::unsigned_long_long_: {
            // unsigned type
            auto res = data().read_unsigned_bits(bit_size, byte_offs, bit_offs_in_byte);
            storage->write(&res, type->size(), 0);
            break;
        }
        default: {
            // signed type
            auto res = data().read_signed_bits(bit_size, byte_offs, bit_offs_in_byte);
            storage->write(&res, type->size(), 0);
            break;
        }
        }

        return std::make_shared<temp_value_impl>(*ctx(), type, storage, 0, memory(), endianness());
    }
    catch(data::error & err) {
        // creating error storage for field
        auto storage = std::make_shared<data::error_storage>(err.what());
        return std::make_shared<temp_value_impl>(*ctx(), type, storage, 0, memory(), endianness());
    }
}


const backend::type_impl * value_impl_common::dynamic_type_at_addr(uint64_t & addr,
                                                          const backend::type_impl * static_type,
                                                          size_t ptr_size) const {
    try {
        DBGFMT_LOG_DEBUG << "trying resolve dynamic type at addr" << std::hex << addr
                            << " for static type '" << static_type->qual_name() << "'";

        // Performing manual dynamic type lookup only for types marked as dynamic.
        // This is required because we still can find pointer to vtable at the beginning
        // of a staic object value, and this leads to displaying incorrect type of value.
        // (CXXDBG-756)
        auto pointee_type = static_type->try_as_pointer()->base();
        assert(pointee_type && "static type must be a pointer");
        auto static_rec_type = dynamic_cast<const backend::record_type_impl*>(pointee_type->untypedef());
        if (!static_rec_type || !static_rec_type->is_dynamic()) {
            DBGFMT_LOG_DEBUG << "type '" << static_type->qual_name()
                                << "' is not a dynamic type, using value context for resolving dynamic type";
            return ctx()->dynamic_type_at_addr(addr, static_type);
        }

        data::accessor mem_acc{memory(), endianness()};

        // reading pointer that points to list of virtual functions somewhere in vtable
        auto vtable_funcs_ptr = mem_acc.read_unsigned(ptr_size, addr);

        // resolving symbol in which list of virtual functions are located
        if (auto vtable_sym = ctx()->sym_at_addr(vtable_funcs_ptr)) {
            DBGFMT_LOG_DEBUG << "vtable symbol name for addr " << std::hex << addr
                                << ": '" << vtable_sym->name() << "'";

            // checking that vtable symbol name starts with "vtable for " and extracting
            // type name from vtable name
            static const std::string vtable_prefix = "vtable for ";
            if (vtable_sym->name().substr(0, vtable_prefix.size()) == vtable_prefix) {

                const backend::type_impl * dtype = nullptr;

                // We found valid virtual table for type. Trying find type from debug info
                // by name

                auto type_name_str = vtable_sym->name().substr(vtable_prefix.size());

                // parsing type name
                std::istringstream str{type_name_str};
                std::string parse_err;
                auto type_name = cxxtn::parse_type_name(str, &parse_err);
                if (type_name.is_valid()) {
                    auto tctx = type()->ctx();

                    // first trying find type with name written in gcc canonical form
                    std::ostringstream gcc_can_type_name;
                    type_name.write(gcc_can_type_name, cxxtn::type_name_write_style::gcc);
                    DBGFMT_LOG_DEBUG << "trying find type with gcc canonical name: '"
                                        << gcc_can_type_name.str() << "'";
                    dtype = tctx->find_type(gcc_can_type_name.str());

                    // if failed to resolve with gcc type name then try use clang canonical form
                    if (!dtype) {
                        std::ostringstream clang_can_type_name;
                        type_name.write(clang_can_type_name, cxxtn::type_name_write_style::clang);
                        DBGFMT_LOG_DEBUG << "trying find type with clang canonical name: '"
                                            << clang_can_type_name.str() << "'";
                        dtype = tctx->find_type(clang_can_type_name.str());
                    }

                    if (!dtype) {
                        DBGFMT_LOG_ERROR << "can't find type with original name: '" << type_name_str << "'";
                    }
                } else {
                    DBGFMT_LOG_ERROR << "can't parse type name obtained from vtable: '" << type_name_str << "'";
                }


                if (!dtype) {
                    // We can't find object type from vtable symbol name. Trying to find it by searching
                    // for debug info for the first virtual function.
                    DBGFMT_LOG_DEBUG << "trying find dynamic type by resolving virtual function debug object";

                    // first reading address of the first virtual function.s
                    auto virt_func = mem_acc.read_unsigned(ptr_size, vtable_funcs_ptr);

                    // resolving symbol for virtual function
                    if (auto virt_func_obj = ctx()->obj_at_addr(virt_func)) {
                        DBGFMT_LOG_DEBUG << "found debug object for the first virtual function: "
                                            << virt_func_obj->name();

                        // trying get type of parent record for function object
                        if (auto virt_func_fobj = dynamic_cast<const backend::function_debug_object_impl*>(virt_func_obj)) {
                            dtype = virt_func_fobj->record_type();
                            if (!dtype) {
                                DBGFMT_LOG_ERROR << "can't find parent record type for debug function object for first virtual function";
                            }
                        } else {
                            DBGFMT_LOG_ERROR << "debug object for the first virtual function is not a function object";
                        }
                    } else {
                        DBGFMT_LOG_ERROR << "can't resolve symbol for the first virtual function in vtable";
                    }
                }


                if (dtype) {
                    // if type is found then detecting object start address

                    // if type is as as static type then object start address is same as original address
                    if (dtype == static_type) {
                        return dtype;
                    }

                    // offset to start of the object is located two pointer sizes above pointer
                    // to list of virtual functions in vtable. We need check for underflow when
                    // calculating address of offset
                    if (vtable_funcs_ptr >= ptr_size * 2) {
                        auto offset = mem_acc.read_signed(ptr_size, vtable_funcs_ptr - 2 * ptr_size);
                        addr += offset;

                        DBGFMT_LOG_DEBUG << "dynamic type object address: " << std::hex << addr;
                        return dtype->pointer_type();
                    }

                    // fallback to symbol context implementation
                    DBGFMT_LOG_ERROR << "vtable ptr addr underflow: " << vtable_funcs_ptr;
                }
            }
        }
    }
    catch (std::exception & err) {
        // ignoring memory read errors
    }

    // using value context implementation as backup path
    DBGFMT_LOG_DEBUG << "using value context to resolve dynamic type at addr " << std::hex << addr;
    return ctx()->dynamic_type_at_addr(addr, static_type);
}


}
