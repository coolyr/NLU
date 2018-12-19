#include <string>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "log.h"

#include "test/test.h"

// using namespace std;
using namespace CppUnit;

int32_t main(int32_t argc, char **argv) {
  LOG_CONFIG(DOTEST_LOGGER_CONF);
  bool ok = false;
  {
    TextUi::TestRunner runner;
    runner.setOutputter(new CompilerOutputter(&runner.result(), std::cerr));
    TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    ok = runner.run("", false);
  }
  LOG_SHUTDOWN();
  return ok ? 0 : 1;
}
