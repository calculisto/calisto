#pragma once
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

    using 
logger_t = std::shared_ptr <spdlog::logger>;

    inline auto
make_logger (std::string const& name)
{
    spdlog::set_pattern ("%C-%m-%d %T.%e %^%l%$ %n: %v");
    return spdlog::stdout_color_mt (name);
}
