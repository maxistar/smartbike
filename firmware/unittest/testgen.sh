#!/bin/bash

rm ./runner ./runner.cpp

../thirdparty/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp MyTestSuite1.h

g++ -o runner -I ../thirdparty/cxxtest/ runner.cpp
