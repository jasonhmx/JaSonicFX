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
  static_assert((capacity > 0) && (capacity && (capacity - 1)) == 0,
                "Buffer size needs to be a power of 2");

private:
  std::array<T, capacity> buffer;
  size_t sizemask;
  size_t pos;

public:
  FIFOBuffer() : sizemask(capacity - 1), pos(sizemask) {}
  size_t size() { return capacity; }
  void write(T *input, size_t numSamples) {}
  void read(T *output, size_t numsamples) {}
};

}; // namespace Util
