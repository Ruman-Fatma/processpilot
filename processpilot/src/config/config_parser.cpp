#include "config_parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace {
std::string trim(std::string s) {
    auto notspace = [](unsigned char c){ return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notspace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
    return s;
}
std::vector<std::string> splitComma(const std::string& s) {
    std::vector<std::string> out;
    std::stringstream ss(s); std::string item;
    while (std::getline(ss, item, ',')) { item = trim(item); if (!item.empty()) out.push_back(item); }
    return out;
}
}

ServiceConfig ConfigParser::parseFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Cannot open config: " + path);
    ServiceConfig cfg;
    std::string section, line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (line.front() == '[' && line.back() == ']') { section = line.substr(1, line.size()-2); continue; }
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string key = trim(line.substr(0,pos));
        std::string val = trim(line.substr(pos+1));
        if (key == "name") cfg.name = val;
        else if (key == "command") cfg.command = val;
        else if (key == "working_directory") cfg.working_directory = val;
        else if (key == "restart") cfg.restart = val;
        else if (key == "restart_delay") cfg.restart_delay = std::stoi(val);
        else if (key == "max_restarts") cfg.max_restarts = std::stoi(val);
        else if (key == "depends_on") cfg.depends_on = splitComma(val);
    }
    if (cfg.name.empty()) throw std::runtime_error("Missing name in " + path);
    if (cfg.command.empty()) throw std::runtime_error("Missing command in " + path);
    return cfg;
}
