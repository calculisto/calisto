#pragma once
#include "logger.hpp"
#include "module_manager.hpp"
#include "isto/json_validator/json_validator.hpp"
    using isto::json_validator::validator_t;    
#include "schemas.hpp"
#include <fmt/chrono.h>


    using namespace tao::json;
    using namespace std::literals;

#define CORE_MODULE_NAME "core"

    namespace
isto::remote_services
{
    namespace 
json_rpc_error
{
        const std::string
    parse_error (){
        return R"({"jsonrpc":"2.0","error":{"code":-32700,"message":"Parse error. Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text."}})";
    }
        const std::string
    parse_error (std::string const& details){
        return R"({"jsonrpc":"2.0","error":{"code":-32700,"message":"Parse error. Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.","data":")" + details + R"("}})";
    }
        const std::string
    internal_error (){
        return R"({"jsonrpc":"2.0","error":{"code":-32603,"message":"Internal error."}})";
    }
        const std::string
    internal_error (std::string const& details){
        return R"({"jsonrpc":"2.0","error":{"code":-32603,"message":"Internal error.","data":")" + details + R"("}})";
    }
}

    constexpr auto
uri_rpc_request = "htpp://calcul-isto.cnrs-orleans.fr/schemas/json_rpc_request.schema.json";
    constexpr auto
uri_rpc_notification_or_request = "htpp://calcul-isto.cnrs-orleans.fr/schemas/json_rpc_request.schema.json#/definitions/notification_or_request";
    constexpr auto
uri_rpc_response = "htpp://calcul-isto.cnrs-orleans.fr/schemas/json_rpc_response.schema.json";



    class
request_processor_t
{
        validator_t
    validator_m;
        std::shared_ptr<spdlog::logger>
    logger_m;
        std::shared_ptr<spdlog::logger>
    access_log_m;
        json_t
    schema_m = {
          { "title", "CALISTO: Remote Numerical Services of the Earth Science Institute of Orléans." }
        , { "description", "The CALISTO server accepts calculation requests, performs the calculation and sends back the result. "
            "Requests and responses are exchanged usng the JSON-RPC protocol, version 2.0." 
          }
    };
        module_manager_t
    module_manager_m;

    //==================================================
    // Core module
        method_table_t
    method_table_m = 
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
            , .function = [](json_t const& params)-> json_t
              {
                return {{ "result", {
                      {"message", "test ok"}
                    , {"received", params}
                }}};
              }
             , .description = "A test function that does nothing"
        },
    },{
        "get_schema",
        {
              .expects = R"({})"_json
            , .returns = R"({
                "$ref": "http://json-schema.org/draft-07/schema#"
              })"_json
            , .function = [this]([[maybe_unused]] json_t const& params)-> json_t
              {
                return {{ "result", schema_m }};
              }
             , .description = "Returns the JSON Schema specifying the server's capabilities."
        },
    },{
        "list_modules",
        {
              .expects = R"({})"_json
            , .returns = R"({
                "type": "array", 
                "items": 
                {
                    "type": "string"
                }
              })"_json
            , .function = [this]([[maybe_unused]] json_t const& params)-> json_t
            {
                    json_t
                r = tao::json::empty_array;
                for (auto const& m: module_manager_m.module_table ())
                {
                    r.push_back (m.first);
                }
                return {{ "result", r }};
            }
            , .description = "Returns the list of modules loaded by the server."
        },
    },{
        "list_methods",
        {
              .expects = R"({
                "type": "string"
              })"_json
            , .returns = R"({
                "type": "array", 
                "items": 
                {
                    "type": "string"
                }
              })"_json
            , .function = [this]([[maybe_unused]] json_t const& module_name)-> json_t
              {
                    auto
                module = module_manager_m.module_table ().find (module_name.as <std::string> ());
                if (module == std::end (module_manager_m.module_table ()))
                {
                    return {{
                        "error", {
                              { "code", 1}
                            , { "message", "Module not found" }
                            , { "data", { { "module", module_name } } }
                            }
                    }};
                }
                    tao::json::value
                r = tao::json::empty_array;
                for (auto const& m: module->second.method_table ())
                {
                    r.push_back (m.first);
                }
                return {{ "result", r }};
              }
            , .description = "Returns the list of methods of the provided module."
        },
    /*
    },{
        "",
        {
              .expects = R"({})"_json
            , .returns = R"({})"_json
            , .function = []([[maybe_unused]] json_t const& params)-> json_t
            {
            }
        },
    */
    }};
public:
    request_processor_t ()
        : logger_m { spdlog::stdout_color_mt ("request_processor") }
        , access_log_m { spdlog::rotating_logger_mt ("access", "access.log", 1024*1024*100, 10) }
    {
        logger_m->info ("Loading the RPC schemas...");
        validator_m.add_schema (
              tao::json::from_string (schemas::json_rpc_request_schema)
            , uri_rpc_request
        );
        validator_m.add_schema (
              tao::json::from_string (schemas::json_rpc_response_schema)
            , uri_rpc_response
        );

        logger_m->info ("Loading the core module...");
        // core module
            auto
        module = module_manager_m.load (CORE_MODULE_NAME, method_table_m);
        schema_m["definitions"][CORE_MODULE_NAME] = module.schema ();
        schema_m["definitions"][CORE_MODULE_NAME]["title"] = "The Core module";
        schema_m["definitions"][CORE_MODULE_NAME]["description"] 
            = "Provides the basic functionalities.";
        // The other modules
            const std::vector <std::string>
        modules_names = {
              "property"
        };
        module_manager_m.add_path ("./");
        for (auto&& module_name: modules_names)
        {
            logger_m->info ("Loading the {} module...", module_name);
                auto
            module = module_manager_m.load (module_name);
            schema_m["definitions"][module_name] = module.schema ();
        }
        logger_m->info ("Final schema is {}", schema_m);
        validator_m.add_schema (
              schema_m
            , "http://calcul-isto.cnrs-orleans.fr/schemas/remote_services.schema.json"s
        );
    }
