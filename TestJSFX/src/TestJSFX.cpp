#include "FIFOBuffer.h"
#include "FIRFilter.h"
#include <gtest/gtest.h>

TEST(FIFOBufferTest, WriteAndRead) {
  using namespace util;
  const size_t capacity = 4;
  FIFOBuffer<int, capacity> buffer;

  // Test writing and reading within bounds
  int writeArray[] = {1, 2, 3, 4};
  // list initialise a span
  buffer.write({writeArray});

  int readData[4];
  int numRead = buffer.read({readData});

  EXPECT_EQ(numRead, 4);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(readData[i], writeArray[i]);
  }

  // Test writing and reading that wraps around
  int writeArrayWrap[] = {5, 6, 7};
  buffer.write({writeArrayWrap});

  int readDataWrap[3];
  numRead = buffer.read({readDataWrap});

  EXPECT_EQ(numRead, 3);
  EXPECT_EQ(readDataWrap[0], 5);
  EXPECT_EQ(readDataWrap[1], 6);
  EXPECT_EQ(readDataWrap[2], 7);
}

TEST(FilterTest, FIR) {
  using namespace dsp::floatingPoint;
  std::vector<float> input = {0.1, 0.2, 0.3, 0.4, 0.5};
  std::vector<float> output(5);

  // Test filter passthrough
  auto passthroughFilter = FIRFilter<float>({1.0});
  passthroughFilter.process(input, output);
  for(int i = 0; i < output.size(); i++){
    EXPECT_FLOAT_EQ(input[i], output[i]);
  }

  // Test simple process
  auto triangularFilter = FIRFilter<float>({0.1, 0.2, 0.3, 0.2, 0.1});
  triangularFilter.process(input, output);
  auto expected = std::vector<float>{0.01, 0.04, 0.10, 0.18, 0.27};
  for(int i = 0; i < output.size(); i++){
    EXPECT_FLOAT_EQ(output[i], expected[i]);
  }

  // Test internal buffer wrap around
  auto miniFilter = FIRFilter<float>({0.1, 0.2, 0.1});
  miniFilter.process(input, output);
  expected = {0.01, 0.04, 0.08, 0.12, 0.16};
  for(int i = 0; i < output.size(); i++){
    EXPECT_FLOAT_EQ(output[i], expected[i]);
  }

  // Test flush
  auto flush_out = std::vector<float>(2);
  expected = std::vector<float>{0.14, 0.05};
  miniFilter.flush(2, flush_out);
  for(int i = 0; i < flush_out.size(); i++){
    EXPECT_FLOAT_EQ(flush_out[i], expected[i]);
  }
  EXPECT_TRUE(miniFilter.empty());

  // Test reset
  miniFilter.reset();
  EXPECT_FALSE(miniFilter.empty());
  miniFilter.process(input, output);
  expected = {0.01, 0.04, 0.08, 0.12, 0.16};
  for(int i = 0; i < output.size(); i++){
    EXPECT_FLOAT_EQ(output[i], expected[i]);
  }
}