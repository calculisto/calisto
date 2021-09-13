#pragma once
#include <tao/json.hpp>
    using namespace tao::json;
#include <functional>
#include <unordered_map>

    using
json_t = tao::json::value;
    using 
schema_t = json_t;

    struct
method_t
{
        schema_t
    expects;
        schema_t
    returns;
        std::function <json_t(json_t const&)>
    function;
        std::string
    description = "";
        std::string
    title = "";
};
    using
method_table_t = std::unordered_map <std::string, method_t>;

/*
    using 
schema_table_t = std::unordered_map <std::string, json_t>;
*/
