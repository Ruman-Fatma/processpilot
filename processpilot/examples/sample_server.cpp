#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>
std::atomic<bool> running{true}; void h(int){running=false;}
int main(){signal(SIGTERM,h);signal(SIGINT,h);std::cout<<"sample_server started\n";while(running){std::cout<<"sample_server heartbeat\n";std::cout.flush();std::this_thread::sleep_for(std::chrono::seconds(2));}return 0;}
