#include "common.hpp"
#include "gui/gui.hpp"

#include <argparse/argparse.hpp>
#include <binlog/default_session.hpp>

#include <iostream>

namespace {

struct arguments_t {
    size_t verbosity;
};

arguments_t
parse_arguments(int argc, char* argv[]) // NOLINT(*-avoid-c-arrays)
{
    arguments_t args{};
    argparse::ArgumentParser program(
        "krompir", KROMPIR_VERSION, argparse::default_arguments::help
    );

    // Add arguments
    program.add_argument("-V", "--version")
        .help("prints version information and exits")
        .action([&](const auto& /* unused */) {
            fmt::print("krompir v{}", KROMPIR_VERSION);
            std::exit(0); // NOLINT(concurrency-*)
        })
        .default_value(false)
        .implicit_value(true)
        .nargs(0);

    program.add_argument("-v", "--verbose")
        .help("increase output verbosity")
        .action([&](const auto& /* unused */) { ++args.verbosity; })
        .append()
        .default_value(false)
        .implicit_value(true)
        .nargs(0);

    // Run parsing
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        exit(1); // NOLINT(concurrency-*)
    }

    return args;
}

} // namespace

int
main(int argc, char* argv[])
{
    // Argument parsing
    auto args = parse_arguments(argc, argv);

    // Update verbosity
    binlog::Severity log_level = binlog::Severity::info;

    if (args.verbosity >= 2) // info -> debug -> trace
        log_level = binlog::Severity::trace;
    else if (args.verbosity >= 1) // info -> debug
        log_level = binlog::Severity::debug;

    binlog::default_session().setMinSeverity(log_level);

    // Transfer control to GUI
    return krompir::gui::main(argc, argv);
}
