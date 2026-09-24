#include "process_manager.hpp"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <sstream>
#include <filesystem>

std::string stateToString(ServiceState s){ switch(s){case ServiceState::STOPPED:return "STOPPED";case ServiceState::STARTING:return "STARTING";case ServiceState::RUNNING:return "RUNNING";default:return "FAILED";} }
ProcessManager::ProcessManager(Logger& l):logger_(l){}
bool ProcessManager::addService(const ServiceConfig& cfg){std::lock_guard<std::mutex>g(mutex_); if(services_.count(cfg.name)) return false; services_[cfg.name].config=cfg; return true;}
bool ProcessManager::has(const std::string& n) const {std::lock_guard<std::mutex>g(mutex_);return services_.count(n)>0;}

bool ProcessManager::spawn(ServiceInfo& svc){
    svc.state=ServiceState::STARTING; svc.stopping=false;
    pid_t pid=fork();
    if(pid<0){svc.state=ServiceState::FAILED;logger_.error("fork failed for "+svc.config.name+": "+std::strerror(errno));return false;}
    if(pid==0){
        if(!svc.config.working_directory.empty()) chdir(svc.config.working_directory.c_str());
        std::filesystem::create_directories("logs/services");
        std::string out="logs/services/"+svc.config.name+".log";
        int fd=open(out.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
        if(fd>=0){dup2(fd,STDOUT_FILENO);dup2(fd,STDERR_FILENO);close(fd);} 
        execl("/bin/sh","sh","-c",svc.config.command.c_str(),(char*)nullptr); _exit(127);
    }
    svc.pid=pid; svc.state=ServiceState::RUNNING; logger_.info("Started "+svc.config.name+" with PID "+std::to_string(pid)); return true;
}

bool ProcessManager::start(const std::string& name){
    std::lock_guard<std::mutex>g(mutex_); auto it=services_.find(name); if(it==services_.end()) return false; if(it->second.state==ServiceState::RUNNING) return true; return spawn(it->second);
}
bool ProcessManager::stop(const std::string& name){
    std::lock_guard<std::mutex>g(mutex_); auto it=services_.find(name); if(it==services_.end()) return false; auto &s=it->second; if(s.pid>0){s.stopping=true; if(kill(s.pid,SIGTERM)<0 && errno!=ESRCH) return false;} s.state=ServiceState::STOPPED; logger_.info("Stopped "+name); return true;
}
bool ProcessManager::restart(const std::string& name){ if(!stop(name)) return false; std::this_thread::sleep_for(std::chrono::milliseconds(200)); return start(name); }

void ProcessManager::reap(ServiceInfo& s,int status){
    bool unexpected=!s.stopping; pid_t old=s.pid; s.pid=-1; s.stopping=false;
    if(WIFEXITED(status) && WEXITSTATUS(status)==0) {s.state=ServiceState::STOPPED; logger_.info(s.config.name+" exited normally"); return;}
    if(!unexpected) { s.state=ServiceState::STOPPED; logger_.info(s.config.name+" stopped"); return; }
    s.state=ServiceState::FAILED; logger_.warn(s.config.name+" (PID "+std::to_string(old)+") terminated unexpectedly");
    if(unexpected && (s.config.restart=="always" || s.config.restart=="on-failure") && (s.config.max_restarts==0 || s.restart_count<s.config.max_restarts)) {
        ++s.restart_count; int delay=s.config.restart_delay; logger_.info("Restart policy triggered for "+s.config.name+"; restarting in "+std::to_string(delay)+"s");
        std::this_thread::sleep_for(std::chrono::seconds(delay)); spawn(s);
    }
}
void ProcessManager::stopAll(){
    std::lock_guard<std::mutex>g(mutex_);
    for(auto& [name,s]:services_) {
        if(s.pid>0) { s.stopping=true; kill(s.pid,SIGTERM); }
    }
}
void ProcessManager::supervise(){
    std::lock_guard<std::mutex>g(mutex_);
    for(auto& [name,s]:services_) if(s.pid>0){int status=0; pid_t r=waitpid(s.pid,&status,WNOHANG); if(r==s.pid) reap(s,status);}
}
std::vector<ServiceInfo> ProcessManager::list() const {std::lock_guard<std::mutex>g(mutex_);std::vector<ServiceInfo>v;for(auto&[n,s]:services_)v.push_back(s);return v;}
