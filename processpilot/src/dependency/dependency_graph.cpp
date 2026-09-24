#include "dependency_graph.hpp"
#include <stdexcept>
#include <functional>
void DependencyGraph::add(const std::string&n,const std::vector<std::string>&d){graph_[n]=d;}
bool DependencyGraph::dfs(const std::string&n,std::set<std::string>&vis,std::set<std::string>&stack)const{if(stack.count(n))return true;if(vis.count(n))return false;vis.insert(n);stack.insert(n);auto it=graph_.find(n);if(it!=graph_.end())for(auto&d:it->second)if(dfs(d,vis,stack))return true;stack.erase(n);return false;}
bool DependencyGraph::hasCycle()const{std::set<std::string>v,s;for(auto&[n,_]:graph_)if(dfs(n,v,s))return true;return false;}
std::vector<std::string> DependencyGraph::startupOrder(const std::string&root)const{std::vector<std::string>o;std::set<std::string>vis;std::function<void(const std::string&)>go=[&](const std::string&n){if(vis.count(n))return;vis.insert(n);auto it=graph_.find(n);if(it!=graph_.end())for(auto&d:it->second)go(d);o.push_back(n);};go(root);return o;}
