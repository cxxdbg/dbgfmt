
/// \file record_value.hpp
/// Contains definition of the record_value class.

#pragma once

#include "value.hpp"
#include <cm/find_field.hpp>
#include <cm/record_type.hpp>
#include <cm/record_type_debug_info.hpp>
#include <algorithm>
#include <ranges>
#include <cstdint>
#include <string>


namespace dbgfmt::test {


/// Builder of record value
class record_value: public value<cm::record_type> {
    using qual_type = cm::qual_type_t<cm::record_type>;

public:
    /// Constructs record value builder
    record_value(test_context & ctx, const qual_type & type):
        value<cm::record_type>(ctx, type) {}

    /// Constructs record value builder
    record_value(const qual_type & type):
        record_value(test_context::current_context(), type) {}

    /// Constructs builder for record value with specified type and data
    record_value(test_context & ctx, const qual_type & type, const data::accessor & d):
        value<cm::record_type>(ctx, type, d) {}

    /// Constructs builder for record value with specified type and data
    record_value(const qual_type & type, const data::accessor & d):
        record_value(test_context::current_context(), type, d) {}

    /// Constructs record value from value
    record_value(const value<> & val):
        record_value(val.type().cast<cm::record_type>(), val.data()) {}

    /// Returns builder for value of base class with specified type
    record_value base(const cm::record_type * btype) const {
        auto bases = type()->bases();
        auto base_it = std::ranges::find(bases, btype);
        assert(base_it != std::ranges::end(bases) && "Can't find base class");
        auto base_idx = std::ranges::distance(std::ranges::begin(bases), base_it);

        auto dbg_info = ctx().dbg().type(type());
        auto rec_dbg_info = dynamic_cast<const cm::record_type_debug_info*>(dbg_info);
        assert(rec_dbg_info && "record type does not have debug info");

        assert(base_idx < rec_dbg_info->bases_offsets().size() && "debug info inconsistency");

        auto base_offs = static_cast<int64_t>(rec_dbg_info->bases_offsets()[base_idx]);
        auto base_rec = (*base_it)->untypedef()->cast<cm::record_type>();
        assert(base_rec && "base is not a record");

        return record_value{ctx(), base_rec, data::accessor{data(), base_offs / 8}};
    }

    /// Returns builder for value of base class with specified type name
    record_value base(const std::string & name) const {
        auto dbg_info = ctx().dbg().type(type());
        auto rec_dbg_info = dynamic_cast<const cm::record_type_debug_info*>(dbg_info);
        assert(rec_dbg_info && "record type does not have debug info");

        const auto & bases = type()->bases();
        auto base_it = std::ranges::find_if(bases, [&name](auto && b) {
            auto named_base = b->template cast<cm::named_record_type>();
            if (!named_base) {
                return false;
            }

            return named_base->name() == name;
        });

        assert(base_it != std::end(bases) && "can't find base with specified name");
        auto base_idx = std::distance(std::begin(bases), base_it);

        assert(base_idx < rec_dbg_info->bases_offsets().size() &&
               "debug info inconsistency");

        auto base_offs = static_cast<int64_t>(rec_dbg_info->bases_offsets()[base_idx]);
        auto base_rec = (*base_it)->untypedef()->cast<cm::record_type>();
        assert(base_rec && "base is not a record");
        return record_value{ctx(), base_rec, data::accessor{data(), base_offs / 8}};
    }

    /// Returns builder for value of field with specified name
    value<> field(const std::string & name) const {
        auto fld_res = cm::find_field(&ctx().dbg(), type().type(), name, true);
        auto fld = std::get<0>(fld_res);
        auto fld_offs = std::get<1>(fld_res);

        assert(fld && "can't find field");
        assert(fld->bit_size() == 0 && "bitfields are not supported");
        auto fld_offs_i = static_cast<int64_t>(fld_offs / 8);
        return value<>{ctx(), fld->type(), data::accessor{data(), fld_offs_i}};
    }

    /// Returns builder for value of field with specified name
    value<> operator[](const std::string & name) const {
        return field(name);
    }
};


template <>
struct cm_type_traits<cm::record_type> {
    using value_type = record_value;
};


template <typename T>
value<> value<T>::operator[](const std::string & fld_name) const {
    return as<record_value>()[fld_name];
}


}


