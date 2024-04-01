#include <atomic>
#include <array>
#include <optional>
/**
 * @brief A double buffer implementation
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
    std::optional<T> pop();

private:
    std::atomic<int> size{0}; // how many elements are in the buffer currently
    int writePtr{0};              // where the write pointer is
    int readPtr{halfCapacity};
    std::atomic<int> readStart{0};
    std::atomic_flag writeDone{false};
    std::atomic_flag readDone{false};
    std::array<T, halfCapacity << 1> buffer;
};

template <typename T, size_t halfCapacity>
inline bool DoubleBuffer<T, halfCapacity>::push(const T &element)
{
    if (readDone){
        return false;
    }
    buffer[writePtr] = element;
    size++;
    writePtr++;
    if (writePtr == halfCapacity)
    {
        ready.test_and_set();
        readStart = 0;
    }
    if (writePtr == halfCapacity * 2)
    {
        ready.test_and_set();
        writePtr = 0;
        readStart = halfCapacity;
    }
    return true;
}

template <typename T, size_t halfCapacity>
inline std::optional<T> DoubleBuffer<T, halfCapacity>::pop()
{
    if (!ready)
    {
        return std::optional<T>();
    }
    

}
