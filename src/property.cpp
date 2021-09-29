#include "logger.hpp"
#include "method.hpp"
#include <isto/iapws/r6.hpp>
#include <isto/iapws/r7.hpp>
#include <isto/iapws/r10.hpp>
    using namespace isto::iapws;
#include <isto/units/units.hpp>
    using namespace isto::units;
    using namespace unit;
#include <isto/uncertain_value/uncertain_value.hpp>
    using isto::uncertain_value::uncertain_value_t;
#include <isto/multikey_multimap/multikey_multimap.hpp>
    using isto::multikey_multimap::multikey_multimap_t;
#include <isto/units/unit_parser.hpp>

#include <tao/json/contrib/traits.hpp>

    using namespace std::literals;

#include <isto/uncertain_value/uncertain_value_fmt.hpp>
#include <isto/units/dimension_fmt.hpp>
    using fmt::format;
// Sample: ideal gas {{{
    namespace
ideal_gas_law_air
{
        constexpr auto
    specific_gas_constant = 287.058 * joule <> / kilogram <> / kelvin <>;

        template <class T>
        constexpr auto
    pressure (
          density_t <T>     const& density
        , temperature_t <T> const& temperature
    ){
        return density * specific_gas_constant * temperature;
    }
        template <class T>
        constexpr auto
    pressure (
          temperature_t <T> const& temperature
        , density_t <T>     const& density
    ){
        return density * specific_gas_constant * temperature;
    }
        template <class T>
        constexpr auto
    density (
          pressure_t <T>    const& pressure
        , temperature_t <T> const& temperature
    ){
        return pressure / specific_gas_constant / temperature;
    }
        template <class T>
        constexpr auto
    density (
          temperature_t <T> const& temperature
        , pressure_t <T>    const& pressure
    ){
        return pressure / specific_gas_constant / temperature;
    }
} // namespace ideal_gas_law_air }}}

#include "property_disambiguate.hpp"

    namespace
