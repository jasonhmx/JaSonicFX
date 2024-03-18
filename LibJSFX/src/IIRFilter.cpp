#include "IIRFilter.h"
#include <limits>

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
        for (auto& i : a_k)
        {
            i = i / div;
        }
        for (auto& i : b_k)
        {
            i = i / div;
        }
    }

    // set number of coefficients bounded by the larger one of M or N
    numCoeffs = std::max(a_k.size(), b_k.size());
    numRemainingSamples = numCoeffs - 1;

    // allocate state buffer for w_n
    int halfBufferSize = 1;
    for (int temp = numCoeffs - 1; temp > 0; temp >>=1){
        halfBufferSize <<= 1;
    }
    buffer.resize(halfBufferSize << 1, 0);
    sizeMask = halfBufferSize - 1;
    index = sizeMask;
}

template <std::floating_point FloatType>
void dsp::floatingPoint::IIRFilter<FloatType>::process(std::span<FloatType> input, std::span<FloatType> output) const
{

}

template <std::floating_point FloatType>
void dsp::floatingPoint::IIRFilter<FloatType>::flush(int numSamples, std::span<FloatType> output)
{
}

template <std::floating_point FloatType>
auto dsp::floatingPoint::IIRFilter<FloatType>::empty() const -> bool
{
    return false;
}

template class dsp::floatingPoint::FilterCoeffs<float>;
template class dsp::floatingPoint::FilterCoeffs<double>;
template class dsp::floatingPoint::IIRFilter<float>;
template class dsp::floatingPoint::IIRFilter<double>;
