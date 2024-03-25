#include <concepts>
#include <fstream>
#include <string>
#include <span>
#include <exception>

namespace framework
{
    template <typename T>
    concept arithmetic = std::integral<T> || std::floating_point<T>;

    template <arithmetic NumberType>
    class FileReader
    {
    private:
        // TODO: better int types?
        int fs;
        int bitDepth;
        int blockSizeMs;
        int blockSizeSamples;
        int numChannels;
        std::ifstream stream;
        const std::string filename;

    public:
        /**
         * @brief Construct a new File Reader object. Opens and closes fstream with RAII.
         *
         * @param name name of input file.
         */
        explicit FileReader(std::string name);

        /**
         * @brief Destroy the File Reader object. Destroys the ifstream and hence closes the file.
         *
         */
        ~FileReader() = default;

        FileReader(const FileReader &other) = delete;
        auto operator=(const FileReader &other) = delete;
        FileReader(FileReader &&other) = delete;
        auto operator=(FileReader &&other) = delete;

        /**
         * @brief Read in a block of audio
         * 
         * @param output 
         */
        void readBlock(std::span<NumberType> output);

        /**
         * @brief Read in specified number of audio samples
         * 
         * @param output 
         * @param numSamples 
         */
        void readSamples(std::span<NumberType> output, int numSamples);

        int getSamplesPerSec() const { return fs; }
        int getBitsPerSample() const { return bitDepth; }
        int getNumChannels() const { return numChannels; }
    };

    template <arithmetic T>
    class FileWriter
    {
    private:
        /* data */
    public:
        FileWriter(/* args */) {}
        ~FileWriter() {}
    };

} // namespace framework
