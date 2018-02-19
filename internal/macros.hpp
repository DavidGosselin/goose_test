#pragma once

#include "macro_helpers.hpp"

#define expect_equal(expected, observed) \
    goose::TestSuite::expectEqual(expected, observed, __FILE__, __LINE__)

#define expect_cstr_equal(expected, observed) \
    goose::TestSuite::expectCStringEqual(expected, observed, __FILE__, __LINE__)

#define expect_true(expression) \
    expect_equal(true, expression)

#define expect_false(expression) \
    expect_equal(false, expression)

#define expect_throw(exception_type, expression)             \
    {                                                        \
        try {                                                \
            expression;                                      \
            goose::TestSuite::set_failure_state(true);       \
        }                                                    \
        catch (const exception_type& e) {                    \
            goose::TestSuite::set_failure_state(false);      \
        }                                                    \
        catch (...) {                                        \
            goose::TestSuite::set_failure_state(true);       \
        }                                                    \
    }

#define create_test_case(testName, suitename, casename, parent) \
    struct testName : parent { \
        void testMethod(); \
        testName() { testPrep(QUOTE(suitename), QUOTE(casename)); } \
        virtual void test() override { testMethod(); } \
    }; \
    testName _##testName; \
    void testName::testMethod()

#define test_case(suitename, casename) \
    create_test_case(goose_test_##suitename##_##casename, \
                     suitename, \
                     casename, \
                     goose::TestSuite)

#define suite_test_case(suitename, casename) \
    create_test_case(goose_test_##suitename##_##casename, \
                     suitename, \
                     casename, \
                     suitename)
