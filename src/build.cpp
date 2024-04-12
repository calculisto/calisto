#include <string>
#include <vector>
#include <filesystem>
#include <fmt/std.h>
#include <fmt/chrono.h>
    using fmt::print;
#include "isto/json_validator/json_validator.hpp"
    using isto::json_validator::validator_t;

    using namespace std::literals;

    const auto
schema = R"({
    "$schema": "http://json-schema.org/draft-07/schema#",
    "$id": "htpp://calcul-isto.cnrs-orleans.fr/schemas/remote_services_property_entry.schema.json#",
    "title":   "ISTO Remote Services property entry",
    "type":"object",
    "properties": 
    {
        "header": { "type":"string" },
        "model": 
        {
            "type": "object",
            "properties":
            {
                "name": { "type": "string" },
                "dois": {
                    "anyOff":
                    [
                        { "type": "string" },
                        { "type": "array", "items": { "type": "string" } }
                    ]
                }
            },
            "required":
            [
                "name",
                "dois"
            ]
        },
        "functions": 
        { 
            "type":"array" ,
            "items":
            {
                "type":"object",
                "properties":
                {
                    "function":   { "type": "string" },
                    "substance":  { "type": "string" },
                    "model":      
                    { 
                        "anyOf": 
                        [
                            { "type": "string" },
                            { "type": "array", "items": { "type": "string" } }
                        ]
                    },
                    "property":   { "type": "string" },
                    "conditions": 
                    { 
                        "type": "array", 
                        "items": { "type": "string" } 
                    },
                    "dimension": { "type": "string" }
                },
                "required": 
                [
                    "function",
                    "substance",
                    "property",
                    "conditions",
                    "dimension"
                ]
            }
        }
    },
    "required":
    [
        "header",
        "model",
        "functions"
    ]
})";

    int
main (int argc, char** argv)
{
        auto
    validator = validator_t {};
    validator.add_schema (tao::json::from_string (schema), "");

        auto
    map = std::ostringstream {};
        auto
    header = std::ostringstream {};
        auto
    models = std::ostringstream {};

        const auto
    filenames = std::vector <std::filesystem::path> (argv + 1, argv + argc);
    for (auto filename: filenames)
    {
        if (!is_regular_file (filename))
        {
            print (
                  stderr
                , "Warning: skipping invalid regular file {}.\n"
                , filename
            );
            continue;
        }
        filename.replace_extension ("");
            const auto
        fn_json = filename.replace_extension (".json");
        if (!is_regular_file (fn_json))
        {
            print (
                  stderr
                , "Warning: skipping invalid regular file {}.\n"
                , filename
            );
            continue;
        }
            const auto
        json = tao::json::from_file (fn_json);
            auto
        [ is_valid, errors ] = validator.validate (json);
        if (!is_valid)
        {
            print (
                  stderr
                , "Warning: invalid JSON in file {}. Validator says: {}.\n"
                , filename
                , errors
            );
            continue;

        }

        print (map, "    // {}\n", fn_json);
        print (header, "#include {}\n", json.at ("header"));

            const auto
        model_id = json.at ("model").at ("name");
            const auto&
        m = json.at ("model").at ("dois");
        if (m.is_array ())
        {
            print (models, "    {{ {}, {{ {} }} }},\n", model_id, fmt::join (m.get_array (), ", "));
        }
        else
        {
            print (models, "    {{ {}, {{ {} }} }},\n", model_id, m);
        }


        for (auto&& e: json.at("functions").get_array ())
        {
                const auto&
            c = e.at ("conditions").get_array ();
            print (
                  map
                , "    {{\n          {{ {}, {}, {{ {} }}, {} }}\n"
                , e.at ("substance")
                , e.at ("property")
                , fmt::join (c, ", ")
                , model_id
            );
            print (
                  map
                , "        , {{ reinterpret_cast <void*> ({} <{}>), {}, {}, {} }}\n    }},\n"
                , e.at ("function").get_string ()
                , fmt::join (std::vector (c.size (), "uv_t"s), ", ")
                , e.at ("dimension").get_string ()
                , c.size ()
                , e.at ("dimension")
            );
        }
    }

    {
            const auto
        fn_out = std::filesystem::path { "module_property_map_data.inc" };
        if (exists (fn_out))
        {
            // TODO: bail out
            print (stderr , "WARNING: OUTPUT FILE EXISTS\n");
        }
            auto
        out = std::ofstream { fn_out };
        print (
              out
            , fmt::runtime ("// generated on {:%Y-%m-%d %X}\n")
            , std::chrono::system_clock::now ()
        );
        out << "    const auto\nmodels_map = std::unordered_map <std::string, std::vector <std::string>>\n{\n";
        out << models.str () << "\n};\n";
        print (out, "    map_t\nfunctions_map\n{{\n");
        out << map.str () << '\n';
        print (out, "}};\n");
    }
    {
            const auto
        fn_out = std::filesystem::path { "module_property_map_headers.inc" };
        if (exists (fn_out))
        {
            // TODO: bail out
            print (stderr , "WARNING: OUTPUT FILE EXISTS\n");
        }
            auto
        out = std::ofstream { fn_out };
        print (
              out
            , fmt::runtime ("// generated on {:%Y-%m-%d %X}\n")
            , std::chrono::system_clock::now ()
        );
        out << header.str () << '\n';
    }
}
