#pragma once
#include "method.hpp"
#include <filesystem>
#include <vector>
#include <dlfcn.h>
    using namespace std::literals;

    class
module_t 
{
public:
    module_t (void* handle)
        : logger_m { make_logger ("module_manager") }
        , handle_m { handle }
    {
        if ((method_table_m = static_cast <method_table_t*> (dlsym (handle, "method_table"))))
        {
            logger_m->info ("Found a method table");
        }
        if ((schema_table_m = static_cast <schema_t*> (dlsym (handle, "schema_table"))))
        {
            logger_m->info ("Found a schema table");
        }
        build_schema ();
    }
    module_t (method_table_t const& method_table)
        : handle_m { 0 }
        , method_table_m { std::addressof (method_table) }
    {
        build_schema ();
    }
        method_table_t const&
    method_table () const
    {
        return *method_table_m;
    }
        schema_t const&
    schema () const
    {
        return schema_m;
    }
private:
        decltype (spdlog::stdout_color_mt("")) 
    logger_m;
        void*
    handle_m;
        method_table_t const*
    method_table_m = nullptr;
        schema_t const*
    schema_table_m = nullptr;
        schema_t
    schema_m; // = {{ "definitions", tao::json::empty_object }};
        void
    build_schema ()
    {
        // FIXME: copy
        if (schema_table_m) schema_m = *schema_table_m;/*for (auto const& m: *schema_table_m)
        {
            schema_m["definitions"]["schemas"]["definitions"][m.first] = m.second;
        }
        */
        if (method_table_m) for (auto const& m: *method_table_m)
        {
            schema_m["definitions"]["functions"]["definitions"][m.first]["definitions"]["expects"] = m.second.expects;
            schema_m["definitions"]["functions"]["definitions"][m.first]["definitions"]["returns"] = m.second.returns;
            schema_m["definitions"]["functions"]["definitions"][m.first]["description"] = m.second.description;
            schema_m["definitions"]["functions"]["definitions"][m.first]["title"] = m.second.title;
        }
    }
};

    using
module_table_t = std::unordered_map <std::string, module_t>;

    class
module_manager_t
{
public:
        void
    add_path (std::filesystem::path const& path)
    {
        directories_m.push_back (path);
    }
        module_t const&
    load (std::string const& name)
        noexcept (false) 
    {
        if (module_exists (name))
        {
            throw std::runtime_error ("Module \""s + name + "\" already loaded.");
        }
            auto 
        file = std::filesystem::path { name };
        file.replace_extension (".so");
        for (auto const& directory: directories_m)
        {
            if (auto path = directory / file; exists (path))
            {
                dlerror ();
                    auto
                module_handle = dlopen (path.c_str (), RTLD_LAZY);
                if (!module_handle)
                {
                    throw std::runtime_error (dlerror ());
                }
                    auto
                [ iterator, dummy] = module_table_m.emplace (name, module_t {module_handle});
                return iterator->second;
            }
        }
        throw std::runtime_error ("Module file not found: "s + file.string ());
    }
        module_t const&
    load (std::string const& name, method_table_t const& method_table)
    {
        if (module_exists (name))
        {
            throw std::runtime_error ("Module \""s + name + "\" already loaded.");
        }
            auto
        [ iterator, dummy] = module_table_m.emplace (name, module_t { method_table });
        return iterator->second;
    }
        module_table_t const&
    module_table () const
    {
        return module_table_m;
    }
        bool
    module_exists (std::string const& module_name) const
    {
        return module_table_m.find (module_name) != std::end (module_table_m);
    }
private:
        std::vector <std::filesystem::path>
    directories_m;
        module_table_t
    module_table_m;
};

