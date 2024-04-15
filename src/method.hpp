#pragma once
#include <tao/json.hpp>
#include <functional>
#include <unordered_map>

    using
json_t = tao::json::value;
    using 
schema_t = json_t;

    namespace
calculisto::remote_services
{
/** A function that processes a request.
 * Its input is JSON data that must conform to the method_t::expects schema, 
 * it returns JSON data that conforms to the mthod_t::returns schema.
 * It also has a method_t::title (a short description) and a mode detailled
 * method_t::description. 
 */
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

/** A set of named methods.
 */
    using
methods_t = std::unordered_map <std::string, method_t>;
} // namespace calculisto::remote_services
