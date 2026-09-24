#include <iostream>
#include <chrono>
#include <thread>
int main(){std::cout<<"crash_service started; will exit after 5 seconds\n";std::cout.flush();std::this_thread::sleep_for(std::chrono::seconds(5));std::cerr<<"crash_service simulating crash\n";std::cerr.flush();return 42;}
