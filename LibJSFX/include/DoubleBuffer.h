#include <atomic>
#include <array>
#include <span>
#include <algorithm>
#include <optional>
#include <mutex>

/**
 * @brief A double buffer implementation. One reader and one writer only!
 *
 * @tparam T The type of element the buffer holds
 * @tparam capacity The capacity of each half of the double buffer
 */
template <typename T, size_t halfCapacity>
class DoubleBuffer
{
public:
    DoubleBuffer() = default;
    bool push(const T &element);
    bool read(std::span<T> output);

private:
    std::atomic<int> size{0}; // how many elements are in the buffer currently
    int writePtr{0};          // where the write pointer is
    std::atomic<int> readStart{0};
    std::atomic_flag ready{false};
    std::array<T, halfCapacity << 1> buffer;
};

template <typename T, size_t halfCapacity>
inline bool DoubleBuffer<T, halfCapacity>::push(const T &element)
{
}

template <typename T, size_t halfCapacity>
inline bool DoubleBuffer<T, halfCapacity>::read(std::span<T> output)
{
    if (!ready)
    {
        return false;
    }
    ready = false;
    auto startIt = buffer.begin() + readStart;
    std::copy(startIt, startIt + halfCapacity, output);
}
