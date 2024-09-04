// Copyright 2024 <Maxime Haselbauer>
#ifndef FIXTURES_H
#define FIXTURES_H
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks.h"

class HandleDeviceFileTest : public ::testing::Test {
 protected:
  OsAbstractionLayer::MockOsAbstractionLayer os_abstraction_layer_{};
};

#endif  // FIXTURES_H
