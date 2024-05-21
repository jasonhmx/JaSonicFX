#include "CLI11.hpp"
#include "FIRFilter.h"
#include "pugixml.hpp"
#include "AudioFile.h"

struct FilterCoeffs {
    std::vector<float> a_k;
    std::vector<float> b_k;
};

int main(int argc, char** argv)
{
    CLI::App app{"Command-line app for JaSonicFX"};
    app.require_subcommand(1);

    auto filter_process_cmd = app.add_subcommand("filter_process", "Perform filter process");

    std::string ifpath, ofpath, coeffpath;
    filter_process_cmd->add_option("--input", ifpath, "Path to input file");
    filter_process_cmd->add_option("--output", ofpath, "Path to output file");
    filter_process_cmd->add_option("--coeffs", coeffpath, "Path to filter coefficients");
    CLI11_PARSE(app, argc, argv);

    if (filter_process_cmd->parsed()) {
        using namespace dsp::floatingPoint;

        FilterCoeffs coeffs;

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(coeffpath.c_str());
        if (!result) {
            std::cerr << "Error loading XML file: " << result.description() << std::endl;
            return 1;
        }

        // Accessing attributes
        pugi::xml_node root = doc.child("FilterCoeffs");
        int sample_rate = root.attribute("sample_rate").as_int();
        int num_samples = root.attribute("num_samples").as_int();

        std::cout << "Coeffs sample Rate: " << sample_rate << std::endl;
        std::cout << "Coeffs number of Samples: " << num_samples << std::endl;
        coeffs.b_k.reserve(num_samples);

        // Parsing coefficients from XML
        for (pugi::xml_node coeff : root.child("b_k").children("coeff")) {
            coeffs.b_k.push_back(coeff.text().as_float());
        }
        
        FIRFilter filter(std::move(coeffs.b_k));

        // Load input wav file
        AudioFile<float> inputAudioFile;
        inputAudioFile.load(ifpath);
        auto &inputBuffer = inputAudioFile.samples;

        // Filter input wav file
        AudioFile<float>::AudioBuffer outputBuffer;
        outputBuffer.resize(inputBuffer.size());
        for (auto &i : outputBuffer)
        {
            i.resize(inputBuffer[0].size() + filter.size() - 1, 0);
        }
        for (int i = 0; i < inputBuffer.size(); i++)
        {
            filter.process(inputBuffer[i], outputBuffer[i]);
            filter.flush(filter.size() - 1, {outputBuffer[i].begin() + inputBuffer[0].size(), outputBuffer[i].end()});
        }

        // Write result to output wav file
        AudioFile<float> outputAudioFile;
        if (outputAudioFile.setAudioBuffer(outputBuffer))
        {
            std::cout << "Output buffer set successfully!" << "\n";
        }
        outputAudioFile.setBitDepth(16);
        outputAudioFile.setSampleRate(inputAudioFile.getSampleRate());
        outputAudioFile.save(ofpath);
    }

    return 0;
}