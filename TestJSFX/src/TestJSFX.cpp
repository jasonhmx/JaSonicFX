#include "FIFOBuffer.h"
#include <gtest/gtest.h>

TEST(FIFOBufferTest, WriteAndRead) {
  using namespace Util;
  const size_t capacity = 4;
  FIFOBuffer<int, capacity> buffer;

  // Test writing and reading within bounds
  int writeArray[] = {1, 2, 3, 4};
  buffer.write(writeArray, 4);

  int readData[4];
  int numRead = buffer.read(readData, 4);

  EXPECT_EQ(numRead, 4);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(readData[i], writeArray[i]);
  }

  // Test writing and reading that wraps around
  int writeArrayWrap[] = {5, 6, 7};
  buffer.write(writeArrayWrap, 3);

  int readDataWrap[3];
  numRead = buffer.read(readDataWrap, 3);

  EXPECT_EQ(numRead, 3);
  EXPECT_EQ(readDataWrap[0], 5);
  EXPECT_EQ(readDataWrap[1], 6);
  EXPECT_EQ(readDataWrap[2], 7);
}