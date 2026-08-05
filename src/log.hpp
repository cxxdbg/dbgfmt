
/// \file log.hpp
/// Contains definition of logger for formatter framework

#pragma once

#include <boost/log/attributes/constant.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <algorithm>
#include <string>


namespace dbgfmt {


template <auto N>
struct log_attr_string {
    constexpr log_attr_string(const char (&s)[N]) {
        std::copy_n(s, N, value);
    }

    /// Returns string value containing attribute
    std::string string() const {
        return std::string{&value[0], sizeof(value) - 1};
    }

    char value[N];
};


using logger_t = boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>;


/// Returns reference to logger with category attribute
template <log_attr_string Category>
inline logger_t & get_logger() {
    auto init_logger = [] {
        logger_t logger;
        logger.add_attribute("Category", boost::log::attributes::make_constant(Category.string()));
        return logger;
    };
    static logger_t logger = init_logger();
    return logger;
}


#define DBGFMT_LOG(level) \
    BOOST_LOG_SEV((::dbgfmt::get_logger<"dbgfmt">()), level)

#define DBGFMT_LOG_TRACE       DBGFMT_LOG(::boost::log::trivial::trace)
#define DBGFMT_LOG_DEBUG       DBGFMT_LOG(::boost::log::trivial::debug)
#define DBGFMT_LOG_INFO        DBGFMT_LOG(::boost::log::trivial::info)
#define DBGFMT_LOG_WARNING     DBGFMT_LOG(::boost::log::trivial::warning)
#define DBGFMT_LOG_ERROR       DBGFMT_LOG(::boost::log::trivial::error)
#define DBGFMT_LOG_FATAL       DBGFMT_LOG(::boost::log::trivial::fatal)


}
