#!/bin/bash

rm ./runner ./runner.cpp

CXXTEST="../thirdparty/cxxtest/";
INCLUDEPATH=".";



../thirdparty/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp MyTestSuite1.h

g++ -o runner -I$CXXTEST -I$INCLUDEPATH  runner.cpp
