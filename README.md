# Goose Test
## What is it?
A simple, intuitive, header-only unit test framework for C++11 code.
```
#include <test/goose_test.hpp>
#include <stdlib/string.hpp>

test_case(StringTest, String)
{
    goose::string x = "hello";
    expect_cstr_equal("hello", x.c_str());
}
```
## It Kinda Reminds Me of Another Popular Test Framework
This code may remind you of another framework, but I found that I wanted something compact, implemented only with headers, MIT-licensed, and used terms like "test case" and "test suite" more intuitively.
### A Test Case...
...is a function implemented to exercise a given behavior in the target code under test (CUT).  In Goose Test, each test case is defined within
its own function, declared using the provided `test_case` macro.  Test cases by default stand alone and share no state with each other.
### A Test Suite...
...is a collection of related test cases.  While test cases are assumed to stand alone, a facility exists to share state between them if
needed.  For example, a Test Suite may depend on a network resource that may be nontrivial to create and destroy.  Your test code can
extend the TestSuite class to encapsulate shared state between test cases.  Virtual methods `start` and `end` are called at the start, and
end, of each test case sharing a TestSuite.
## Sample Test Suite
```
struct CustomTestSuite : goose::TestSuite
{
    virtual void start()
    {
        // Called before the SanityCheck test case runs
        shared_state = 5;
    }

    virtual void end()
    {
        // Called after the SanityCheck test case runs
    }

    unsigned int shared_state = 0;
};

suite_test_case(CustomTestSuite, SanityCheck)
{
    expect_equal(5, shared_state);
}
```
## Deployment
You can copy the headers directly into your project, or use this repo as a git submodule (https://git-scm.com/book/en/v2/Git-Tools-Submodules),
or just copy whatever you'd like from here within the guidelines of the MIT license.
To save you the boilerplate, a custom `main` function for the test application targeting the CUT is provided that will register your test
cases with the internal test framework automagically.  You need only include the `goose_test.hpp` header and you're off to testland.
## Supported Compilers
## TODOs
 - Fill-out the `Supported Compilers` section.
 - Supply inline documentation for macros and Test Suite to generate online help
 - Supply working examples
 - Provide cookie-cutter support for statically initialized data members in TestSuite
