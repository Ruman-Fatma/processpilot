#include "resource_monitor.hpp"
#include <fstream>
#include <sstream>
#include <unistd.h>
ResourceMonitor::Sample ResourceMonitor::read(pid_t pid){
    Sample s; std::ifstream stat("/proc/"+std::to_string(pid)+"/stat"); if(!stat)return s; std::string line;std::getline(stat,line);auto rp=line.rfind(')'); if(rp==std::string::npos)return s;std::istringstream ss(line.substr(rp+2));char state; ss>>state; unsigned long long x; for(int i=0;i<10;i++) ss>>x; unsigned long long utime=0,stime=0; ss>>utime>>stime; s.proc=utime+stime;
    std::ifstream cpu("/proc/stat");std::string name;cpu>>name; if(name=="cpu"){for(int i=0;i<8;i++){cpu>>x;s.total+=x;}}
    return s;
}
ResourceUsage ResourceMonitor::sample(pid_t pid){
    ResourceUsage u; if(pid<=0)return u; auto cur=read(pid); if(last_pid_==pid && cur.total>last_.total){double dt=double(cur.total-last_.total);double dp=double(cur.proc-last_.proc);u.cpu_percent=100.0*dp/dt;}
    last_=cur;last_pid_=pid; std::ifstream st("/proc/"+std::to_string(pid)+"/status");std::string k;while(st>>k){if(k=="VmRSS:"){st>>u.memory_kb;break;}std::string dummy;std::getline(st,dummy);}return u;
}
