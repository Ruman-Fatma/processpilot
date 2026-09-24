#pragma once
#include <string>
class UnixSocketServer { public: explicit UnixSocketServer(const std::string& path); ~UnixSocketServer(); bool bindAndListen(); int acceptClient(); std::string receive(int fd); bool send(int fd,const std::string& data); private: std::string path_; int fd_=-1; };
class UnixSocketClient { public: explicit UnixSocketClient(const std::string& path); ~UnixSocketClient(); bool connectToServer(); std::string request(const std::string& req); private: std::string path_; int fd_=-1; };
