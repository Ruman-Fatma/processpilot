#pragma once
#include "../process/process_manager.hpp"
#include "../monitor/resource_monitor.hpp"
#include "../dependency/dependency_graph.hpp"
#include "../ipc/unix_socket.hpp"
#include "../logging/logger.hpp"
#include <atomic>
#include <string>
class Daemon { public: Daemon(const std::string&configDir,const std::string&socketPath); int run(); void stop(); private: void loadConfigs(); std::string handle(const std::string&request); std::string configDir_,socketPath_; std::atomic<bool>running_{true}; Logger logger_; ProcessManager pm_; ResourceMonitor monitor_; DependencyGraph graph_; };
