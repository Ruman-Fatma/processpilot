#include "unix_socket.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <filesystem>
UnixSocketServer::UnixSocketServer(const std::string&p):path_(p){} UnixSocketServer::~UnixSocketServer(){if(fd_>=0)close(fd_);unlink(path_.c_str());}
bool UnixSocketServer::bindAndListen(){fd_=socket(AF_UNIX,SOCK_STREAM,0);if(fd_<0)return false;unlink(path_.c_str());sockaddr_un a{};a.sun_family=AF_UNIX;std::snprintf(a.sun_path,sizeof(a.sun_path),"%s",path_.c_str());if(bind(fd_,(sockaddr*)&a,sizeof(a))<0)return false;chmod(path_.c_str(),0660);return listen(fd_,10)==0;}
int UnixSocketServer::acceptClient(){fd_set set; FD_ZERO(&set); FD_SET(fd_, &set); timeval tv{1,0}; int r=select(fd_+1,&set,nullptr,nullptr,&tv); if(r<=0) return -1; return accept(fd_,nullptr,nullptr);} std::string UnixSocketServer::receive(int fd){char b[4096];ssize_t n=read(fd,b,sizeof(b)-1);if(n<=0)return {};b[n]=0;return b;}bool UnixSocketServer::send(int fd,const std::string&d){return ::send(fd,d.data(),d.size(),0)>=0;}
UnixSocketClient::UnixSocketClient(const std::string&p):path_(p){} UnixSocketClient::~UnixSocketClient(){if(fd_>=0)close(fd_);}bool UnixSocketClient::connectToServer(){fd_=socket(AF_UNIX,SOCK_STREAM,0);if(fd_<0)return false;sockaddr_un a{};a.sun_family=AF_UNIX;std::snprintf(a.sun_path,sizeof(a.sun_path),"%s",path_.c_str());return connect(fd_,(sockaddr*)&a,sizeof(a))==0;}std::string UnixSocketClient::request(const std::string&r){if(!connectToServer())return {};send(fd_,r.data(),r.size(),0);char b[8192];ssize_t n=read(fd_,b,sizeof(b)-1);if(n<=0)return {};b[n]=0;return b;}
