#include "utils.hpp"

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vector) {
    os << "[";

    for (size_t i = 0; i < vector.size(); i++) {
        os << vector[i];
        if (i != vector.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

std::string operator*(const std::string &base_string, size_t n) {
    std::stringstream out;

    while ((n--) != 0U) {
        out << base_string;
    }
    return out.str();
}

std::string operator*(size_t n, const std::string &base_string) {
    return base_string * n;
}
