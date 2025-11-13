# LeetCode TestKit

A comprehensive C++ testing framework designed specifically for LeetCode problem solutions. This header-only library provides powerful testing utilities with beautiful console output and extensive type support.

## Features

- 🧪 **Easy Test Creation** - Simple API for defining test cases and running solutions
- 🎨 **Colorful Output** - Clear pass/fail indicators with colored console output
- 📦 **Comprehensive Type Support** - Automatic pretty-printing for:
  - Primitive types (int, double, bool, etc.)
  - STL containers (vector, list, deque, set, map, etc.)
  - Strings and C-style strings
  - Pairs and tuples
  - Optional values
  - Custom types (with stream support)
- 🔧 **Flexible Testing** - Support for:
  - Single-parameter functions
  - Two-parameter functions  
  - Class methods
- 🚀 **Header-Only** - Just include and use, no compilation needed

## Quick Start

```cpp
#include "leetcode_testkit.h"
#include <vector>

// Example: Testing a two-sum like function
std::vector<int> twoSum(std::vector<int>& nums, int target) {
    // Your solution here
}

int main() {
    TestSolution("Two Sum", twoSum, {
        {{{2, 7, 11, 15}, 9}, {0, 1}},
        {{{3, 2, 4}, 6}, {1, 2}},
        {{{3, 3}, 6}, {0, 1}}
    });
    
    return 0;
}
```

## Installation

Simply copy `leetcode_testkit.h` into your project and include it:

```cpp
#include "leetcode_testkit.h"
```

No additional dependencies required (uses C++11 standard library).

## Usage Examples

### Testing Single-Parameter Functions

```cpp
int square(int x) {
    return x * x;
}

int main() {
    TestSolution("Square function", square, {
        {2, 4},
        {5, 25},
        {-3, 9},
        {0, 0}
    });
}
```

### Testing Two-Parameter Functions

```cpp
int add(int a, int b) {
    return a + b;
}

int main() {
    TestSolution("Add function", add, {
        {{{2, 3}, 5}},
        {{{-1, 1}, 0}},
        {{{10, -5}, 5}}
    });
}
```

### Testing Class Methods

```cpp
class Calculator {
public:
    int multiply(int a, int b) {
        return a * b;
    }
};

int main() {
    Calculator calc;
    TestClassMethod("Multiply method", &calc, &Calculator::multiply, {
        {{{4, 5}, 20}},
        {{{-2, 3}, -6}},
        {{{0, 100}, 0}}
    });
}
```

## Supported Data Types

The library automatically handles pretty-printing for:

- **Primitives**: `int`, `double`, `bool`, `char`, etc.
- **Strings**: `std::string`, `const char*`
- **Containers**: 
  - Sequence: `vector`, `list`, `deque`, `forward_list`
  - Associative: `map`, `set`, `unordered_map`, `unordered_set`
- **Utility**: `pair`, `tuple`, `optional`
- **Custom Types**: Any type with `operator<<` support

### Example Outputs

```cpp
std::vector<int> vec = {1, 2, 3};
// Prints: [1, 2, 3]

std::map<std::string, int> m = {{"a", 1}, {"b", 2}};
// Prints: {"a": 1, "b": 2}

std::pair<int, std::string> p = {42, "answer"};
// Prints: (42, "answer")

std::optional<int> opt = 5;
// Prints: optional(5)
```

## API Reference

### Main Testing Functions

```cpp
// Single parameter functions
template <typename Input, typename Output, typename SolutionFunc>
void TestSolution(const std::string& testName, 
                 SolutionFunc solution,
                 const std::vector<std::pair<Input, Output>>& testCases);

// Two parameter functions  
template <typename Input1, typename Input2, typename Output, typename SolutionFunc>
void TestSolution(const std::string& testName,
                 SolutionFunc solution,
                 const std::vector<std::pair<std::pair<Input1, Input2>, Output>>& testCases);

// Class methods
template <typename ClassType, typename Input1, typename Input2, typename Output, typename Method>
void TestClassMethod(const std::string& testName,
                    ClassType* obj,
                    Method method,
                    const std::vector<std::pair<std::pair<Input1, Input2>, Output>>& testCases);
```

### Utility Functions

```cpp
// Convert any supported type to string for debugging
template <typename T>
std::string ToString(const T& value);

// Print vector (legacy compatibility)
template <typename T>
void PrintVector(const std::vector<T>& vec);
```

## Complete Example

```cpp
#include "leetcode_testkit.h"
#include <vector>
#include <unordered_map>

// LeetCode problem solution
std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> num_map;
    for (int i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];
        if (num_map.find(complement) != num_map.end()) {
            return {num_map[complement], i};
        }
        num_map[nums[i]] = i;
    }
    return {};
}

int main() {
    // Test the solution
    TestSolution("Two Sum", twoSum, {
        {{{2, 7, 11, 15}, 9}, {0, 1}},
        {{{3, 2, 4}, 6}, {1, 2}},
        {{{3, 3}, 6}, {0, 1}},
        {{{1, 2, 3}, 10}, {}}  // No solution case
    });

    return 0;
}
```

## Sample Output

```
Testing Two Sum:
--------------------------------------------------------------------
Test 1: PASSED Input: [[2, 7, 11, 15], 9] Output: [0, 1] Expected: [0, 1]
Test 2: PASSED Input: [[3, 2, 4], 6] Output: [1, 2] Expected: [1, 2]
Test 3: PASSED Input: [[3, 3], 6] Output: [0, 1] Expected: [0, 1]
Test 4: PASSED Input: [[1, 2, 3], 10] Output: [] Expected: []
--------------------------------------------------------------------
Result: 4/4 tests passed
All tests PASSED! ✓
```

## Requirements

- C++11 or later
- Standard Library

## Contributing

Feel free to extend the library with additional features:
- More container types
- Custom comparators
- Performance timing
