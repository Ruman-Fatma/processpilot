#include "../src/dependency/dependency_graph.hpp"
#include <cassert>
int main(){DependencyGraph g;g.add("a",{});g.add("b",{"a"});g.add("c",{"b"});auto o=g.startupOrder("c");assert(o.size()==3&&o[0]=="a"&&o[1]=="b"&&o[2]=="c");DependencyGraph x;x.add("a",{"b"});x.add("b",{"a"});assert(x.hasCycle());return 0;}
