# Leetcode TestKit

A simple and powerful header-only C++ testing framework for Leetcode-style problems. Quickly test your solutions using the examples provided in problem statements.

## Features

- 🚀 **Header-only** - Just include one file and you're ready to go
- 🎨 **Colorful output** - Easy-to-read test results with green/red indicators
- 📝 **Multiple function signatures** - Support for various parameter combinations
- 🏗️ **Class method testing** - Perfect for Leetcode's `Solution` class format
- 🔧 **Flexible templates** - Works with any input/output types
- 📊 **Detailed reporting** - Shows input, output, expected results, and pass/fail counts

## Quick Start

1. **Add the header to your project:**
```cpp
#include "leetcode_testkit.h"
```

2. **Create your solution:**
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // Your implementation here
    }
};
```

3. **Test with examples:**
```cpp
int main() {
    Solution solution;

    TestClassMethod<Solution, vector<int>, int, vector<int>>(
        "Two Sum",
        &solution,
        &Solution::twoSum,
        {
            { {{2, 7, 11, 15}, 9}, {0, 1} },
            { {{3, 2, 4}, 6}, {1, 2} },
            { {{3, 3}, 6}, {0, 1} },
            { {{1, 2, 3}, 10}, {} }
        }
    );

    return 0;
}
```

## Usage Examples

### Testing Single Parameter Functions
```cpp
// For problems like: bool isPalindrome(int x)
TestSolution("Palindrome Number",
    [](int x) { return solution(x); },
    {
        {121, true},
        {-121, false},
        {10, false}
    }
);
```

### Testing Vector Solutions
```cpp
// Specialized output for vector problems
TestVectorSolution("Find Max",
    findMax,
    {
        {{1, 2, 3, 4, 5}, 5},
        {{-1, -2, -3}, -1},
        {{42}, 42}
    }
);
```

### Testing Class Methods Directly
```cpp
Solution sol;
TestClassMethod("Two Sum (Class Method)",
    &sol, 
    &Solution::twoSum,
    {
        {{{2,7,11,15}, 9}, {0,1}},
        {{{3,2,4}, 6}, {1,2}}
    }
);
```

## Available Testing Functions

- `TestSolution()` - General purpose, 1 or 2 parameters
- `TestVectorSolution()` - Specialized for vector inputs
- `TestTwoParamSolution()` - For two-parameter functions with custom output
- `TestClassMethod()` - Direct class method testing
- `PrintVector()` - Utility for pretty vector printing

## Sample Output

```
Testing Two Sum:
--------------------------------------------------------------------
Test 1: PASSED nums = [2,7,11,15], target = 9 -> Output: [0,1] Expected: [0,1]
Test 2: PASSED nums = [3,2,4], target = 6 -> Output: [1,2] Expected: [1,2]
Test 3: PASSED nums = [3,3], target = 6 -> Output: [0,1] Expected: [0,1]
--------------------------------------------------------------------
Result: 3/3 tests passed
All tests PASSED! ✓
```

## Requirements

- C++11 or later
- Standard library (`<iostream>`, `<vector>`, `<string>`, `<cassert>`)

## Why Use Leetcode TestKit?

- **Save Time** - No more manual test case checking
- **Avoid Mistakes** - Automated comparison eliminates human error
- **Rapid Iteration** - Test multiple solutions quickly
- **Professional Workflow** - Structured testing similar to real development
