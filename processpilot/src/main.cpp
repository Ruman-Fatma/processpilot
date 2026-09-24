#include "daemon/daemon.hpp"
#include <iostream>
int main(int argc,char**argv){std::string config="configs";std::string socket="/tmp/processpilot.sock";if(argc>1)config=argv[1];if(argc>2)socket=argv[2];std::cout<<"ProcessPilot daemon\n";return Daemon(config,socket).run();}
