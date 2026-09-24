#pragma once
#include <string>
#include <vector>

struct ServiceConfig {
    std::string name;
    std::string command;
    std::string working_directory;
    std::string restart = "on-failure"; // no, always, on-failure
    int restart_delay = 1;
    int max_restarts = 0; // 0 = unlimited
    std::vector<std::string> depends_on;
};

class ConfigParser {
public:
    static ServiceConfig parseFile(const std::string& path);
};