isto::remote_services::module::property
{

    using
uv_t = uncertain_value_t <double>;
    using
function_t = uv_t (*) (uv_t const&, uv_t const&);
    struct
function_entry_t
{
    function_t function;
    dimension_t dimension;
        auto 
    operator<=>(function_entry_t const& e) const
    {
        return 
                reinterpret_cast <void*> (function) 
            <=> reinterpret_cast <void*> (e.function)
        ;
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
    , std::string                          // substance
    , std::string                          // property
    , std::pair <std::string, std::string> // conditions
    , std::string                          // model
>;
    
    using
signature_t = map_t::key_type;

    value
signature_to_json (signature_t const& s)
{
    return value {
          { "substance",  std::get <0> (s) }
        , { "property",   std::get <1> (s) }
        , { "conditions", value::array ({ std::get <2> (s).first, std::get <2> (s).second }) }
        , { "model",      std::get <3> (s) }
    };
}
/*
    signature_t
json_to_signature (json_t const& j)
{
    return signature_t { 
          s.at ("substance").as <std::string> ()
        , s.at ("property").as <std::string> ()
        , std::pair { s.at ("conditions").at (0).as <std::string> (), s.at ("conditions").at (1).as <std::string> () }
        , s.at ("model").as <std::string> ()
    };
}
*/
    template <class...> struct S;

    class
property_t
{
private:
        inline static logger_t
    logger_m = make_logger ("module/property");
        map_t const& 
    map_m;
        std::set <std::string>
    substances_m;
        std::set <std::string>
    properties_m;
        std::set <std::string>
    conditions_m;
        std::set <std::string>
    models_m;
        value
    substances_json_m;
        value
    properties_json_m;
        value
    conditions_json_m;
        value
    models_json_m;
        value
    prefixes_json_m;
        value
    units_json_m;
        value
    functions_json_m;
        value
    property_units_m;
public:
        explicit
    property_t (map_t const& map)
        : map_m { map }
    {
            using namespace tao::json;
        logger_m->info ("Initializing...");
        for (auto const& [k, v]: map)
        {
            substances_m.insert (std::get <0> (k));
            properties_m.insert (std::get <1> (k));
            conditions_m.insert (std::get <2> (k).first);
            conditions_m.insert (std::get <2> (k).second);
            models_m.insert (std::get <3> (k));
        }
        logger_m->info ("There are {} substances", substances_m.size ());
        logger_m->info ("There are {} properties", properties_m.size ());
        logger_m->info ("There are {} conditions", conditions_m.size ());
        logger_m->info ("There are {} models",     models_m.size ());
        substances_json_m = value {{ "result", substances_m }};
        substances_json_m = value {{ "result", substances_m }};
        properties_json_m = value {{ "result", properties_m }};
        conditions_json_m = value {{ "result", conditions_m }};
        models_json_m     = value {{ "result", models_m }};
            value
        tmp = empty_array;
        for (auto&& [symbol, name]: prefix::symbols_to_names)
        {
            tmp.push_back (symbol);
        }
        logger_m->info ("There are {} prefixes", tmp.get_array ().size ());
        prefixes_json_m   = value {{ "result", std::move (tmp) }};
        tmp = empty_array;
        for (auto&& [symbol, name]: unit::symbols_to_names)
        {
            tmp.push_back (symbol);
        }
        logger_m->info ("There are {} units", tmp.get_array ().size ());
        units_json_m   = value {{ "result", std::move (tmp) }};
        tmp = empty_array;
        for (auto const& [k, v]: map)
        {
            tmp.push_back (signature_to_json (k));
        }
        logger_m->info ("There are {} functions", tmp.get_array ().size ());
        functions_json_m = {{ "result", std::move (tmp) }};
        property_units_m = empty_object;
        for (auto const& [d, v]: unit::dimension_names_to_unit_names)
        {
            property_units_m[d] = empty_array;
            logger_m->info ("  {}", d);
            for (auto const& u: v)
            {
                logger_m->info ("     {}", u);
                property_units_m[d].push_back (unit::names_to_symbols.at (std::string {u}));
            }
            if (property_units_m[d].get_array ().size () == 0)
            {
                property_units_m[d].push_back (base_unit_symbol (dimension::names_to_values.at (d)));
            }
        }
        logger_m->info ("Multimap content:");
        for (auto&& [k, v]: map_m)
        {
            logger_m->info (
                  "  {} {} {} {} {} {} {}"
                , reinterpret_cast <void*> (v.function)
                , v.dimension, std::get <0> (k)
                , std::get <0> (k)
                , std::get <1> (k)
                , std::get <2> (k).first
                , std::get <2> (k).second
                , std::get <3> (k)
            );
        }
        logger_m->info ("Initialization done.");
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
            return value {{ "result", *i }};
        }
        return value {{"error", { 
              { "code", 9 }
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
        // std::string                          // substance
        // std::string                          // property
        // std::pair <std::string, std::string> // conditions
        // std::string                          // model
        // =====================================================================
            auto
        s = std::optional <std::string> {};
        if (auto is = params.find ("substance"); is && *is != "")
        {
            s = is->as <std::string> ();
            logger_m->info ("  specified substance: {}", *s);
            if (!map_m.contains <0> (*s))
            {
                return value {{"error", { 
                      { "code", 7 }
                    , { "message", format ("unkown substance: {}", *s) }
                }}};
            }
        }
        else
        {
            logger_m->info ("  unspecified substance");
        }
            auto
        p = std::optional <std::string> {};
        if (auto ip = params.find ("property"); ip && *ip != "")
        {
            p = ip->as <std::string> ();
            logger_m->info ("  specified property: {}", *p);
            if (! map_m.contains <1> (*p))
            {
                return value {{"error", { 
                      { "code", 7 }
                    , { "message", format ("unkown property: {}", *p) }
                }}};
            }
        }
        else
        {
            logger_m->info ("  unspecified property");
        }
            auto
        m = std::optional <std::string> {};
        if (auto im = params.find ("model"); im && *im != "")
        {
            m = im->as <std::string> ();
            logger_m->info ("  specified model: {}", *m);
            if (! map_m.contains <3> (*m))
            {
                return value {{"error", { 
                      { "code", 7 }
                    , { "message", format ("unkown model: {}", *m) }
                }}};
            }
        }
        else
        {
            logger_m->info ("  unspecified model");
        }
            auto
        c = std::optional <std::pair <std::string, std::string>> {};
        // As per the schema, the array has two elements
        if (auto ic = params.find ("conditions"); ic) 
        {
                auto const
            c1 = (*ic)[0].get_string ();
                auto const
            c2 = (*ic)[1].get_string ();
            logger_m->info ("  specified conditions: {}, {}", c1, c2);
            if (! map_m.contains <2> ({c1, c2}))
            {
                return value {{"error", { 
                      { "code", 7 }
                    , { "message", format ("unkown conditions: {}, {}", c1, c2) }
                }}};
            }
            c = { c1, c2 };
        }
        else
        {
            logger_m->info ("  unspecified conditions");
        }

            auto
        r = std::vector <map_t::const_iterator> {};
        map_m.select (std::back_inserter (r), s, p, c, m);
        logger_m->info ("  got {} matches", r.size ());
            auto
        j = value ( empty_array );
        for (auto&& e: r)
        {
            logger_m->info (
                  "    {} {} ({}, {}) {}"
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first).first
                , std::get <2> (e->first).second
                , std::get <3> (e->first)
            );
            j.push_back (signature_to_json (e->first));
        }
        return value {{"result", j }};
    }
private:
        class
    iterable_t
    {
    public:
        iterable_t (json_t const& json)
        {
            if (json.is_array ())
            {
                is_array_m = true;
                array_m = &(json.get_array ());
                logger_m->info ("Array of {} values", array_m->size ());
                return;
            }
            is_array_m = false;
            if (json.is_number ())
            {
                value_m = json.as <double> ();
                logger_m->info ("Scalar: {}", value_m);
                return;
            }
            logger_m->info ("Default (0.0)");
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
                return (*array_m)[index_m].as <double> ();
            }
            return value_m;
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
        // TODO: use a std::vector
            std::vector <json_t> const*
        array_m;
            double
        value_m;
    };
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
                ++(owner_m->v1_m);
                ++(owner_m->u1_m);
                ++(owner_m->v2_m);
                ++(owner_m->u2_m);
                ++index_m;
                return *this;
            }
                auto
            operator * () const
                -> std::pair <uv_t, uv_t>
            {
                return std::pair {
                      uv_t { *(owner_m->v1_m), *(owner_m->u1_m) }
                    , uv_t { *(owner_m->v2_m), *(owner_m->u2_m) } 
                };
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
        magic_t (json_t const& json)
            : v1_m { json.at (0).at ("value") }
            , u1_m { get_uncertainty (json.at (0)) }
            , v2_m { json.at (1).at ("value") }
            , u2_m { get_uncertainty (json.at (1)) }
            , size_m { std::max (v1_m.size (), v2_m.size ()) }
        {
            logger_m->info ("There are {} values", size_m);
            // FIXME: what if 
            //      v1_m.size () > 1 
            //   && v2_m.size () > 1 
            //   && v1_m.size () != v2_m.size ()
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
            iterable_t
          v1_m
        , u1_m
        , v2_m
        , u2_m
        ;
            std::size_t
        size_m = 0;
    };
public:
    // This implementation is proof-of-concept.
        auto
    execute_function (json_t const& params) const
    {
            auto const&
        s = params.at ("signature");
        logger_m->info ("  substance:   {}", s.at ("substance").as <std::string> ());
        logger_m->info ("  property:    {}", s.at ("property").as <std::string> ());
        logger_m->info ("  condition 1: {}", s.at ("conditions").at (0).as <std::string> ()); 
        logger_m->info ("  condition 2: {}", s.at ("conditions").at (1).as <std::string> ());
        logger_m->info ("  model:       {}", s.at ("model").as <std::string> ());

        auto substance  = s.at ("substance").as <std::string> ();
        auto property   = s.at ("property").as <std::string> ();
        auto condition1 = s.at ("conditions").at (0).as <std::string> ();
        auto condition2 = s.at ("conditions").at (1).as <std::string> ();
        auto model      = s.at ("model").as <std::string> ();
        auto function_entry_it = map_m.find (signature_t { 
              substance
            , property
            , { condition1, condition2 }
            , model
        });
        if (function_entry_it == std::end (map_m))
        {
            return value {{"error", { 
                  { "code", 9 }
                , { "message", "Function no found" }
            }}};
        }
            auto const&
        function_entry = function_entry_it->second;
        logger_m->info ("       {}", dimension::values_to_names.at (function_entry.dimension));
        logger_m->info ("       {}", unit::dimension_names_to_unit_names.at (dimension::values_to_names.at (function_entry.dimension)).size ());
            auto const&
        uv = unit::dimension_names_to_unit_names.at (dimension::values_to_names.at (function_entry.dimension));
            auto const
        r_u = uv.size () == 0
            ? base_unit_symbol (function_entry.dimension)
            : unit::names_to_symbols.at (uv[0])
        ;
        /*
            auto const
        uv = unit::dimensions_to_units <double>.find (function_entry.dimension);
            auto const
        r_u = uv == unit::dimensions_to_units <double>.end () 
            ? base_unit_symbol (function_entry.dimension)
            : unit::values_to_symbols <double>. at (uv->second.at (0))
        ;
        */
        
        /*
            auto const&
        uv = unit::dimensions_to_units <double>.at (function_entry.dimension);
        if (uv.size () == 0)
        {
            return value {{"error", { 
                  { "code", 11 }
                , { "message", fmt::format ("No unit found for dimension {}", function_entry.dimension) }
            }}};
        }
            auto const&
        u = uv.at (0);
        //u = best_unit <double> (function_entry.dimension);
        //BUG logger_m->info ("  Unit: {}", u);
            auto const
        r_u = unit::values_to_symbols <double>.at (u);
        */
        /*
            auto const
        us = unit::values_to_symbols <double>.find (u);
            auto const
        r_u = us == unit::values_to_symbols <double>.end () 
            ? base_unit_symbol (function_entry.dimension)
            : us->second
        ;
        */

        // Arguments
        // =====================================================================
            auto const
        u1 = params.at ("arguments").at (0).at ("unit").as <std::string> ();
            auto const
        uu1 = unit_parse_opt (u1);
        if (!uu1)
        {
            return value {{"error", { 
                  { "code", 9 }
                , { "message", "Can't parse first unit" }
                , { "data", u1 }
            }}};
        }
        if (uu1->dimension != dimension::names_to_values.at (condition1))
        {
            return value {{"error", { 
                  { "code", 9 }
                , { "message", "First unit (" + u1 + ") is not a " + condition1 + " unit" }
                , { "data", u1 }
            }}};
        }
            auto const
        u2 = params.at ("arguments").at (1).at ("unit").as <std::string> ();
            auto const
        uu2 = unit_parse_opt (u2);
        if (!uu2)
        {
            return value {{"error", { 
                  { "code", 9 }
                , { "message", "Can't parse second unit" }
                , { "data", u2 }
            }}};
        }
        if (uu2->dimension != dimension::names_to_values.at (condition2))
        {
            return value {{"error", { 
                  { "code", 9 }
                , { "message", "Second unit (" + u2 + ") is not a " + condition2 + " unit" }
                , { "data", u2 }
            }}};
        }
        //BUG logger_m->info ("First  unit: {}", *uu1);
        //BUG logger_m->info ("Second unit: {}", *uu2);
            json_t
        ret = empty_array;
        for (auto&& [arg1, arg2]: magic_t (params.at ("arguments")))
        {
                auto const
            r = function_entry.function ((arg1 * *uu1).magnitude, (arg2 * *uu2).magnitude);
                logger_m->info ("result: {}", r);
            ret.push_back ({
                  { "value", r.value }
                , { "uncertainty", r.uncertainty }
                , { "unit", r_u }
            });
        }
        if (ret.get_array ().size () == 1)
        {
            return value {{"result", ret[0]}};
        }
        return value {{"result", ret}};
        /*
        return value {{"error", { 
              { "code", 9 }
            , { "message", "Working on implementation" }
        }}};
        */
    }
};

// What is the value of <property> of <substance/solution> at 
// <conditions> according to <model>

#define ENTRY1(SUBSTANCE, PROPERTY, CONDITION1, CONDITION2, MODEL, FUNCTION, DIMENSION) \
{                                                                                       \
      {                                                                                 \
          #SUBSTANCE                                                                    \
        , #PROPERTY                                                                     \
        , {                                                                             \
              #CONDITION1                                                               \
            , #CONDITION2                                                               \
          }                                                                             \
        , #MODEL                                                                        \
      }                                                                                 \
    , {                                                                                 \
          reinterpret_cast <function_t> (static_cast <                                  \
              DIMENSION##_t <uv_t>(*)(                                                  \
                    CONDITION1##_t <uv_t> const&                                        \
                  , CONDITION2##_t <uv_t> const&                                        \
              )                                                                         \
          > (FUNCTION <uv_t>))                                                          \
        ,  dimension::DIMENSION                                                         \
      }                                                                                 \
}

#define ENTRY2(SUBSTANCE, PROPERTY, CONDITION1, CONDITION2, MODEL, FUNCTION) \
    ENTRY1(SUBSTANCE, PROPERTY, CONDITION1, CONDITION2, MODEL, FUNCTION, PROPERTY)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
    static const map_t
map
{
      ENTRY2( air,   pressure,                       density,         temperature,     Clapeyron, ideal_gas_law_air::pressure                               )
    , ENTRY2( air,   pressure,                       temperature,     density,         Clapeyron, ideal_gas_law_air::pressure                               )
    , ENTRY2( air,   density,                        temperature,     pressure,        Clapeyron, ideal_gas_law_air::density                                )
    , ENTRY2( air,   density,                        pressure,        temperature,     Clapeyron, ideal_gas_law_air::density                                )
    , ENTRY2( water, pressure,                       density,         temperature,     IAPWS_R6,  r6::pressure                                              )
    , ENTRY2( water, pressure,                       temperature,     density,         IAPWS_R6,  r6::pressure                                              )
    , ENTRY1( water, massic_internal_energy,         density,         temperature,     IAPWS_R6,  r6::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         temperature,     density,         IAPWS_R6,  r6::massic_internal_energy,         massic_energy         )
    , ENTRY2( water, massic_entropy,                 density,         temperature,     IAPWS_R6,  r6::massic_entropy                                        )
    , ENTRY2( water, massic_entropy,                 temperature,     density,         IAPWS_R6,  r6::massic_entropy                                        )
    , ENTRY1( water, massic_enthalpy,                density,         temperature,     IAPWS_R6,  r6::massic_enthalpy,                massic_energy         )
    , ENTRY1( water, massic_enthalpy,                temperature,     density,         IAPWS_R6,  r6::massic_enthalpy,                massic_energy         )
    , ENTRY1( water, massic_isochoric_heat_capacity, density,         temperature,     IAPWS_R6,  r6::massic_isochoric_heat_capacity, massic_heat_capacity  )
    , ENTRY1( water, massic_isochoric_heat_capacity, temperature,     density,         IAPWS_R6,  r6::massic_isochoric_heat_capacity, massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  density,         temperature,     IAPWS_R6,  r6::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  temperature,     density,         IAPWS_R6,  r6::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_gibbs_free_energy,       density,         temperature,     IAPWS_R6,  r6::massic_gibbs_free_energy,       massic_energy         )
    , ENTRY1( water, massic_gibbs_free_energy,       temperature,     density,         IAPWS_R6,  r6::massic_gibbs_free_energy,       massic_energy         )
    , ENTRY1( water, speed_of_sound,                 density,         temperature,     IAPWS_R6,  r6::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 temperature,     density,         IAPWS_R6,  r6::speed_of_sound,                 velocity              )
    , ENTRY1( water, density,                        massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        massic_enthalpy, pressure,        IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        massic_entropy,  pressure,        IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        pressure,        massic_enthalpy, IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        pressure,        massic_entropy,  IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        pressure,        temperature,     IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, density,                        temperature,     pressure,        IAPWS_R7,  r7::density,                        density               )
    , ENTRY1( water, massic_enthalpy,                massic_entropy,  pressure,        IAPWS_R7,  r7::massic_enthalpy,                massic_enthalpy       )
    , ENTRY1( water, massic_enthalpy,                pressure,        massic_entropy,  IAPWS_R7,  r7::massic_enthalpy,                massic_enthalpy       )
    , ENTRY1( water, massic_enthalpy,                pressure,        temperature,     IAPWS_R7,  r7::massic_enthalpy,                massic_enthalpy       )
    , ENTRY1( water, massic_enthalpy,                temperature,     pressure,        IAPWS_R7,  r7::massic_enthalpy,                massic_enthalpy       )
    , ENTRY1( water, massic_entropy,                 massic_enthalpy, pressure,        IAPWS_R7,  r7::massic_entropy,                 massic_entropy        )
    , ENTRY1( water, massic_entropy,                 pressure,        massic_enthalpy, IAPWS_R7,  r7::massic_entropy,                 massic_entropy        )
    , ENTRY1( water, massic_entropy,                 pressure,        temperature,     IAPWS_R7,  r7::massic_entropy,                 massic_entropy        )
    , ENTRY1( water, massic_entropy,                 temperature,     pressure,        IAPWS_R7,  r7::massic_entropy,                 massic_entropy        )
    , ENTRY1( water, massic_internal_energy,         massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         massic_enthalpy, pressure,        IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         massic_entropy,  pressure,        IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         pressure,        massic_enthalpy, IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         pressure,        massic_entropy,  IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         pressure,        temperature,     IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_internal_energy,         temperature,     pressure,        IAPWS_R7,  r7::massic_internal_energy,         massic_energy         )
    , ENTRY1( water, massic_isobaric_heat_capacity,  massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  massic_enthalpy, pressure,        IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  massic_entropy,  pressure,        IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  pressure,        massic_enthalpy, IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  pressure,        massic_entropy,  IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  pressure,        temperature,     IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_isobaric_heat_capacity,  temperature,     pressure,        IAPWS_R7,  r7::massic_isobaric_heat_capacity,  massic_heat_capacity  )
    , ENTRY1( water, massic_volume,                  massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  massic_enthalpy, pressure,        IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  massic_entropy,  pressure,        IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  pressure,        massic_enthalpy, IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  pressure,        massic_entropy,  IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  pressure,        temperature,     IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, massic_volume,                  temperature,     pressure,        IAPWS_R7,  r7::massic_volume,                  massic_volume         )
    , ENTRY1( water, pressure,                       massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::pressure,                       pressure              )
    , ENTRY1( water, pressure,                       massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::pressure,                       pressure              )
    , ENTRY1( water, speed_of_sound,                 massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 massic_enthalpy, pressure,        IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 massic_entropy,  pressure,        IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 pressure,        massic_enthalpy, IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 pressure,        massic_entropy,  IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 pressure,        temperature,     IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, speed_of_sound,                 temperature,     pressure,        IAPWS_R7,  r7::speed_of_sound,                 velocity              )
    , ENTRY1( water, temperature,                    massic_enthalpy, massic_entropy,  IAPWS_R7,  r7::temperature,                    temperature           )
    , ENTRY1( water, temperature,                    massic_enthalpy, pressure,        IAPWS_R7,  r7::temperature,                    temperature           )
    , ENTRY1( water, temperature,                    massic_entropy,  massic_enthalpy, IAPWS_R7,  r7::temperature,                    temperature           )
    , ENTRY1( water, temperature,                    massic_entropy,  pressure,        IAPWS_R7,  r7::temperature,                    temperature           )
    , ENTRY1( water, temperature,                    pressure,        massic_enthalpy, IAPWS_R7,  r7::temperature,                    temperature           )
    , ENTRY1( water, temperature,                    pressure,        massic_entropy,  IAPWS_R7,  r7::temperature,                    temperature           )
    , ENTRY1( iceIh, massic_enthalpy,                pressure,        temperature,     IAPWS_R10, r10::massic_enthalpy,               massic_enthalpy       )
    , ENTRY1( iceIh, massic_helmholtz_energy,        pressure,        temperature,     IAPWS_R10, r10::massic_helmholtz_energy,       massic_energy         )
    , ENTRY1( iceIh, massic_internal_energy,         pressure,        temperature,     IAPWS_R10, r10::massic_internal_energy,        massic_energy         )
    , ENTRY1( iceIh, massic_entropy,                 pressure,        temperature,     IAPWS_R10, r10::massic_entropy,                massic_entropy        )
    , ENTRY1( iceIh, massic_isobaric_heat_capacity,  pressure,        temperature,     IAPWS_R10, r10::massic_isobaric_heat_capacity, massic_heat_capacity  )
    , ENTRY1( iceIh, density,                        pressure,        temperature,     IAPWS_R10, r10::density,                       density               )
    /*
    , ENTRY1( iceIh, cubic_expansion_coefficient,    pressure,        temperature,     IAPWS_R10, r10::cubic_expansion_coefficient,   expansion_coefficient )
    , ENTRY1( iceIh, pressure_coefficient,           pressure,        temperature,     IAPWS_R10, r10::pressure_coefficient,          pressure_coefficient  )
    , ENTRY1( iceIh, isothermal_compressibility,     pressure,        temperature,     IAPWS_R10, r10::isothermal_compressibility,    compressibility       )
    , ENTRY1( iceIh, isentropic_compressibility,     pressure,        temperature,     IAPWS_R10, r10::isentropic_compressibility,    compressibility       )
    */
    , ENTRY1( iceIh, massic_enthalpy,                temperature,     pressure,        IAPWS_R10, r10::massic_enthalpy,               massic_enthalpy       )
    , ENTRY1( iceIh, massic_helmholtz_energy,        temperature,     pressure,        IAPWS_R10, r10::massic_helmholtz_energy,       massic_energy         )
    , ENTRY1( iceIh, massic_internal_energy,         temperature,     pressure,        IAPWS_R10, r10::massic_internal_energy,        massic_energy         )
    , ENTRY1( iceIh, massic_entropy,                 temperature,     pressure,        IAPWS_R10, r10::massic_entropy,                massic_entropy        )
    , ENTRY1( iceIh, massic_isobaric_heat_capacity,  temperature,     pressure,        IAPWS_R10, r10::massic_isobaric_heat_capacity, massic_heat_capacity  )
    , ENTRY1( iceIh, density,                        temperature,     pressure,        IAPWS_R10, r10::density,                       density               )
    /*
    , ENTRY1( iceIh, cubic_expansion_coefficient,    temperature,     pressure,        IAPWS_R10, r10::cubic_expansion_coefficient,   expansion_coefficient )
    , ENTRY1( iceIh, pressure_coefficient,           temperature,     pressure,        IAPWS_R10, r10::pressure_coefficient,          pressure_coefficient  )
    , ENTRY1( iceIh, isothermal_compressibility,     temperature,     pressure,        IAPWS_R10, r10::isothermal_compressibility,    compressibility       )
    , ENTRY1( iceIh, isentropic_compressibility,     temperature,     pressure,        IAPWS_R10, r10::isentropic_compressibility,    compressibility       )
    */
};
#pragma GCC diagnostic pop
#undef ENTRY1
#undef ENTRY2

    auto
x = r10::cubic_expansion_coefficient <uv_t> (temperature_t <uv_t> {}, pressure_t <uv_t> {});

    static const auto
property = property_t { map };

} // namespace isto::remote_services::module::property


    using isto::remote_services::module::property::property;
    extern "C"
{
    //schema_table_t
    schema_t
schema_table = R"({
"title": "The property module",
"description": "Computes the physical properties of substances according to models.",
"definitions":{"schemas": {"definitions": {
    "signature":{
        "type": "object", 
        "properties":{
            "substance":  { "type": "string" },
            "property":   { "type": "string" },
            "model":      { "type": "string" },
            "conditions": { "type": "array", "items": { "type": "string"}, "minItems": 2, "maxItems": 2}
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
            "conditions": { "type": "array", "items": { "type": "string"}, "maxItems": 2, "minItems": 2}
        }
    },
    "argument":{
        "type": "object",
        "properties":{
            "value":       { "oneOf": [{"type": "number"}, {"type": "array", "items": {"type":"number"}}] },
            "uncertainty": { "oneOf": [{"type": "number"}, {"type": "array", "items": {"type":"number"}}] },
            "unit":        { "type": "string" }
        },
        "required": [ "value", "unit" ],
        "title": "argument",
        "description": "An argument to a function"
    }
}}}
})"_json;
/*
{{
      "signature"
    , R"({
          "type": "object"
        , "properties": 
          {
              "substance":  { "type": "string" }
            , "property":   { "type": "string" }
            , "model":      { "type": "string" }
            , "conditions": { "type": "array", "items": { "type": "string"}, "minItems": 2, "maxItems": 2}
          }
        , "required": [ "substance", "property", "model", "conditions" ]
        , "title": "function signature"
        , "description": "The signature (i.e. the number and type of arguments it takes and its retunr type) of a function."

      })"_json
},{
      "filter"
    , R"({
          "type": "object"
        , "properties": 
          {
                "substance":  { "type": "string" }
              , "property":   { "type": "string" }
              , "model":      { "type": "string" }
              , "conditions": { "type": "array", "items": { "type": "string"}, "maxItems": 2}
          }
      })"_json
},{
      "argument"
    , R"({ 
          "type": "object"  
        , "properties":
          {
              "value":       { "oneOf": [{"type": "number"}, {"type": "array", "items": {"type":"number"}}] }
            , "uncertainty": { "oneOf": [{"type": "number"}, {"type": "array", "items": {"type":"number"}}] }
            , "unit":        { "type": "string" }
          }
        , "required": [ "value", "unit" ]
        , "title": "argument"
        , "description": "An argument to a function"
      })"_json
}};
*/
    method_table_t
method_table = 
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
        , .returns = R"({"type": "array", "items": {"$ref": "#/definitions/property/definitions/schemas/definitions/signature"}})"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.functions ();
        }
        , .description = "Returns the list of function the server knows."
    },
},{
    "filter_functions",
    {
          .expects = R"({"$ref": "#/definitions/property/definitions/schemas/definitions/filter"})"_json
        , .returns = R"({"type": "array", "items": {"$ref": "#/definitions/property/definitions/schemas/definitions/signature"}})"_json
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
                  "signature": { "$ref": "#/definitions/property/definitions/schemas/definitions/signature" }
                , "arguments": {
                      "type": "array"
                    , "items": { "$ref": "#/definitions/property/definitions/schemas/definitions/argument" }
                    , "minItems": 2
                    , "maxItems": 2
                  }
              }
            , "required": ["signature", "arguments"]
          })"_json
        , .returns = R"({ "$ref": "#/definitions/property/definitions/schemas/definitions/argument" })"_json
        , .function = []([[maybe_unused]] json_t const& params)-> json_t
        {
            return property.execute_function (params);
        }
        , .description = "Executes a function and return its result."
    },
}};
} // extern "C"

// property/execute_function {"signature": {"substance":"air","property":"density","model":"Clapeyron","conditions":["pressure","temperature"]},"arguments":[{"value":1e5,"uncertainty":10,"unit":"Pa"},{"value":293,"unit":"K"}]}
 

// vim: fdm=marker
