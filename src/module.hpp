#pragma once
#include "method.hpp"
#include "logger.hpp"
#include <filesystem>
#include <vector>
#include <dlfcn.h>
    using namespace std::literals;

    namespace
isto::remote_services
{
    class
module_t 
{
public:
    /** Build a module from the handle of a shared object.
     */
    module_t (std::string const& name, void* handle)
        : logger_m { make_logger ("module "s + name) }
    {
        if ((methods_m = static_cast <methods_t*> (dlsym (handle, "module_methods"))))
        {
            logger_m->info ("Found a set of named methods");
        }
            schema_t*
        module_schema;
        if ((module_schema = static_cast <schema_t*> (dlsym (handle, "module_schema"))))
        {
            logger_m->info ("Found a schema");
        }
        build_schema (module_schema);
    }

    /** Build a module from a set of named methods.
     * Warning: we will store and use the address of module_methods argument, 
     * It shall remain valid at all times. 
     */
    module_t (std::string const& name, methods_t const& module_methods)
        : logger_m { make_logger ("module " + name) }
        , methods_m { std::addressof (module_methods) }
    {
        build_schema ();
    }

    /** Returns the set of named methods of this module.
     */
        auto const&
    methods () const
    {
        return *methods_m;
    }

    /** Returns the schema associated with this module.
     */
        auto const&
    schema () const
    {
        return schema_m;
    }
private:
        logger_t
    logger_m;
        methods_t const*
    methods_m = nullptr;
        schema_t
    schema_m;
        void
    build_schema (schema_t const* module_schema = nullptr)
    {
        if (module_schema) schema_m = *module_schema; 
        if (methods_m) for (auto [name, method]: *methods_m)
        {
                auto&
            x = schema_m["definitions"]["functions"]["definitions"][name];
            x["definitions"]["expects"] = method.expects;
            x["definitions"]["returns"] = method.returns;
            x["description"] = method.description;
            x["title"] = method.title;
        }
    }
};

/** A set of named modules.
 * It partially models a `std::map <std::string, module_t>`;
 */
    class
modules_t
{
public:
    /** Check if a module is loaded.
     */
        auto
    contains (std::string const& module_name) const
    {
        return map_m.contains (module_name);
    }
    /** @defgroup iterators ''Iterators''
     */
    // @{
        auto
    begin ()
    {
        return map_m.begin ();
    }
        auto
    begin () const
    {
        return map_m.begin ();
    }
        auto
    cbegin () const
    {
        return map_m.begin ();
    }
        auto
    end ()
    {
        return map_m.end ();
    }
        auto
    end () const
    {
        return map_m.end ();
    }
        auto
    cend () const
    {
        return map_m.end ();
    }
    // @}
    /** Find a module.
     */
        auto
    find (std::string const& name)
    {
        return map_m.find (name);
    }
    /** Load a module from a shared object.
     */
        auto const&
    load (std::string const& name)
        noexcept (false) 
    {
        if (contains (name))
        {
            throw std::runtime_error (
                  "Module \""s 
                + name 
                + "\" already loaded."
            );
        }
            auto 
        file = std::filesystem::path { name };
        file.replace_extension (".so");
        dlerror ();
            auto
        module_handle = dlopen (file.c_str (), RTLD_LAZY);
        if (!module_handle)
        {
            throw std::runtime_error (dlerror ());
        }
            auto
        [ iterator, dummy] = map_m.emplace (
              name
            , module_t { name,  module_handle }
        );
        return iterator->second;
    }
    /** Load a module from a set of named methods.
     */
        auto const&
    load (std::string const& name, methods_t const& module_methods)
    {
        if (contains (name))
        {
            throw std::runtime_error (
                  "Module \""s 
                + name 
                + "\" already loaded."
            );
        }
            auto
        [ iterator, dummy] = map_m.emplace (
              name
            , module_t { name, module_methods }
        );
        return iterator->second;
    }
private:
        using
    map_t = std::unordered_map <std::string, module_t>;
        map_t
    map_m;
};
} // namespace isto::remote_services
