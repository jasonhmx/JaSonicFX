#include <FIRFilter.h>

template <std::floating_point FloatType>
FIRFilter<FloatType>::FIRFilter(std::vector<FloatType> coeffs)
{
    filterCoeffs = std::move(coeffs);
    int numCoeffs = filterCoeffs.size();
    int bufferSize = 1;
    while(numCoeffs > 0){
        numCoeffs>>;
        bufferSize<<;
    }
    buffer.resize(bufferSize, 0);
    sizeMask = bufferSize / 2 - 1;
    readIdx = bufferSize;
}

template <std::floating_point FloatType>
void FIRFilter<FloatType>::process(std::span<FloatType> input, std::span<FloatType> output)
{
}

template class FIRFilter<float>;
template class FIRFilter<double>;