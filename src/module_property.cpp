#include <source_location>
#include <string>
#include <vector>
#include <set>
#include <fmt/ranges.h>
#include "logger.hpp"
#include "method.hpp"
#include <calculisto/multikey_multimap/multikey_multimap.hpp>
    using calculisto::multikey_multimap::multikey_multimap_t;
#include <calculisto/uncertain_value/uncertain_value.hpp>
    using calculisto::uncertain_value::uncertain_value_t;
#include <calculisto/units/unit_parser.hpp>
    using namespace calculisto::units::dimension;
    using calculisto::units::quantity_t;
    using calculisto::units::any_quantity_t;
#include <calculisto/uncertain_value/uncertain_value_fmt.hpp>
#include <calculisto/units/dimension_fmt.hpp>
    using fmt::format;
#include <calculisto/array/array.hpp>
    using calculisto::array::array_t;

    using namespace calculisto;

#include "module_property_disambiguate.hpp"

#include <tao/json/contrib/traits.hpp>
    using namespace tao::json;

#include "module_property_map_headers.inc"

    namespace
calculisto::remote_services::module::property
{
inline static logger_t
LOG = make_logger ("module/property");

// Error codes
    constexpr auto
error_no_unit_for_this_property = 1;
    constexpr auto
error_unknown_substance         = 2;
    constexpr auto
error_unkown_property           = 3;
    constexpr auto
error_unkown_model              = 4;
    constexpr auto
error_unkown_conditions         = 5;
    constexpr auto
error_too_many_recursive_calls  = 6;
    constexpr auto
error_function_not_found        = 7;
    constexpr auto
error_missing_argument          = 8;
    constexpr auto
error_recursive_call_failed     = 9;
    constexpr auto
error_can_t_parse_unit          = 10;
    constexpr auto
error_bad_unit                  = 11;
    constexpr auto
error_too_many_arguments        = 12;

    using
uv_t = uncertain_value_t <double>;

    struct
function_entry_t
{
        void*
    function;
        dimension_t 
    dimension;
        std::size_t
    arity;
        std::string
    dimension_string;
        auto 
    operator <=> (function_entry_t const& e) const
    {
        return function <=> e.function ;
    }
        friend bool
    operator == (function_entry_t const& a, function_entry_t const& b)
    {
        return a.function == b.function;
    }
        auto
    apply0 () const
    {
        return reinterpret_cast <uv_t (*) ()> (function) ();
    }
        auto
    apply1 (uv_t const& a) const
    {
        return reinterpret_cast <uv_t (*) (uv_t const&)> (function) (a);
    }
        auto
    apply2 (uv_t const& a, uv_t const& b) const
    {
        return reinterpret_cast <uv_t (*) (uv_t const&, uv_t const&)> (function) (a, b);
    }
        auto
    apply3 (uv_t const& a, uv_t const& b, uv_t const& c) const
    {
        return reinterpret_cast <uv_t (*) (uv_t const&, uv_t const&, uv_t const&)> (function) (a, b, c);
    }
        auto
    apply4 (uv_t const& a, uv_t const& b, uv_t const& c, uv_t const& d) const
    {
        return reinterpret_cast <uv_t (*) (uv_t const&, uv_t const&, uv_t const&, uv_t const&)> (function) (a, b, c, d);
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

    using
signature_t = map_t::key_type;

#include "module_property_map_data.inc"
    
    json_t
signature_to_json (signature_t const& s)
{
        auto
    r = json_t {
          { "substance",  std::get <0> (s) }
        , { "property",   std::get <1> (s) }
        , { "model",      std::get <3> (s) }
    };
        auto&
    c = r["conditions"] = tao::json::empty_array;
    std::ranges::copy (std::get <2> (s), std::back_inserter (c.get_array ()));
    return r;
}

    class
property_t
{
private:
        std::set <std::string>
    substances_m;
        std::set <std::string>
    properties_m;
        std::set <std::string>
    conditions_m;
        std::set <std::string>
    models_m;
        json_t
    substances_json_m;
        json_t
    properties_json_m;
        json_t
    conditions_json_m;
        json_t
    models_json_m;
        json_t
    prefixes_json_m;
        json_t
    units_json_m;
        json_t
    functions_json_m;
        json_t
    property_units_m;
public:
        explicit
    property_t ()
    {
        LOG->info ("Initializing...");
            auto
        bail_out = false;
        for (auto const& [k, v]: functions_map)
        {
            substances_m.insert (std::get <0> (k));
            properties_m.insert (std::get <1> (k));
            // Check that the conditions are sorted
            if (!std::ranges::is_sorted (std::get <2> (k)))
            {
                LOG->error (
                      "In function for {} of {} according to {}, conditions are not sorted: {}"
                    , std::get <1> (k)
                    , std::get <0> (k)
                    , std::get <3> (k)
                    , fmt::join (std::get <2> (k), ", ")
                );
                bail_out = true;
            }
            for (const auto& c: std::get <2> (k))
            {
                conditions_m.insert (c);
            }
            models_m.insert (std::get <3> (k));
        }
        if (bail_out) throw std::runtime_error { "Initialization of the property module failed." };
        LOG->info ("There are {} substances:", substances_m.size ());
        for (auto const& s: substances_m)
        {
            LOG->info ("  {}", s);
        }
        LOG->info ("There are {} properties:", properties_m.size ());
        for (auto const& p: properties_m)
        {
            LOG->info ("  {}", p);
        }
        LOG->info ("There are {} conditions:", conditions_m.size ());
        for (auto const& c: conditions_m)
        {
            LOG->info ("  {}", c);
        }
        LOG->info ("There are {} models:",     models_m.size ());
        for (auto const& m: models_m)
        {
            LOG->info ("  {}", m);
        }
        substances_json_m = json_t {{ "result", substances_m }};
        properties_json_m = json_t {{ "result", properties_m }};
        conditions_json_m = json_t {{ "result", conditions_m }};
        models_json_m     = json_t {{ "result", models_m }};

            json_t
        tmp = empty_array;
        for (auto&& [symbol, name]: units::prefix::symbols_to_names)
        {
            tmp.push_back (symbol);
        }
        LOG->info ("There are {} prefixes:", tmp.get_array ().size ());
        for (auto const& t: tmp.get_array ())
        {
            LOG->info ("  {}", t.get_string ());
        }
        prefixes_json_m   = json_t {{ "result", std::move (tmp) }};
        tmp = empty_array;
        for (auto&& [symbol, name]: units::unit::symbols_to_names)
        {
            tmp.push_back (symbol);
        }
        LOG->info ("There are {} units:", tmp.get_array ().size ());
        for (auto const& t: tmp.get_array ())
        {
            LOG->info ("  {}", t.get_string ());
        }
        units_json_m   = json_t {{ "result", std::move (tmp) }};
        tmp = empty_array;
        for (auto const& [k, v]: functions_map)
        {
            tmp.push_back (signature_to_json (k));
        }
        LOG->info ("There are {} functions:", tmp.get_array ().size ());
        for (auto const& t: tmp.get_array ())
        {
            LOG->info (
                  "  \"{}\" of \"{}\" according to \"{}\" at given {}"
                , t.at ("property").get_string ()
                , t.at ("substance").get_string ()
                , t.at ("model").get_string ()
                , [&t]
                  {
                        auto
                    r = std::vector <std::string> {};
                    for (auto const& c: t.at ("conditions").get_array ())
                    {
                        r.push_back (c.get_string ());
                    }
                    return r;
                  }()
            );
        }
        functions_json_m = {{ "result", std::move (tmp) }};
        property_units_m = empty_object;
        LOG->info ("Available units for dimensions are:");
        for (auto const& [d, v]: units::unit::dimension_names_to_unit_names)
        {
            property_units_m[d] = empty_array;
            LOG->info ("  {}", d);
            for (auto const& u: v)
            {
                LOG->info ("     {}", u);
                property_units_m[d].push_back (
                    units::unit::names_to_symbols.at (std::string {u})
                );
            }
            if (property_units_m[d].get_array ().size () == 0)
            {
                property_units_m[d].push_back (
                    base_unit_symbol (units::dimension::names_to_values.at (d))
                );
            }
        }
        LOG->info ("Multimap content:");
        for (auto&& [k, v]: functions_map)
        {
            LOG->info (
                  "  {} {} {} {} ({}) {}"
                , reinterpret_cast <void*> (v.function)
                , v.dimension
                , std::get <0> (k)
                , std::get <1> (k)
                , fmt::join (std::get <2> (k), ", ")
                , std::get <3> (k)
            );
        }
        LOG->info ("Initialization done.");
    }

        auto const&
    substances () const
    {
        return substances_json_m;
    }
        auto const&
    properties () const
    {
        return properties_json_m;
    }
        auto const&
    conditions () const
    {
        return conditions_json_m;
    }
        auto const&
    models () const
    {
        return models_json_m;
    }
        auto const&
    prefixes () const
    {
        return prefixes_json_m;
    }
        auto const&
    units () const
    {
        return units_json_m;
    }
        auto
    property_units  (json_t const& params) const
    {
            auto
        i = property_units_m.find (params.get_string ());
        if (i)
        {

            return json_t {{ "result", *i }};
        }
        return json_t {{"error", { 
              { "code", error_no_unit_for_this_property }
            , { "message", "No units for this property" }
        }}};
    }
        auto const&
    functions () const
    {
        return functions_json_m;
    }
        auto
    filter_functions (json_t const& params) const
    {
            auto
        s = std::optional <std::string> {};
        if (auto is = params.find ("substance"); is && *is != "")
        {
            s = is->get_string ();
            LOG->info ("  specified substance: {}", *s);
            if (!functions_map.contains <0> (*s))
            {
                return json_t {{"error", { 
                      { "code", error_unknown_substance }
                    , { "message", fmt::format ("unkown substance: {}", *s) }
                }}};
            }
        }
        else
        {
            LOG->info ("  unspecified substance");
        }
            auto
        p = std::optional <std::string> {};
        if (auto ip = params.find ("property"); ip && *ip != "")
        {
            p = ip->get_string ();
            LOG->info ("  specified property: {}", *p);
            if (! functions_map.contains <1> (*p))
            {
                return json_t {{"error", { 
                      { "code", error_unkown_property }
                    , { "message", fmt::format ("unkown property: {}", *p) }
                }}};
            }
        }
        else
        {
            LOG->info ("  unspecified property");
        }
            auto
        m = std::optional <std::string> {};
        if (auto im = params.find ("model"); im && *im != "")
        {
            m = im->get_string ();
            LOG->info ("  specified model: {}", *m);
            if (! functions_map.contains <3> (*m))
            {
                return json_t {{"error", { 
                      { "code", error_unkown_model }
                    , { "message", fmt::format ("unkown model: {}", *m) }
                }}};
            }
        }
        else
        {
            LOG->info ("  unspecified model");
        }
            auto
        c = std::optional <std::vector <std::string>> {};
        if (auto ic = params.find ("conditions"); ic) 
        {
            c = std::vector <std::string> {};
            for (auto const& cx: ic->get_array ())
            {
                c->push_back (cx.get_string ());
            }
            LOG->info ("  specified conditions: {}", fmt::join (*c, ", "));
            if (! functions_map.contains <2> (*c))
            {
                return json_t {{"error", { 
                      { "code", error_unkown_conditions }
                    , { "message", fmt::format (
                          "unkown conditions: {}"
                        , fmt::join (*c, ", ")) 
                      }
                }}};
            }
        }
        else
        {
            LOG->info ("  unspecified conditions");
        }

            auto
        r = std::vector <map_t::const_iterator> {};
        functions_map.select (std::back_inserter (r), s, p, c, m);
        LOG->info ("  got {} matches", r.size ());
            json_t
        j = tao::json::empty_array;
        for (auto&& e: r)
        {
            LOG->info (
                  "    {} {} ({}) {}"
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , fmt::join (std::get <2> (e->first), ", ")
                , std::get <3> (e->first)
            );
            j.push_back (signature_to_json (e->first));
        }
        return json_t {{"result", j }};
    }
private:
    /* Iterate over an array of values or returns a single value at each
     * iteration.
     */
        class
    iterable_t
    {
    public:
        iterable_t (json_t const& json, double magnitude)
            : magnitude_m { magnitude }
        {
            if (json.is_array ())
            {
                is_array_m = true;
                array_m = &(json.get_array ());
                return;
            }
            is_array_m = false;
            if (json.is_number ())
            {
                value_m = json.as <double> ();
                return;
            }
            LOG->info ("Default (0.0)");
            value_m = 0.0;
        }
            iterable_t&
        operator ++ ()
        {
            ++index_m;
            return *this;
        }
            double
        operator * () const
        {
            if (is_array_m)
            {
                switch ((*array_m)[index_m].type ())
                {
                    case type::SIGNED:
                        return (*array_m)[index_m].get_signed () * magnitude_m;
                    case type::UNSIGNED:
                        return (*array_m)[index_m].get_unsigned () * magnitude_m;
                    case type::DOUBLE:
                        return (*array_m)[index_m].get_double () * magnitude_m;
                    default:
                        throw std::runtime_error { format ("Internal error {}", std::source_location::current ().line ())};
                };
            }
            return value_m * magnitude_m;
        }
            auto
        size () const
            -> std::size_t
        {
            if (is_array_m)
            {
                return array_m->size ();
            }
            return 1;
        }
    private:
            std::size_t
        index_m = 0;
            bool
        is_array_m;
            std::vector <json_t> const*
        array_m;
            double
        value_m;
            double
        magnitude_m;
    };
    /* Takes an array of array and iterate over the inner arrays.
     */
        class
    magic_t
    {
            class
        iterator_t
        {
        public:
            iterator_t (magic_t* owner, std::size_t index)
                : owner_m { owner }
                , index_m { index }
            {}
                auto&
            operator ++ ()
            {
                for (auto& v: owner_m->v_m)
                {
                    ++v;
                }
                for (auto& u: owner_m->u_m)
                {
                    ++u;
                }
                ++index_m;
                return *this;
            }
                auto
            operator * () const
                -> std::vector <uv_t>
            {
                    auto
                r = std::vector <uv_t> {};
                for (auto i = 0u; i < owner_m->u_m.size (); ++i)
                {
                    r.push_back (uv_t { 
                          *(owner_m->v_m[i])
                        , *(owner_m->u_m[i]) 
                    });
                }
                return r;
            }
                friend bool
            operator != (iterator_t const& a, iterator_t const& b)
            {
                return a.index_m != b.index_m;
            }
        private:
                magic_t*
            owner_m;
                std::size_t
            index_m;
        };
            auto const&
        get_uncertainty (json_t const& j)
        {
                static const json_t
            def = 0.0;
            if (auto u = j.find ("uncertainty"); u)
            {
                return *u;
            }
                return def;
        }
    public:
        magic_t (std::vector <std::pair <json_t, double>> const& arguments)
        {
            size_m = 0;
            for (const auto& a: arguments)
            {
                v_m.emplace_back (a.first.at ("value"), a.second);
                    const auto
                s = v_m.back ().size ();
                LOG->info ("  size: {}", s);
                u_m.emplace_back (get_uncertainty (a.first), a.second);
                if (u_m.back ().size () != 1 && u_m.back ().size () != s)
                {
                    throw std::runtime_error { format ("Internal error {}", std::source_location::current ().line ())};
                }
                if (size_m == 0) 
                {
                    size_m = s;
                    continue;
                }
                if (s != size_m && s == 1) continue;
                if (s > size_m && size_m == 1) 
                {
                    size_m = s;
                    continue;
                }
                if (s == size_m) continue;
                throw std::runtime_error { format ("Internal error {}", std::source_location::current ().line ())};
            }
            LOG->info ("There are {} values", size_m);
        }
            auto
        begin ()
        {
            return iterator_t { this, 0 };
        }
            auto
        end ()
        {
            return iterator_t { this, size_m };
        }
    private:
            std::vector <iterable_t>
          v_m
        , u_m
        ;
            std::size_t
        size_m = 0;
    };
public:
        auto
    execute_function (json_t const& params, int depth = 0) const
    {
        if (depth > 1)
        {
            return json_t {{"error", { 
                  { "code", error_too_many_recursive_calls }
                , { "message", "Too many recursive calls" }
            }}};
        }
            auto const&
        s = params.at ("signature");
        LOG->info ("  substance:  {}", s.at ("substance").get_string ());
        LOG->info ("  property:   {}", s.at ("property").get_string ());
        LOG->info ("  conditions: {}", to_string (s.at ("conditions")));
        LOG->info ("  model:      {}", s.at ("model").get_string ());

            const auto&
        substance = s.at ("substance").get_string ();
            const auto&
        property = s.at ("property").get_string ();
            const auto 
        conditions = [&s]
        {
                auto
            r = std::vector <std::string> {};
            for (auto const& i: s.at ("conditions").get_array ())
            {
                r.push_back (i.get_string ());
            }
            std::ranges::sort (r);
            return r;
        }();
            const auto&
        model = s.at ("model").get_string ();

        // Find the function
        // =====================================================================
            const auto 
        function_entry_it = functions_map.find (signature_t { 
              substance
            , property
            , conditions
            , model
        });
        if (function_entry_it == std::end (functions_map))
        {
            return json_t {{"error", { 
                  { "code", error_function_not_found }
                , { "message", "Function not found" }
            }}};
        }
            auto const&
        function_entry = function_entry_it->second;
        LOG->info (
              "       {}"
            , units::dimension::values_to_names.at (function_entry.dimension)
        );
        LOG->info (
              "       {}"
            , units::unit::dimension_names_to_unit_names.at (
                units::dimension::values_to_names.at (
                    function_entry.dimension
                )
              ).size ()
        );

        // Return unit
        // =====================================================================
            auto const
        r_u = units::unit_symbol (function_entry.dimension_string);

        // Arguments
        // =====================================================================
            const auto&
        argument_obj = params.at ("arguments");
            auto
        arguments = std::vector <std::pair <json_t, double>> {};
        for (auto i = 0u; i < function_entry.arity; ++i)
        {
                const auto&
            condition = conditions[i];
            LOG->info ("Looking for {}", condition);
                auto 
            v = argument_obj.find (condition);
            if (v == nullptr)
            {
                return json_t {{"error", { 
                      { "code", error_missing_argument }
                    , { 
                          "message"
                        , fmt::format ("Missing argument: {}.", condition) 
                      }
                }}};
            }
            // At this point, we either have a value-uncertainty-unit combo 
            // or another function call that will provide those.
                auto
            function_result = json_t {};
            if (v->find ("signature"))
            {
                function_result = execute_function (*v, depth + 1);
                v = function_result.find ("result");
                if (v == nullptr)
                {
                    return json_t {{"error", { 
                          { "code", error_recursive_call_failed }
                        , { "message", "Recursive call failed" }
                        , { "data", *v }
                    }}};
                }
            }
            // Back to normal operations
                const auto&
            u = v->at ("unit").get_string ();;
                const auto
            uu = units::unit_parse_opt (u);
            if (!uu)
            {
                return json_t {{"error", { 
                      { "code", error_can_t_parse_unit }
                    , { "message", "Can't parse unit" }
                    , { "data", u }
                }}};
            }
            if (uu->dimension != units::dimension::names_to_values.at (condition))
            {
                return json_t {{"error", { 
                      { "code", error_bad_unit }
                    , { "message", "First unit (" + u + ") is not a " +
                            condition + " unit" }
                    , { "data", u }
                }}};
            }
            /*
            LOG->info ("  found a {}", to_string (v->type ()));
            for (auto&& [k, v]: v->get_object ())
            {
                LOG->info ("    {}, {}", k, to_string (v));
            }
            */
            arguments.push_back ({ std::move (*v), uu->magnitude });
        }
        /*
        for (auto i = 0; auto&& e: arguments)
        {
            LOG->info ("argument {}: {}", i, to_string (e.type ()));
            for (auto&& [k, v]: e.get_object ())
            {
                LOG->info ("  {}: {}", k, to_string (v.type ()));
            }
            ++i;
        }
        */
        // Execution
        // =====================================================================
            json_t
        ret = tao::json::empty_array;
        for (const auto& args: magic_t (arguments))
        {
            try
            {
                    auto
                r = uv_t {};
                switch (function_entry.arity)
                {
                    case 0:
                        r = function_entry.apply0 ();
                        break;
                    case 1:
                        r = function_entry.apply1 (args[0]);
                        break;
                    case 2:
                        r = function_entry.apply2 (args[0], args[1]);
                        break;
                    case 3:
                        r = function_entry.apply3 (args[0], args[1], args[2]);
                        break;
                    case 4:
                        r = function_entry.apply4 (args[0], args[1], args[2], args[3]);
                        break;
                    default:
                        return json_t {{"error", { 
                              { "code", error_too_many_arguments }
                            , { "message", "Not implemented: too many arguments." }
                        }}};
                }
                ret.push_back ({
                      { "value", r.value }
                    , { "uncertainty", r.uncertainty }
                    , { "unit", r_u }
                });
            }
            catch (std::exception const& e)
            {
                LOG->error ( format ("Internal error {}, {}", e.what (), std::source_location::current ().line ()));
                ret.push_back ({
                      { "error", e.what () }
                });
            }
            catch (...)
            {
                LOG->error ( format ("Internal error {}", std::source_location::current ().line ()));
                ret.push_back ({
                      { "error", "(unspecified)" }
                });
            }
        }
        if (ret.get_array ().size () == 1)
        {
            return json_t {{"result", ret[0]}, {"references", models_map.at (model)}};
        }
        return json_t {{"result", ret}, {"references", models_map.at (model)}};
    }
        auto
    get_references (json_t const& model) const
    {
        return json_t {{ "result", models_map.at (model.get_string ()) }};
    }
}; // class property_t

    static const auto
property = property_t {};

} // namespace calculisto::remote_services::module::property


    using calculisto::remote_services::module::property::property;
    extern "C"
{
    schema_t
module_schema = R"({
"title": "The property module",
"description": "Computes the physical properties of substances according to models.",
"definitions":{"schemas": {"definitions": {
    "signature":{
        "type": "object", 
        "properties":{
            "substance":  { "type": "string" },
            "property":   { "type": "string" },
            "model":      { "type": "string" },
            "conditions": { "type": "array", "items": { "type": "string"}}
        },
        "required": [ "substance", "property", "model", "conditions" ],
        "title": "function signature",
        "description": "The signature (i.e. the number and type of arguments it takes and its retunr type) of a function."
    },
    "filter":{
        "type": "object",
        "properties":{
            "substance":  { "type": "string" },
            "property":   { "type": "string" },
            "model":      { "type": "string" },
            "conditions": { "type": "array", "items": { "type": "string"}}
        }
    },
    "argument":{
        "oneOf":[{
            "type": "object",
            "properties":{
                "value":       { "oneOf": [{"type": "number"}, {"type": "array", "items": {"type": "number"}, "minItems": 1}] },
                "uncertainty": { "oneOf": [{"type": "number"}, {"type": "array", "items": {"type": "number"}}] },
                "unit":        { "type": "string" }
            },
            "required": [ "value", "unit" ],
            "title": "argument",
            "description": "An argument to a function"
        },{
            "type": "object",
            "properties":{
                "signature": { "$ref": "#/definitions/module_property/definitions/schemas/definitions/signature" },
                "arguments": {
                    "type": "object",
                    "additionalProperties": { "$ref": "#/definitions/module_property/definitions/schemas/definitions/argument" }
                }
            },
            "required": [ "signature", "arguments" ],
            "title": "forward argument",
            "description": "Use the result of a call to a method as argument"
        }]
    }
}}}
})"_json;

    calculisto::remote_services::methods_t
module_methods = 
{{
    "test", 
    {
          .expects = "{}"_json
        , .returns = R"({
            "type": "object", 
            "properties": {
                "message": { "type": "string" },
                "received": {}
            },
            "required": [ "message", "received" ]
          })"_json
        , .function = [](json_t const& params) -> json_t
        {
            return {{ "result", {
                  {"message", "Property module test ok"}
                , {"received", params}
            }}};
        }
        , .description = "A test function that does nothing."
    }
},{
    "list_substances",
    {
          .expects = R"({})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.substances ();
        }
        , .description = "Returns the list of substances the server knows."
    },
},{
    "list_properties",
    {
          .expects = R"({})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.properties ();
        }
        , .description = "Returns the list of properties the server knows."
    },
},{
    "list_conditions",
    {
          .expects = R"({})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.conditions ();
        }
        , .description = "Returns the list of conditions the server knows."
    },
},{
    "list_models",
    {
          .expects = R"({})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.models ();
        }
        , .description = "Returns the list of models the server knows."
    },
},{
    "list_prefixes",
    {
          .expects = R"({})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.prefixes ();
        }
        , .description = "Returns the list of SI prefixes the server knows."
    },
},{
    "list_units",
    {
          .expects = R"({})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.units ();
        }
        , .description = "Returns the list of SI physical units the server knows"
    },
},{
    "list_property_units",
    {
          .expects = R"({"type": "string"})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.property_units (params);
        }
        , .description = "Returns the list of SI physical units for a given property the server knows"
    },
},{
    "list_functions",
    {
          .expects = R"({})"_json
        , .returns = R"({"type": "array", "items": {"$ref": "#/definitions/module_property/definitions/schemas/definitions/signature"}})"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.functions ();
        }
        , .description = "Returns the list of function the server knows."
    },
},{
    "filter_functions",
    {
          .expects = R"({"$ref": "#/definitions/module_property/definitions/schemas/definitions/filter"})"_json
        , .returns = R"({"type": "array", "items": {"$ref": "#/definitions/module_property/definitions/schemas/definitions/signature"}})"_json
        , .function = [](json_t const& params)-> json_t
        {
            return property.filter_functions (params);
        }
        , .description = "Returns a list of functions matching some criteria."
    },
},{
    "execute_function",
    {
          .expects = R"({
              "type": "object"
            , "properties": 
              {
                  "signature": { "$ref": "#/definitions/module_property/definitions/schemas/definitions/signature" }
                , "arguments": {
                      "type": "object"
                    , "additionalProperties": { "$ref": "#/definitions/module_property/definitions/schemas/definitions/argument" }
                  }
              }
            , "required": ["signature", "arguments"]
          })"_json
        , .returns = R"({ "$ref": "#/definitions/module_property/definitions/schemas/definitions/argument" })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.execute_function (params);
        }
        , .description = "Executes a function and return its result."
    },
},{
    "get_references",
    {
          .expects = R"({"type": "string"})"_json
        , .returns = R"({
            "type": "array", 
            "items": 
            {
                "type": "string"
            }
          })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.get_references (params);
        }
        , .description = "Returns the bibliographic references for a model."
    },
}};
} // extern "C"

