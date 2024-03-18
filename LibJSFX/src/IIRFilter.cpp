#include "IIRFilter.h"
#include <limits>
#include <algorithm>
#include <numeric>

using namespace dsp::floatingPoint;

template <std::floating_point FloatType>
IIRFilter<FloatType>::IIRFilter(FilterCoeffs<FloatType> coeffs)
{
    // TODO: add error handling

    // set coefficients and normalise if needed
    a_k = std::move(coeffs.a_k);
    b_k = std::move(coeffs.b_k);
    if (std::abs(a_k[0] - 1.0) > std::numeric_limits<FloatType>::epsilon())
    {
        FloatType div = a_k[0];
        for (auto &i : a_k)
        {
            i = i / div;
        }
        for (auto &i : b_k)
        {
            i = i / div;
        }
    }

    // set number of coefficients bounded by the larger one of M or N
    numCoeffs = std::max(a_k.size(), b_k.size());
    numRemainingSamples = numCoeffs - 1;

    // allocate state buffer for w_n
    int halfBufferSize = 1;
    for (int temp = numCoeffs - 1; temp > 0; temp >>= 1)
    {
        halfBufferSize <<= 1;
    }
    buffer.resize(halfBufferSize << 1, 0);
    sizeMask = halfBufferSize - 1;
    index = sizeMask;
}

template <std::floating_point FloatType>
void dsp::floatingPoint::IIRFilter<FloatType>::process(const std::span<FloatType> input, std::span<FloatType> output)
{
    if (empty()){
        numRemainingSamples = numCoeffs - 1;
    }
    for (int i = 0; i < input.size(); i++)
    {
        process(input[i], output[i]);
    }
}

template <std::floating_point FloatType>
void dsp::floatingPoint::IIRFilter<FloatType>::process(const FloatType& input, FloatType &output)
{
    // w[n] = x[n] - \sum_{k=1}^{N} a[k]w[n-k]
    buffer[index] = input - std::inner_product(a_k.begin() + 1, a_k.end(), buffer.begin() + index + 1, 0.0);
    buffer[index + buffer.size() / 2] = buffer[index];
    // y[n] = \sum{k=0}^M b[k]w[n-k]
    output = std::inner_product(b_k.begin(), b_k.end(), buffer.begin() + index, 0.0);
    // update index
    index = (index - 1) & sizeMask;
}

template <std::floating_point FloatType>
void dsp::floatingPoint::IIRFilter<FloatType>::flush(int numSamples, std::span<FloatType> output)
{
    if (empty()){
        return;
    }
    for (int i = 0; i < numSamples && numRemainingSamples > 0; i++)
    {
        // write 0 to flush out remaining values
        process(0.0, output[i]);
        numRemainingSamples--;
    }
}

template <std::floating_point FloatType>
auto dsp::floatingPoint::IIRFilter<FloatType>::empty() const -> bool
{
    return numRemainingSamples == 0;
}

template class dsp::floatingPoint::FilterCoeffs<float>;
template class dsp::floatingPoint::FilterCoeffs<double>;
template class dsp::floatingPoint::IIRFilter<float>;
template class dsp::floatingPoint::IIRFilter<double>;
