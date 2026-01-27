#include "../include/Formatter.h"
#include <chrono>
#include <string>
#include <format>

static tm safe_localtime(const time_t& t) { //thread-safe version of localtime()
    tm result;
    #ifdef _WIN32
        localtime_s(&result, &t);
    #else
        localtime_r(&t, &result);
    #endif
    return result;
}

static tm safe_gmtime(const time_t& t) { //thread-safe version of gmtime()
    tm result;
    #ifdef _WIN32
        gmtime_s(&result, &t);
    #else
        gmtime_r(&t, &result);
    #endif
    return result;
}

std::string Formatter::formatTimestampLocal(const std::chrono::system_clock::time_point& tp) const{
    time_t t = std::chrono::system_clock::to_time_t(tp);
    tm tm = safe_localtime(t);

    return std::format(
        "{:04}-{:02}-{:02} {:02}:{:02}:{:02}",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
    );
}

std::string Formatter::formatTimestampUTC(const std::chrono::system_clock::time_point& tp) const{
    time_t t = std::chrono::system_clock::to_time_t(tp);
    tm tm = safe_gmtime(t);

    return std::format(
        "{:04}-{:02}-{:02} {:02}:{:02}:{:02}Z",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
    );
}