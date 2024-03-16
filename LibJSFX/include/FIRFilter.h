#include <concepts>
#include <vector>
#include <span>

/**
 * @brief An FIR filter implementation for floating point arithmetic
 * 
 * @tparam FloatType 
 */
template <std::floating_point FloatType>
class FIRFilter
{
public:
    /**
     * @brief Construct a new FIRFilter object
     * 
     * @param coeffs The coefficients/impulse response of the FIR filter
     */
    FIRFilter(std::vector<FloatType> coeffs);

    /**
     * @brief Processes input samples into the output
     * 
     * @param input A std::span of input samples
     * @param output A std::span of output destination
     */
    void process(std::span<FloatType> input, std::span<FloatType> output);

    /**
     * @brief Flushes the FIR tail, the (N-1) samples where N is filter length
     * 
     * @param numSamples Number of samples requested to flush
     * @param output Output destination
     */
    void flush(int numSamples, std::span<FloatType> output);

    /**
     * @brief Checks if there are still samples to be flushed
     * 
     * @return true No samples are left to be flushed
     * @return false Still samples to be flushed
     */
    bool empty();

    /**
     * @brief Resets the filter states and internal buffer
     * 
     */
    void reset();

private:
    std::vector<FloatType> filterCoeffs;
    std::vector<FloatType> buffer; // power of 2 size, uses double buffer FIR optimisation
    int index;
    int numCoeffs;
    int sizeMask;
    int numRemainingSamples;
};
