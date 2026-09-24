#pragma once
#include "../config/config_parser.hpp"
#include "../logging/logger.hpp"
#include <map>
#include <mutex>
#include <sys/types.h>
#include <vector>

enum class ServiceState { STOPPED, STARTING, RUNNING, FAILED };
struct ServiceInfo { ServiceConfig config; pid_t pid=-1; ServiceState state=ServiceState::STOPPED; int restart_count=0; bool stopping=false; };
class ProcessManager {
public:
    explicit ProcessManager(Logger& logger);
    bool addService(const ServiceConfig& cfg);
    bool start(const std::string& name);
    bool stop(const std::string& name);
    bool restart(const std::string& name);
    void supervise();
    void stopAll();
    std::vector<ServiceInfo> list() const;
    bool has(const std::string& name) const;
private:
    bool spawn(ServiceInfo& svc);
    void reap(ServiceInfo& svc, int status);
    mutable std::mutex mutex_; std::map<std::string,ServiceInfo> services_; Logger& logger_;
};
std::string stateToString(ServiceState state);
