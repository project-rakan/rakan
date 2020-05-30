#ifndef TEST_SUITE_H_
#define TEST_SUITE_H_

#ifndef TEST_MODE
#define TEST_MODE
#define SEED 100
#endif

#include <iostream>

#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
  testing::InitGoogleTest();
  std::cout << "Running Rakan test_suite:" << std::endl;
  return RUN_ALL_TESTS();
}

#endif    // TEST_SUITE_H