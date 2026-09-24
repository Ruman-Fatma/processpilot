#include "../src/ipc/unix_socket.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
static void usage(){std::cout<<"Usage: processpilot-cli [--socket PATH] <start|stop|restart|status|list|ping> [service]\n";}
int main(int argc,char**argv){std::string sock="/tmp/processpilot.sock";int i=1;if(i+1<argc&&std::string(argv[i])=="--socket"){sock=argv[i+1];i+=2;}if(i>=argc){usage();return 1;}std::string cmd=argv[i++];std::string req;if(cmd=="start"||cmd=="stop"||cmd=="restart"){if(i>=argc){usage();return 1;}req=std::string(cmd.begin(),cmd.end());for(char&c:req)c=std::toupper(c);req+=" "+std::string(argv[i]);}else if(cmd=="status"||cmd=="list")req="STATUS";else if(cmd=="ping")req="PING";else {usage();return 1;}UnixSocketClient c(sock);auto r=c.request(req);if(r.empty()){std::cerr<<"Cannot connect to ProcessPilot daemon at "<<sock<<"\n";return 2;}if(cmd=="status"||cmd=="list"){std::cout<<std::left<<std::setw(18)<<"SERVICE"<<std::setw(10)<<"PID"<<std::setw(12)<<"STATE"<<std::setw(12)<<"CPU%"<<std::setw(12)<<"MEM(KB)"<<"RESTARTS\n";std::cout<<std::string(76,'-')<<"\n";std::istringstream ss(r);std::string line;while(std::getline(ss,line)){std::stringstream ls(line);std::string f;std::vector<std::string>v;while(std::getline(ls,f,'|'))v.push_back(f);if(v.size()>=6)std::cout<<std::left<<std::setw(18)<<v[0]<<std::setw(10)<<v[1]<<std::setw(12)<<v[2]<<std::setw(12)<<v[3]<<std::setw(12)<<v[4]<<v[5]<<"\n";}}else std::cout<<r;return 0;}
