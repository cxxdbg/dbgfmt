
/// \file forward.hpp
/// Contains forward definitions for fmt library classes.

#pragma once

#include <memory>


namespace dbgfmt {


class formatter;
class value;


}


namespace dbgfmt::backend {


class value_impl;


typedef std::shared_ptr<value_impl> value_impl_sp;
typedef std::shared_ptr<const value_impl> const_value_impl_sp;


}


