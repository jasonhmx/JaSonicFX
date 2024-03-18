#include <concepts>
#include <vector>
#include <span>

namespace dsp::floatingPoint {

    /**
     * @brief A struct containing filter coefficients
     * 
     * @tparam FloatType
     */
    template <std::floating_point FloatType>
    struct FilterCoeffs {
        /**
         * @brief Denominator coefficients
         * 
         */
        std::vector<FloatType> a_k;

        /**
         * @brief Numerator coefficients
         * 
         */
        std::vector<FloatType> b_k;
    };

    /**
     * @brief An IIR filter implementation using the canonical form
     * 
     * @tparam FloatType 
     */
    template <std::floating_point FloatType>
    class IIRFilter{
    public:
        IIRFilter(FilterCoeffs<FloatType> coeffs);

        void process(std::span<FloatType> input, std::span<FloatType> output) const;

        void flush(int numSamples, std::span<FloatType> output);

        auto empty() const -> bool;

    private:
        std::vector<FloatType> a_k;
        std::vector<FloatType> b_k;
        std::vector<FloatType> buffer;
        int index;
        int numCoeffs;
        int sizeMask;
        int numRemainingSamples;
    };

}