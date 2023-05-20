#pragma once

#include "ctre/ctre.hpp"

static constexpr auto LEXME_INFOS_REGEX =
    ctll::fixed_string{R"-(^(\.?[a-zA-Z][a-zA-Z_0-9]*)\s+"(([^"]|\\")*)"$)-"};

static constexpr auto LEXME_NAME_REGEX = ctll::fixed_string{R"-(^\.?[a-zA-Z][a-zA-Z_0-9]*\s+)-"};