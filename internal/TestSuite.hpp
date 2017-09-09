#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <list>

namespace goose {

class TestSuite
{
    static bool failure;
    static std::ostream* output;
    static std::list<std::function<void()>> testSuites;

    void preamble(const char* suiteName, const char* caseName)
    {
        *output << suiteName << "." << caseName << "\n";
        start();
    }

    void postamble(const char* suiteName, const char* caseName)
    {
        end();
        *output << suiteName << "."
                << caseName << " "
                << (failure ? "FAILED\n" : "passed") << "\n";
    }

protected:
    void testPrep(const char* suiteName, const char* caseName)
    {
        auto t = [this, suiteName, caseName] () {
            preamble(suiteName, caseName);
            test();
            postamble(suiteName, caseName);
        };

        testSuites.push_back(std::move(t));
    }

    virtual void test() = 0;

    virtual void start() {}

    virtual void end() {}

public:
    template <typename ExpectedT,
              typename ObservedT,
              typename FileNameT,
              typename LineNumberT>
    static void expectEqual(const ExpectedT& expected,
                            const ObservedT& observed,
                            const FileNameT& filename,
                            const LineNumberT& lineNumber)
    {
        auto pass = expected == observed;

        if (!pass) {
            *output << "Test Failure at "
                    << filename << ":"
                    << lineNumber << std::endl;
        }

        failure = !pass;
    }

    template <typename FileNameT,
              typename LineNumberT>
    static void expectCStringEqual(const char* expected,
                                   const char* observed,
                                   const FileNameT& filename,
                                   const LineNumberT& lineNumber)
    {
        auto expectedInvalid = expected == nullptr;
        auto observedInvalid = observed == nullptr;
        if (expectedInvalid || observedInvalid) {
            *output << (expectedInvalid ? "expected value " : "")
                    << (expectedInvalid && observedInvalid ? "and " : "")
                    << (observedInvalid ? "observed value " : "")
                    << "invalid" << std::endl;
        }

        auto pass = strcmp(observed, expected) == 0;

        if (!pass) {
            *output << "Test Failure at "
                    << filename << ":"
                    << lineNumber << std::endl;
        }

        failure = !pass;
    }

    static int execute()
    {
        output = &std::cout;
        for (const auto& testSuite : testSuites) testSuite();
        return 0;
    }
};

bool TestSuite::failure;
std::ostream* TestSuite::output;
std::list<std::function<void()>> TestSuite::testSuites;

}  // namespace goose
