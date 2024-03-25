#include "FileIO.h"

using namespace framework;

template <arithmetic T>
FileReader<T>::FileReader(std::string name)
{
    // construct the input file stream, throw if not opened
    // read in metadata from txt file, populate members
    // check dimensions, throw if doesn't match
}

template <arithmetic NumberType>
void framework::FileReader<NumberType>::readBlock(std::span<NumberType> output)
{
}

template <arithmetic NumberType>
void framework::FileReader<NumberType>::readSamples(std::span<NumberType> output, int numSamples)
{
}

template class FileReader<int>;
template class FileReader<float>;
template class FileReader<double>;