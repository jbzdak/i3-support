
#define BOOST_TEST_MODULE ExampleTest
#include <boost/test/unit_test.hpp>

#include "src/MainLoop.h"

BOOST_AUTO_TEST_SUITE(ExampleSuite)

BOOST_AUTO_TEST_CASE(TestConstructionAndDestruction)
{
    jb::pa::MainLoop * ml = new jb::pa::MainLoop("foo");
    delete ml;
}

BOOST_AUTO_TEST_CASE(TestWhetherMainLoopIsFreed) {
    // The idea is that this should crash something --- if we open 1000 main loops.
    //
    for (int ii = 0; ii < 1E4; ii++) {
        jb::pa::MainLoop *ml = new jb::pa::MainLoop("foo");
        delete ml;
    }
}

BOOST_AUTO_TEST_CASE(TestConnectionSync) {
    // The idea is that this should
    jb::pa::MainLoop ml("foo");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
}

BOOST_AUTO_TEST_CASE(TestConnectionAsync) {
    jb::pa::MainLoop ml("foo");
    ml.connect(false);
    // This might be testing a rece condition --- but I thing that
    BOOST_CHECK(ml.get_context_state() < PA_CONTEXT_READY);
}



BOOST_AUTO_TEST_SUITE_END()
