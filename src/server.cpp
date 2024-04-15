#include "server.hpp"
#include "request_processor.hpp"
#include <cxxopts.hpp>

int main(int argc, char **argv) 
{
        std::string
    address;
        int
    port;
        int
    threads;

    {
            cxxopts::Options 
        options("isto-rs-server", "ISTO remote services -- the server.");
        options.add_options ()
            ("h,help", "")
            (
                  "a,address"
                , "Listening address"
                , cxxopts::value (address)->default_value ("127.0.0.1")
            )
            (
                  "p,port"
                , "TCP port to use"
                , cxxopts::value (port)->default_value ("7327")
            )
            (
                  "t,threads"
                , "number of threads to use"
                , cxxopts::value (threads)->default_value ("1")
            )
        ;
            auto 
        args = options.parse (argc, argv);
    }

        using namespace calculisto::remote_services;

        request_processor_t
    request_processor;
        server_t
    server { address, port, threads, request_processor };
    return 0;
}
