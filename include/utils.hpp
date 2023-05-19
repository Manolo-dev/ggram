#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vector);
std::string operator*(const std::string &base_string, size_t n);
std::string operator*(size_t n, const std::string &base_string);
template<typename T> void extend(std::vector<T> &dest, const std::vector<T> &src);
template<typename T> void pushToEachElement(std::vector<std::vector<T>> &list, const T &new_elem);
