#pragma once
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

    namespace
isto::remote_services
{
    using 
logger_t = std::shared_ptr <spdlog::logger>;

    inline auto
make_logger (std::string const& name)
{
        auto
    r = spdlog::stdout_color_mt (name);
    r->set_pattern ("%C-%m-%d %T.%e %^%l%$ %n: %v");
    return r;
}

    inline auto
make_logger_file (std::string const& name, std::string const& file)
{
        auto
    r = spdlog::rotating_logger_mt (name, file, 1024 * 1024 * 100, 10);
    r->set_pattern ("%C-%m-%d %T.%e %^%l%$ %n: %v");
    return r;
}
} // namespace isto::remote_services
