
/// \file template_name.hpp
/// Contains utility functions for making template instantiation names.

#pragma once

#include <sstream>
#include <string>
#include <vector>


namespace dbgfmt {


/// Makes template instantiation name for specified template
/// name and template parameters
template <typename C>
std::basic_string<C>
basic_make_template_inst_name(const std::basic_string<C> & templ_name,
                              const std::vector<std::basic_string<C>> & pars) {
    std::basic_ostringstream<C> res;
    res << templ_name << "<";

    bool first = true;
    for (auto && p : pars) {
        if (!first) {
            res << ", ";
        } else {
            first = false;
        }

        res << p;
    }

    // adding space if last parameter ends with '>'
    if (!pars.empty() && !pars.back().empty() && pars.back().back() == '>') {
        res << ' ';
    }

    res << ">";

    return res.str();
}


inline auto make_template_inst_name(const std::string & templ_name,
                                    const std::vector<std::string> & pars) {
    return basic_make_template_inst_name<char>(templ_name, pars);
}


inline auto wmake_template_inst_name(const std::wstring & templ_name,
                                     const std::vector<std::wstring> & pars) {
    return basic_make_template_inst_name<wchar_t>(templ_name, pars);
}


}
