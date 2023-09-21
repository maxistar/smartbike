#!/bin/bash

rm ./runner ./runner.cpp

CXXTEST="../thirdparty/cxxtest/";
INCLUDEPATH="../sim7000/sim7000/";



../thirdparty/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp MyTestSuite1.h

g++ -o runner -I$CXXTEST -I$INCLUDEPATH ../sim7000/sim7000/SleepTimer.cpp  runner.cpp
