#pragma once
#include "logger.hpp"
#include <httplib.h>

    namespace
isto::remote_services
{
    template <class RequestProcessor>
    class
server_t
{
        RequestProcessor&
    request_processor_m;
        logger_t
    logger_m;
        httplib::Server
    server_m;
public:
    server_t (
          std::string address
        , int port
        , int threads
        , RequestProcessor& request_processor
    )
        : request_processor_m { request_processor }
        , logger_m { make_logger ("server") }
    {
            using namespace httplib;
        logger_m->info ("Starting server, listening on {}:{}", address, port);
        server_m.Options ("/", [](
              [[maybe_unused]] Request const& request
            , Response& response)
        {
            response.set_header ("Content-Type", "text/plain");
            response.set_header ("Access-Control-Allow-Origin", "*");
            response.set_header ("Access-Control-Allow-Methods", "POST");
            response.set_header ("Access-Control-Allow-Headers", "content-type");
        });
        server_m.Post ("/", [this](Request const& request, Response& response)
        {
            logger_m->info ("New request");
            logger_m->info ("Headers:");
            for (auto&& [k, v]: request.headers)
            {
                logger_m->info ("  {}: {}", k, v);
            }
                auto
            ra = request.headers.find ("REMOTE_ADDR");
            if (ra == std::end (request.headers))
            {
                logger_m->error ("Couldn't determine remote address");
                return;
            }
                auto
            r = request_processor_m.handle_post (request.body, ra->second);
            response.set_content (r, "application/json");
            response.set_header ("Access-Control-Allow-Origin", "*");
        });
        server_m.set_mount_point ("/", ".");
        server_m.listen (address.c_str (), port);
    }
    ~server_t ()
    {
        logger_m->info ("Shutting down.");
        server_m.stop ();
    }
};
} // namespace isto::remote_services
