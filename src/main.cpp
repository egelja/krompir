#include "lib.hpp"
#include "logging.hpp"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

int
main()
{
    auto const lib = library{};
    auto const message = "Hello from " + lib.name + "!";
    std::cout << message << '\n';

    // change log pattern
    krompir::logging::init();

    log_i("Welcome to spdlog!");
    log_w("Easy padding in numbers like {:08d}", 12);
    log_e("Some error message with arg: {}", 1);
    log_c("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    log_i("Support for floats {:03.2f}", 1.23456);
    log_i("Positional args are {1} {0}..", "too", "supported");
    log_i("{:<30}", "left aligned");

    for (int i = 0; i < 100; i++) {
        log_d("Backtrace message {}", i); // not logged yet..
        log_t("Trace!!!");
    }
    log_e("Oops UwU");

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    log_i("Some trace message with param {}", 42);
    log_w("Some debug message");

    return 0;
}
