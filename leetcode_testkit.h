#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <utility>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <forward_list>
#include <optional>
#include <tuple>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"

// ----------------- type traits -----------------

// is_pair
template <typename T>
struct is_pair : std::false_type {};

template <typename A, typename B>
struct is_pair<std::pair<A, B>> : std::true_type {};

// is_string
template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

// is_map_like (has key_type and mapped_type)
template <typename T, typename = void>
struct is_map_like : std::false_type {};

template <typename T>
struct is_map_like<T, std::void_t<typename T::key_type, typename T::mapped_type>> : std::true_type {};

// is_iterable (has begin/end) but exclude std::string and map-like (maps handled separately)
template <typename T, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::integral_constant<bool, !is_string<T>::value && !is_map_like<T>::value> {};

// is_streamable: check if oss << t is valid
template <typename T, typename = void>
struct is_streamable : std::false_type {};

template <typename T>
struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
    : std::true_type {};

// ----------------- ToString overloads (mutually exclusive) -----------------

// forward declaration of generic ToString so dependent calls inside detail::ToStringElement compile
template <typename T>
std::string ToString(const T& v);

// C-style strings
inline std::string ToString(const char* s) {
    if (!s) return "null";
    return std::string("\"") + s + "\"";
}

// std::string
inline std::string ToString(const std::string& s) {
    return std::string("\"") + s + "\"";
}

// bool prints true/false
inline std::string ToString(bool b) {
    return b ? "true" : "false";
}

// std::optional<T>
template <typename T>
std::string ToString(const std::optional<T>& opt) {
    if (!opt.has_value()) return "nullopt";
    return std::string("optional(") + ToString(*opt) + ")";
}

// std::pair<A,B>
template <typename A, typename B>
std::string ToString(const std::pair<A, B>& p) {
    std::ostringstream oss;
    oss << "(" << ToString(p.first) << ", " << ToString(p.second) << ")";
    return oss.str();
}

// std::tuple<Ts...>
namespace detail {
    template <typename Tuple, std::size_t... Is>
    std::string TupleToStringImpl(const Tuple& t, std::index_sequence<Is...>) {
        std::ostringstream oss;
        oss << "(";
        bool first = true;
        ((void)(
            (first ? (first = false, oss << ToString(std::get<Is>(t)))
                   : (oss << ", " << ToString(std::get<Is>(t))))
        ), ...);
        oss << ")";
        return oss.str();
    }
}

template <typename... Ts>
std::string ToString(const std::tuple<Ts...>& t) {
    return detail::TupleToStringImpl(t, std::index_sequence_for<Ts...>{});
}

// helper for printing a single element in a container (deterministic)
namespace detail {
    template <typename U>
    std::string ToStringElement(const U& x) {
        if constexpr (std::is_same_v<U, bool>) {
            return ::ToString(x);
        } else if constexpr (std::is_integral_v<U>) {
            return std::to_string(x);
        } else {
            // explicitly call global ToString to avoid lookup issues
            return ::ToString(x);
        }
    }
}

// --- Single universal template that dispatches by traits to avoid conflicting overloads ---
template <typename T>
std::string ToString(const T& v) {
    if constexpr (is_map_like<T>::value) {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& kv : v) {
            if (!first) oss << ", ";
            first = false;
            oss << ToString(kv.first) << ": " << ToString(kv.second);
        }
        oss << "}";
        return oss.str();
    } else if constexpr (is_iterable<T>::value) {
        std::ostringstream oss;
        oss << "[";
        auto it = std::begin(v);
        auto end_it = std::end(v);
        bool first = true;
        for (; it != end_it; ++it) {
            if (!first) oss << ", ";
            first = false;
            oss << detail::ToStringElement(*it);
        }
        oss << "]";
        return oss.str();
    } else if constexpr (!is_pair<T>::value && is_streamable<T>::value && !is_string<T>::value) {
        std::ostringstream oss;
        oss << v;
        return oss.str();
    } else {
        return "<unprintable>";
    }
}

// ----------------- testing functions -----------------

