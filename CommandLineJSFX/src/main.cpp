#include "CLI11.hpp"
#include "FIRFilter.h"
#include "pugixml.hpp"
#include "iostream"
#include "fstream"

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

        std::cout << "Sample Rate: " << sample_rate << std::endl;
        std::cout << "Number of Samples: " << num_samples << std::endl;
        coeffs.b_k.reserve(num_samples);

        // Parsing coefficients from XML
        for (pugi::xml_node coeff : root.child("b_k").children("coeff")) {
            coeffs.b_k.push_back(coeff.text().as_float());
        }
        
        FIRFilter filter(std::move(coeffs.b_k));
        
    }

    return 0;
}