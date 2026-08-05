
/// \file fmt_result.cpp
/// Contains implementaion of formatting result classes.

#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include <sstream>


namespace dbgfmt {


/// Returns child node at specified index
void fmt_result::br_get_child_at(size_t idx,
                                 ref_counted_string ** name,
                                 ref_counted_string ** pos_file,
                                 unsigned int * pos_line,
                                 fmt_result_bridge ** res) const {
    try {
        auto ch = child_at(idx);
        *name = make_string(ch.name()).do_return();

        if (ch.pos()) {
            *pos_file = make_string(ch.pos().file().wstring()).do_return();
            *pos_line = ch.pos().line();
        } else {
            *pos_file = nullptr;
            *pos_line = 0;
        }

        auto res_ref = ch.res();
        *res = res_ref.do_return();
    }
    catch(format_error & err) {
        *name = make_string().do_return();
        *res = make_error_fmt_result(err).do_return();
    }
}


fmt_result_ref make_error_fmt_result(const std::wstring & msg) {
    std::wostringstream str;
    str << L"<error: " << msg << L">";
    return make_fmt_result<fixed_fmt_result>(str.str(),
                                             L"<error>",
                                             source_position{},
                                             source_position{});
}


fmt_result_ref make_error_fmt_result(const format_error & err) {
    std::string serr{err.what()};
    std::wstring swerr(serr.begin(), serr.end());
    return make_error_fmt_result(swerr);
}


}
