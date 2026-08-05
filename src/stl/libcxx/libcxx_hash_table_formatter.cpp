
/// \file libcxx_hash_table_formatter.cpp
/// Contains implementation of formatters libc++ hash table containers and iterators.

#include "libcxx_hash_table_formatter.hpp"
#include "libcxx_ns_utils.hpp"
#include "dbgfmt/format_error.hpp"
#include "dbgfmt/context.hpp"
#include "../../template_name.hpp"
#include <cxxtn/tn.hpp>


namespace dbgfmt {


bool libcxx_hash_table_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "unordered_set") ||
           is_libcxx_record(v, "unordered_multiset") ||
           is_libcxx_record(v, "unordered_map") ||
           is_libcxx_record(v, "unordered_multimap");
}


size_t libcxx_hash_table_formatter::size(const value & val) const {
    return static_cast<std::size_t>(val["__table_"]["__p2_"]["__value_"].as_uint64());
}


value libcxx_hash_table_formatter::element(const value & val, std::size_t index) const {
    value htable = val["__table_"];

    const std::string & tname = htable.type().template_param_name(0);
    auto hash_node_t_name = make_libcxx_type_name("__hash_node", htable);
    const std::string & node_tname = make_template_inst_name(hash_node_t_name, {tname, "void*"});
    auto node_ptr_type = ctx().find_type(node_tname).pointer();

    auto node = htable["__p1_"]["__value_"]["__next_"];

    for (std::size_t i = 0; i < index; ++i) {
        node = (*node.cast(node_ptr_type))["__next_"];
    }

    auto res = (*node.cast(node_ptr_type))["__value_"];

    if (is_libcxx_record(val, "unordered_map") || is_libcxx_record(val, "unordered_multimap")) {
        res = res["__cc"];
    }

    return res;
}


std::wstring libcxx_hash_table_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}



bool libcxx_hash_table_iterator_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "__hash_iterator") ||
           is_libcxx_record(v, "__hash_const_iterator");
}


value libcxx_hash_table_iterator_formatter::get_value_pointer(const value & it) const {
    std::istringstream node_ptr_type_name_str{it.type().template_param_name(0)};
    auto node_ptr_type_name = cxxtn::parse_type_name(node_ptr_type_name_str);
    if (auto ptr_tn = dynamic_cast<const cxxtn::pointer_type_name*>(node_ptr_type_name.type())) {
        auto node_type_name = ptr_tn->base();
        std::ostringstream node_type_name_str;
        node_type_name.write(node_type_name_str, cxxtn::type_name_write_style::clang);
        auto node_type = ctx().find_type(node_type_name_str.str());
        auto res = ((*it["__node_"]).cast(node_type))["__value_"];

        // if value is record and contains __cc field then it is __hash_value_type and we should
        // take __cc field
        if (res.is_record() && res.has_field("__cc")) {
            res = res["__cc"];
        }

        return res.ptr();
    } else {
        std::ostringstream msg;
        msg << "libcxx hash map iterator template parameter is not a pointer: '"
            << it.type().template_param_name(0) << "'";
        throw format_error{msg.str()};
    }
}



bool libcxx_hash_map_iterator_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "__hash_map_iterator");
}


value libcxx_hash_map_iterator_formatter::convert(const value & it) const {
    return it["__i_"];
}



}
