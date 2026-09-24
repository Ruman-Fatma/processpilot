#pragma once
#include <string>
#include <mutex>
class Logger {
public:
    explicit Logger(const std::string& path = "logs/processpilot.log");
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);
private:
    void write(const char* level, const std::string& msg);
    std::string path_; std::mutex mutex_;
};
