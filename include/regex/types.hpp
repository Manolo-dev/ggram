#pragma once

#include "global_defines.hpp"
#include <functional>
#include <optional>
#include <string>
#include <string_view>

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = std::function<MatchResult(std::string_view)>;
