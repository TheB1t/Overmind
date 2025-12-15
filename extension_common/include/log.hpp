#pragma once

#include <tinyformat.h>
#include <iostream>
#include <fstream>
#include <mutex>

#define _LOG_VA_ARGS(...) , ##__VA_ARGS__
#define LOG(level, format, ...) Log::get_instance().printf(level, __FILE__, __LINE__, __func__, format _LOG_VA_ARGS(__VA_ARGS__))

#define LOG_INFO(format, ...)   LOG(Log::INFO, format, __VA_ARGS__)
#define LOG_WARN(format, ...)   LOG(Log::WARN, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...)  LOG(Log::DEBUG, format, __VA_ARGS__)
#define LOG_ERR(format, ...)    LOG(Log::ERR, format, __VA_ARGS__)
#define LOG_CRIT(format, ...)   LOG(Log::CRIT, format, __VA_ARGS__)
#define LOG_FATAL(format, ...)  LOG(Log::FATAL, format, __VA_ARGS__)

#define LOG_SET_LOG_FILE(path)  Log::get_instance().set_log_file(path)

class Log {
    public:
        enum LogLevel {
            INFO, WARN, DEBUG, ERR, CRIT, FATAL, ALL
        };

        constexpr static const char* level_str[] = {
            "INFO", "WARN", "DEBUG", "ERR", "CRIT", "FATAL"
        };

        static const LogLevel global_level = Log::ALL;

        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;

        static Log& get_instance() {
            static Log instance;
            return instance;
        }

        template<typename... Args>
        void printf(LogLevel level, const char* file, int line, const char* func, const char *format, const Args&... args) {
            if (level > global_level)
                return;

            std::lock_guard<std::mutex> lock(_mutex);
            std::ostringstream oss;

#if defined(LOG_SHOW_FILE_LINE)
    #if defined(LOG_SHOW_FILE_FULLPATH)
            const char* _file = file;
    #else
            const char* _file = __builtin_strrchr(file, '/') ? __builtin_strrchr(file, '/') + 1 : \
                                __builtin_strrchr(file, '\\') ? __builtin_strrchr(file, '\\') + 1 : file;
    #endif
            tfm::format(oss, "[%20s:%-6d]", _file, line);
#endif

#if defined(LOG_SHOW_FUNC_NAME)
            tfm::format(oss, "[%-25s]", func);
#endif
            tfm::format(oss, "<%6s>: ", level_str[level]);
            tfm::format(oss, format, args...);

            std::cout << oss.str();
            std::cout.flush();

            if (_logfile.is_open()) {
                _logfile << oss.str();
                _logfile.flush();
            }

            if (level == FATAL)
                throw std::runtime_error("Fatal error");
        }

        void set_log_file(std::string path) {
            if (_logfile.is_open())
                _logfile.close();

            _logfile.open(path, std::ios::out);
        }

    private:
        Log() = default;

        std::ofstream _logfile;
        std::mutex _mutex;
};