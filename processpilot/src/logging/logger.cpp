#include "logger.hpp"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <iostream>
Logger::Logger(const std::string& path): path_(path) { std::filesystem::path p(path_); if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path()); }
void Logger::write(const char* level, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto now=std::chrono::system_clock::now(); auto tt=std::chrono::system_clock::to_time_t(now);
    std::tm tm{}; localtime_r(&tt,&tm); std::ostringstream os;
    os<<std::put_time(&tm,"%Y-%m-%d %H:%M:%S")<<" ["<<level<<"] "<<msg;
    std::ofstream out(path_,std::ios::app); if(out) out<<os.str()<<'\n'; std::cout<<os.str()<<'\n';
}
void Logger::info(const std::string& m){write("INFO",m);} void Logger::warn(const std::string& m){write("WARN",m);} void Logger::error(const std::string& m){write("ERROR",m);}
