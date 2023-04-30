#include "config.h"
#include "gui/gui.hpp"
#include "logging.hpp"

#include <argparse/argparse.hpp>
#include <fmt/core.h>

#include <iostream>
#include <string>

int
main(int argc, char* argv[])
{
    // Setup logging
    // NOLINTNEXTLINE(readability-identifier-naming)
    const auto LOG = krompir::logging::init();

    // Argument parsing
    argparse::ArgumentParser program(
        "krompir", VERSION, argparse::default_arguments::help
    );

    program.add_argument("-V", "--version")
        .help("prints version information and exits")
        .action([&](const auto& /* unused */) {
            fmt::print("krompir v{}", VERSION);
            std::exit(0); // NOLINT(concurrency-*)
        })
        .default_value(false)
        .implicit_value(true)
        .nargs(0);

    size_t verbosity = 0;
    program.add_argument("-v", "--verbose")
        .help("increase output verbosity")
        .action([&](const auto& /* unused */) { ++verbosity; })
        .append()
        .default_value(false)
        .implicit_value(true)
        .nargs(0);

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    // Update verbosity
    if (verbosity >= 2) // info -> debug -> trace
        spdlog::set_level(spdlog::level::trace);
    else if (verbosity >= 1) // info -> debug
        spdlog::set_level(spdlog::level::debug);

    // Transfer control to GUI
    return krompir::gui::main(argc, argv);
}
