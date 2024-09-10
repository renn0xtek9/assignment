// Copyright 2024 <Maxime Haselbauer>
#ifndef FIXTURES_H
#define FIXTURES_H
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <os_abstraction_layer/mocks.h>

/*! \brief Test fixture for tests of FakeImu library*/
class FakeImuTest : public ::testing::Test {
 protected:
  OsAbstractionLayer::MockOsAbstractionLayer
      os_abstraction_layer_{}; /**< Operating System abstraction layer interface.*/
};

#endif  // FIXTURES_H