template <typename Input, typename Output, typename SolutionFunc>
void TestSolution(const std::string& testName, SolutionFunc solution,
                 const std::vector<std::pair<Input, Output>>& testCases) {
    std::cout << std::endl;
    std::cout << CYAN << "Testing " << testName << ":\n" << RESET;
    std::cout << "--------------------------------------------------------------------\n";

    int passedCount = 0;

    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& [input, expected] = testCases[i];

        Output result = solution(input);

        bool isCorrect = (result == expected);

        std::cout << "Test " << i + 1 << ": ";
        if (isCorrect) {
            std::cout << GREEN << "PASSED" << RESET;
            passedCount++;
        } else {
            std::cout << RED << "FAILED" << RESET;
        }

        std::cout << " Input: " << ToString(input)
                  << " Output: " << ToString(result)
                  << " Expected: " << ToString(expected)
                  << "\n";
    }

    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "Result: " << passedCount << "/" << testCases.size() << " tests passed\n";

    if (passedCount == static_cast<int>(testCases.size())) {
        std::cout << GREEN << "All tests PASSED! ✓" << RESET << std::endl;
    } else {
        std::cout << RED << "Some tests FAILED! ✗" << RESET << std::endl;
    }
    std::cout << std::endl;
}

template <typename Input1, typename Input2, typename Output, typename SolutionFunc>
void TestSolution(const std::string& testName, SolutionFunc solution,
                 const std::vector<std::pair<std::pair<Input1, Input2>, Output>>& testCases) {
    std::cout << std::endl;
    std::cout << CYAN << "Testing " << testName << ":\n" << RESET;
    std::cout << "--------------------------------------------------------------------\n";

    int passedCount = 0;

    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& [inputs, expected] = testCases[i];
        const auto& [input1, input2] = inputs;

        Output result = solution(input1, input2);
        bool isCorrect = (result == expected);

        std::cout << "Test " << i + 1 << ": ";
        if (isCorrect) {
            std::cout << GREEN << "PASSED" << RESET;
            passedCount++;
        } else {
            std::cout << RED << "FAILED" << RESET;
        }

        std::cout << " Input: [" << ToString(input1) << ", " << ToString(input2) << "]"
                  << " Output: " << ToString(result)
                  << " Expected: " << ToString(expected) << "\n";
    }

    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "Result: " << passedCount << "/" << testCases.size() << " tests passed\n\n";
}

// PrintVector helper (kept for compatibility)
template <typename T>
void PrintVector(const std::vector<T>& vec) {
    std::cout << ToString(vec);
}

// template for testing class methods
template <typename ClassType, typename Input1, typename Input2, typename Output, typename Method>
void TestClassMethod(const std::string& testName, ClassType* obj, Method method,
                     const std::vector<std::pair<std::pair<Input1, Input2>, Output>>& testCases) {
    std::cout << std::endl;
    std::cout << CYAN << "Testing " << testName << ":\n" << RESET;
    std::cout << "--------------------------------------------------------------------\n";

    int passedCount = 0;

    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& [inputs, expected] = testCases[i];
        const auto& [input1, input2] = inputs;

        // Calling a class method
        Output result = (obj->*method)(input1, input2);
        bool isCorrect = (result == expected);

        std::cout << "Test " << i + 1 << ": ";
        if (isCorrect) {
            std::cout << GREEN << "PASSED" << RESET;
            passedCount++;
        } else {
            std::cout << RED << "FAILED" << RESET;
        }

        std::cout << " Input: [" << ToString(input1) << ", " << ToString(input2) << "]"
                  << " -> Output: " << ToString(result)
                  << " Expected: " << ToString(expected) << "\n";
    }

    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "Result: " << passedCount << "/" << testCases.size() << " tests passed\n";

    if (passedCount == static_cast<int>(testCases.size())) {
        std::cout << GREEN << "All tests PASSED! ✓" << RESET << std::endl;
    } else {
        std::cout << RED << "Some tests FAILED! ✗" << RESET << std::endl;
    }
    std::cout << std::endl;
}

