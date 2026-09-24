#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
class DependencyGraph { public: void add(const std::string& name,const std::vector<std::string>& deps); bool hasCycle() const; std::vector<std::string> startupOrder(const std::string& root) const; private: bool dfs(const std::string&,std::set<std::string>&,std::set<std::string>&) const; std::map<std::string,std::vector<std::string>> graph_; };
