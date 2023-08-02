#pragma once

#include "config.h"
#include "logging.hpp"
#include "utils/utils.hpp"

#include <fmt/core.h>
#include <fmt/xchar.h>

#include <string>

#define UNUSED(x)             (void)(x)

#define __STRINGIFY_HELPER(x) #x
#define STRINGIFY(x)          __STRINGIFY_HELPER(x)
