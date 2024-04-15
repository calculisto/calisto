#pragma once
#include <calculisto/multikey_multimap/multikey_multimap.hpp>
    using calculisto::multikey_multimap::multikey_multimap_t;
#include <calculisto/uncertain_value/uncertain_value.hpp>
    using calculisto::uncertain_value::uncertain_value_t;
#include <string>
#include <vector>

    using
uv_t = uncertain_value_t <double>;

    struct
function_entry_t
{
        void*
    function;
        std::size_t
    arity;
        auto 
    operator<=>(function_entry_t const& e) const
    {
        return function <=> e.function ;
    }
        friend bool
    operator == (function_entry_t const& a, function_entry_t const& b)
    {
        return a.function == b.function;
    }
};

    using
map_t = multikey_multimap_t <
      function_entry_t
    , std::string               // substance
    , std::string               // property
    , std::vector <std::string> // conditions
    , std::string               // model
>;

