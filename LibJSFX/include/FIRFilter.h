#include <concepts>
#include <vector>
#include <span>

template <std::floating_point FloatType>
class FIRFilter
{
public:
    FIRFilter(std::vector<FloatType> coeffs);
    void process(std::span<FloatType> input, std::span<FloatType> output);
    void flush(int numSamples, std::span<FloatType> output);
    bool empty();

private:
    std::vector<FloatType> filterCoeffs;
    std::vector<FloatType> buffer; // power of 2 size, uses double buffer FIR optimisation
    int index;
    int numCoeffs;
    int sizeMask;
    int numRemainingSamples;
};
