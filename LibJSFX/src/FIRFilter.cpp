#include <FIRFilter.h>
#include <numeric>
#include <algorithm>

template <std::floating_point FloatType>
FIRFilter<FloatType>::FIRFilter(std::vector<FloatType> coeffs)
{
    filterCoeffs = std::move(coeffs);
    numCoeffs = filterCoeffs.size();
    numRemainingSamples = numCoeffs - 1;
    int halfBufferSize = 1;
    while (numCoeffs > 0)
    {
        numCoeffs >>= 1;
        halfBufferSize <<= 1;
    }
    buffer.resize(halfBufferSize << 1, 0);
    sizeMask = halfBufferSize - 1;
    index = sizeMask;
}

template <std::floating_point FloatType>
void FIRFilter<FloatType>::process(std::span<FloatType> input, std::span<FloatType> output)
{
    for (int i = 0; i < input.size(); i++)
    {
        // write sample in the buffer
        buffer[index] = input[i];
        buffer[index + buffer.size() / 2] = input[i];
        // multiply accumulate the result
        output[i] = std::inner_product(filterCoeffs.begin(), filterCoeffs.end(), buffer.begin() + index, 0.0);
        // update index
        index = (index - 1) & sizeMask;
    }
}

template <std::floating_point FloatType>
void FIRFilter<FloatType>::flush(int numSamples, std::span<FloatType> output)
{
    if (empty())
        return;
    for(int i = 0; i < numSamples && numRemainingSamples > 0; i++){
        // write 0 in the buffer to flush out remaining samples
        buffer[index] = 0;
        buffer[index + buffer.size() / 2] = 0;
        // multiply accumulate the result
        output[i] = std::inner_product(filterCoeffs.begin(), filterCoeffs.end(), buffer.begin() + index, 0.0);
        // update index
        index = (index - 1) & sizeMask;
        numRemainingSamples--;
    }
}

template <std::floating_point FloatType>
bool FIRFilter<FloatType>::empty()
{
    return numRemainingSamples == 0;
}

template <std::floating_point FloatType>
void FIRFilter<FloatType>::reset()
{
    std::ranges::fill(buffer, 0.0);
    index = sizeMask;
    numRemainingSamples = numCoeffs - 1;
}

template class FIRFilter<float>;
template class FIRFilter<double>;