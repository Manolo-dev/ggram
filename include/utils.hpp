#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vector);
/**
 * @brief Implements the * operator between string and int. It behave like in python i.e "a" * 5 ==
 * "aaaaa"
 *
 * @param base_string the string to duplicate
 * @param n the number of duplications to perform
 * @return std::string the duplicated string
 */
std::string operator*(const std::string &base_string, size_t n);
std::string operator*(size_t n, const std::string &base_string);

/**
 * @brief Concatenate two vector toguether
 *
 * @tparam T Types of the two vectors
 * @param dest the destination vector
 * @param src the vector to concatenate at the end of the dest vector
 */
template<typename T> void extend(std::vector<T> &dest, const std::vector<T> &src) {
    dest.reserve(dest.size() + src.size());
    dest.insert(dest.end(), src.begin(), src.end());
}

/**
 * @brief Push an element to each element of a given vector
 *
 * @tparam T the type of the elements
 * @param list the list of vector to push
 * @param new_elem the element to push into each vector
 */
template<typename T> void pushToEachElement(std::vector<std::vector<T>> &list, const T &new_elem) {
    for (auto &elem : list) {
        elem.push_back(new_elem);
    }
}
