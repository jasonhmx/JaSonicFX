// TODO: Add documentation
#include <array>
#include <span>

namespace util {

// TODO: write a dynamic buffer on the heap with resize()

/**
 * @brief A stack-allocated audio buffer
 *
 * @tparam T : number format e.g. float, int32_t
 * @tparam capacity: the capacity of the buffer, needs to be a power of 2
 */
template <typename T, size_t capacity> class FIFOBuffer {
  static_assert((capacity > 0) && ((capacity & (capacity - 1)) == 0),
                "Buffer capacity needs to be a power of 2");

private:
  std::array<T, capacity> buffer;
  static constexpr size_t sizeMask = capacity - 1;
  int writeIdx = 0;
  size_t size = 0;

public:
  /**
   * @brief Construct a new FIFOBuffer object
   * 
   */
  FIFOBuffer() = default;
  auto getSize() { return size; }
  /**
   * @brief Writes samples into circular buffer.
   * Note that it Keeps overwriting if buffer is full.
   *
   * @param input std::span object to input location
   */
  void write(std::span<T> input) {
    for (int i = 0; i < input.size(); i++) {
      buffer[writeIdx] = input[i];             // copies input in
      writeIdx = (writeIdx + 1) & sizeMask; // wraps writeIdx back to start
      size = (size == capacity) ? size : size + 1;
    }
  }

  /**
   * @brief Reads samples from the buffer
   *
   * @param output std::span object to output location
   * @return int Number of samples that was actually read
   */
  auto read(std::span<T> output) {
    if (size == 0) {
      return 0;
    }
    int readIdx = (writeIdx - (int)size) & sizeMask; // prevent writeIdx from being casted to size_t which is unsigned
    int numRead = 0;

    for (int i = 0; i < output.size() && size != 0; i++) {
      output[i] = buffer[readIdx];
      size--;
      numRead++;
      readIdx = (readIdx + 1) & sizeMask;
    }

    return numRead;
  }
};

// TODO: write test for this FIFOBuffer

}; // namespace Util
