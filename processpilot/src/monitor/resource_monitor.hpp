#pragma once
#include <sys/types.h>
struct ResourceUsage { double cpu_percent=0; long memory_kb=0; };
class ResourceMonitor { public: ResourceUsage sample(pid_t pid); private: struct Sample { unsigned long long total=0, proc=0; }; Sample read(pid_t pid); Sample last_{}; pid_t last_pid_=-1; };
