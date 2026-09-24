#include "../src/process/process_manager.hpp"
#include <cassert>
int main(){Logger l("/tmp/processpilot-test.log");ProcessManager p(l);ServiceConfig c;c.name="test";c.command="sleep 1";c.restart="no";assert(p.addService(c));assert(p.has("test"));assert(p.start("test"));p.supervise();assert(p.stop("test"));return 0;}
