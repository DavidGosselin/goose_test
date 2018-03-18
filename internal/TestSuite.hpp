#pragma once

#include <goose/OutputStream.h>

#include <functional>
#include <list>

namespace goose {

class TestSuite
{
    static bool failure;
    static OutputStream output;
    static std::list<std::function<void()>> testSuites;

    void preamble(const char* suiteName, const char* caseName)
    {
        output << suiteName << "." << caseName << goose::endl;
        start();
    }

    void postamble(const char* suiteName, const char* caseName)
    {
        end();
        output << suiteName << "."
               << caseName << " "
               << (failure ? "FAILED\n" : "passed") << goose::endl;
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
            output << "Test Failure at "
                   << filename << ":"
                   << lineNumber << goose::endl;
        }

        failure = !pass;
    }

    static bool strings_match(const char* s1,
                              const char* s2)
    {
        auto i = 0;
        while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]) ++i;
        return s1[i] == 0 && s2[i] == 0;
    }

    template <typename FileNameT,
              typename LineNumberT>
    static void expectCStringEqual(const char* expected,
                                   const char* observed,
                                   const FileNameT& filename,
                                   const LineNumberT& lineNumber)
    {
        auto pass = strings_match(observed, expected);

        if (!pass) {
            output << "Test Failure at "
                   << filename << ":"
                   << lineNumber << goose::endl;

            auto expectedInvalid = expected == nullptr;
            auto observedInvalid = observed == nullptr;
            output << "  expected value: " << (expectedInvalid ? "<null>" : expected)
                   << "\n  observed value: " << (observedInvalid ? "<null>" : observed)
                   << goose::endl;
        }

        failure = !pass;
    }

    static void set_failure_state(bool fs)
    {
        failure = fs;
    }

    static int execute()
    {
        for (const auto& testSuite : testSuites) testSuite();
        return 0;
    }
};

bool TestSuite::failure;
OutputStream TestSuite::output;
std::list<std::function<void()>> TestSuite::testSuites;

}  // namespace goose
