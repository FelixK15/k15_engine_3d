
#ifndef _K15Engine_TestApplication_TestModule_h_
#define _K15Engine_TestApplication_TestModule_h_

#include "K15_Prerequisites.h"
#include "K15_ApplicationModule.h"

namespace K15_Engine { namespace Test {
  class TestModule : public ApplicationModule
  {
  public:
    TestModule();
    virtual ~TestModule();
  }; // end of TestModule class declaration
}} // end of K15_Engine::Test namespace

#endif //_K15Engine_TestApplication_TestModule_h_