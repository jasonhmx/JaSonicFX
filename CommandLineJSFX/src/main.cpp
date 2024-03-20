#include "CLI11.hpp"
#include "FIRFilter.h"

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
    return 0;
}