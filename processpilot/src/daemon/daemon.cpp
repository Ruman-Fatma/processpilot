#include "daemon.hpp"
#include "../config/config_parser.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <signal.h>
namespace
{
    Daemon *g = nullptr;
    void handler(int)
    {
        if (g)
            g->stop();
    }
}
Daemon::Daemon(const std::string &c, const std::string &s) : configDir_(c), socketPath_(s), logger_("logs/processpilot.log"), pm_(logger_) { g = this; }
void Daemon::stop() { running_ = false; }
void Daemon::loadConfigs()
{
    for (auto &e : std::filesystem::directory_iterator(configDir_))
    {
        if (e.path().extension() == ".service")
        {
            try
            {
                auto c = ConfigParser::parseFile(e.path().string());
                pm_.addService(c);
                graph_.add(c.name, c.depends_on);
            }
            catch (const std::exception &ex)
            {
                logger_.error(ex.what());
            }
        }
    }
    if (graph_.hasCycle())
        logger_.error("Dependency cycle detected in configuration");
}
std::string Daemon::handle(const std::string &r)
{
    std::istringstream ss(r);
    std::string cmd, arg;
    ss >> cmd >> arg;
    if (cmd == "PING")
        return "PONG\n";
    if (cmd == "START")
    {
        if (!pm_.has(arg))
            return "ERROR unknown service\n";
        auto order = graph_.startupOrder(arg);
        if (graph_.hasCycle())
            return "ERROR dependency cycle\n";
        for (auto &n : order)
            if (!pm_.start(n))
                return "ERROR failed to start " + n + "\n";
        return "OK started " + arg + "\n";
    }
    if (cmd == "STOP")
        return pm_.stop(arg) ? "OK stopped " + arg + "\n" : "ERROR unknown service\n";
    if (cmd == "RESTART")
        return pm_.restart(arg) ? "OK restarted " + arg + "\n" : "ERROR restart failed\n";
    if (cmd == "STATUS" || cmd == "LIST")
    {
        std::ostringstream out;
        for (auto &s : pm_.list())
        {
            auto u = monitor_.sample(s.pid);
            out << s.config.name << "|" << s.pid << "|" << stateToString(s.state) << "|" << u.cpu_percent << "|" << u.memory_kb << "|" << s.restart_count << "\n";
        }
        return out.str();
    }
    if (cmd == "STOPDAEMON")
    {
        stop();
        return "OK\n";
    }
    return "ERROR unknown command\n";
}
int Daemon::run()
{
    loadConfigs();
    UnixSocketServer server(socketPath_);
    if (!server.bindAndListen())
    {
        logger_.error("Cannot bind socket " + socketPath_);
        return 1;
    }
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    logger_.info("ProcessPilot daemon started");
    while (running_)
    {
        int c = server.acceptClient();
        if (c >= 0)
        {
            auto r = server.receive(c);
            auto ans = handle(r);
            server.send(c, ans);
            close(c);
        }
        pm_.supervise();
    }
    pm_.stopAll();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pm_.supervise();
    logger_.info("ProcessPilot daemon stopped");
    return 0;
}
