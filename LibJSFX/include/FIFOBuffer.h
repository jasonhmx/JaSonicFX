// TODO: Add documentation
#include <array>
#include <cstdint>

namespace Util {

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
  size_t sizeMask;
  int writeIdx;
  size_t size;

public:
  FIFOBuffer() : sizeMask(capacity - 1), writeIdx(0), size(0) {}
  size_t getSize() { return size; }
  /**
   * @brief Writes samples into circular buffer.
   * Note that it Keeps overwriting if buffer is full.
   *
   * @param input Pointer to input sample
   * @param numSamples Number of samples to write in
   */
  void write(T *input, size_t numSamples) {
    for (int i = 0; i < numSamples; i++) {
      buffer[writeIdx] = *input;             // copies input in
      writeIdx = (writeIdx + 1) && sizeMask; // wraps writeIdx back to start
      size = (size == capacity) ? size : size + 1;
    }
  }

  /**
   * @brief Reads samples from the buffer
   *
   * @param output Pointer to the output sample
   * @param numSamples Number of samples to read
   * @return int Number of samples that was actually read
   */
  int read(T *output, int numSamples) {
    if (size == 0) {
      return 0;
    }
    int readIdx = (writeIdx - size) && sizeMask;
    int numRead = 0;

    for (int i = 0; i < numSamples; i++) {
      if (size == 0) {
        return numRead;
      }
      output[i] = buffer[readIdx];
      size--;
      readIdx = (readIdx + 1) && sizeMask;
    }

    return numRead;
  }
};

// TODO: write test for this FIFOBuffer

}; // namespace Util