public:
        std::string
    handle_post (std::string const& request, std::string const& remote_addr) noexcept
    {
        // Catch-all, yields an internal error.
        try
        {
                auto
            start = std::chrono::system_clock::now ();
            logger_m->info ("Got request from {}: {}", remote_addr, request);
            // TODO Notifications.
                json_t
            json;
            try
            {
                json = tao::json::from_string (request);
            } 
            catch (std::exception const& e)
            {
                return json_rpc_error::parse_error (e.what ());
            }
            catch (...)
            {
                return json_rpc_error::parse_error ();
            }
                json_t
            response = {
                  { "jsonrpc", "2.0" }
            };
                auto
            [is_valid, errors] = validator_m.validate (json, uri_rpc_notification_or_request);
            if (!is_valid)
            {
                response["error"] = {
                      { "code", -32600}
                    , { "message", "Invalid request. The JSON sent is not a valid Request object." }
                    , { "data", errors }
                };
                logger_m->info ("Sending: {}", response);
                return tao::json::to_string (response);
            }
            // Has an id?
            if (json.find ("id") == nullptr)
            {
                response["error"] = {
                      { "code", -1}
                    , { "message", "Not yet implemented." }
                    , { "data", "JSON-RPC notifications are not yet implemented." }
                };
                logger_m->info ("Sending: {}", response);
                return tao::json::to_string (response);
            }
            response["id"] = json["id"];
                std::string
            qualified_method_name = json["method"].get_string ();
                std::string
            module_name = CORE_MODULE_NAME;
                std::string
            method_name = qualified_method_name;
            if (
                    auto
                  s = qualified_method_name.rfind ('/')
                ; s != std::string::npos 
            ){
                module_name = qualified_method_name.substr (0, s);
                method_name = qualified_method_name.substr (s+1);
            }
            logger_m->info ("Module is {}", module_name);
            logger_m->info ("Method is {}", method_name);
            // Get the module
                auto
            module = module_manager_m.module_table ().find (module_name);
            if (module == std::end (module_manager_m.module_table ()))
            {
                response["error"] = {
                      { "code", 1}
                    , { "message", "Module not found" }
                    , { "data", { { "module", module_name } } }
                };
                logger_m->info ("Sending: {}", response);
                return tao::json::to_string (response);
            }
            // Get the method
                auto
            method = module->second.method_table ().find (method_name);
            if (method == std::end (module->second.method_table ()))
            {
                response["error"] = {
                      { "code", 2}
                    , { "message", "Method not found" }
                    , { "data", { 
                          { "module", module_name } 
                        , { "method", method_name } 
                      }}
                };
                logger_m->info ("Sending: {}", response);
                return tao::json::to_string (response);
            }
            // Check the params against the schema
            // Should be useless.
            if (json.find ("params") == nullptr)
            {
                json["params"] = tao::json::null;
            }
                auto const&
            params = json["params"];
            logger_m->info ("Params is {}", params);
                auto
            p = 
                  "http://calcul-isto.cnrs-orleans.fr/schemas/remote_services.schema.json#/definitions/"s
                + module_name 
                + "/definitions/functions/definitions/"
                + method_name
                + "/definitions/expects"
            ;
                std::tie
            (is_valid, errors) = validator_m.validate (
                  params
                , p                
            );
            if (!is_valid)
            {
                response["error"] = {
                      { "code", -32602}
                    , { "message", "Invalid params" }
                    , { "data", { 
                          { "module", module_name } 
                        , { "method", method_name } 
                        , { "errors", errors }
                      }}
                };
                logger_m->info ("Sending: {}", response);
                return tao::json::to_string (response);
            }
            // Actually call the method.
            // TODO wrap in a try block?
                auto
            r = method->second.function (params);
            // TODO validate what it returned?
            // Merge what it returned
            // TODO assert r.is_object ()
            response.get_object ().insert (
                  std::begin (r.get_object ())
                , std::end   (r.get_object ())
            );
            logger_m->info ("Sending: {}", response);
                auto
            finish = std::chrono::system_clock::now ();
                auto
            duration = std::chrono::duration_cast <std::chrono::milliseconds>(finish - start);
            logger_m->info ("Processig took: {}", duration);
            return tao::json::to_string (response);
        }
        catch (std::exception const& e)
        {
            logger_m->error ("Internal error: {}", e.what ());
            return json_rpc_error::internal_error ();
        }
        catch (...)
        {
            logger_m->error ("Internal error");
            return json_rpc_error::internal_error ();
        }
    }
        std::string
    handle_get (std::string const& request)
    {
        logger_m->info ("GET {}", request);
        /*
            namespace
        fs = std::filesystem;
            auto
        p = fs::path (request);
        */
        return R"(
            <!DOCTYPE html>
            <html lang="en">
              <head>
                <meta charset="utf-8">
                <title>ISTO Web services</title>
              </head>
              <body>
              <p>Test.</p>
              </body>
            </html>
        )";
    }
};
} // namespace isto::remote_services

