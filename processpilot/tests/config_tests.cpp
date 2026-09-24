#include "../src/config/config_parser.hpp"
#include <cassert>
#include <fstream>
int main(){std::ofstream f("/tmp/pp_test.service");f<<"[service]\nname=x\ncommand=echo hi\nrestart=always\nrestart_delay=3\ndepends_on=a,b\n";f.close();auto c=ConfigParser::parseFile("/tmp/pp_test.service");assert(c.name=="x");assert(c.command=="echo hi");assert(c.restart=="always");assert(c.depends_on.size()==2);return 0;}
