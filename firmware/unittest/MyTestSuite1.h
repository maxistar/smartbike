// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include <cxxtest/TestSuite.h>

// # include "SleepTimer.h"


long long int GetSleepDuration(float voltage, float insulation) {
  return 0;
}

class MyTestSuite1 : public CxxTest::TestSuite
{
public:
    void testAddition(void)
    {
        TS_ASSERT(1 + 1 > 1);
        TS_ASSERT_EQUALS(1 + 1, 2);
    }

    void testSleepTimer(void)
    {
        long long result = GetSleepDuration(0.0, 0.0);
        TS_ASSERT_EQUALS(result, 0.0);
    }
};
