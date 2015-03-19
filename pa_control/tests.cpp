
#define BOOST_TEST_MODULE ExampleTest
#include <boost/test/unit_test.hpp>

#include <iostream>

#include "src/MainLoop.h"
#include "src/GetSinkListOp.h"
#include "src/GetSinkInfo.h"
#include "src/SetVolumeOp.h"


using namespace jb::pa;

std::vector<PaSink> get_all_sinks(MainLoop& ml){
    auto sink_op = std::make_shared<GetSinkListOp>();
    ml.schedule_operation(sink_op);
    ml.wait_for_all_pending_operations();
    return sink_op->get_sink_list();
}

template<typename arg>
PaSink get_sink(MainLoop& ml, arg a){
    auto op = std::make_shared<GetSinkInfo>(a);
    ml.schedule_operation(op);
    ml.wait_for_all_pending_operations();
    return op->get_sink();
}

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
    // This might be testing a race condition --- but I thing that
    // we will never be able to connect that fast.
    BOOST_CHECK(ml.get_context_state() < PA_CONTEXT_READY);
}

BOOST_AUTO_TEST_CASE(TestGetSinkList) {
    jb::pa::MainLoop ml("foo");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
    auto sink_op = std::make_shared<GetSinkListOp>();
    ml.schedule_operation(sink_op);
    ml.wait_for_all_pending_operations();
    auto sinks = sink_op->get_sink_list();
    BOOST_CHECK(sinks.size() > 0);

    for (auto s: sinks){
        std::cout << s << std::endl;
    }

}
//
//BOOST_AUTO_TEST_CASE(TestMuteSink) {
//    jb::pa::MainLoop ml("foo");
//    ml.connect(true);
//    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
//    ml.mute_sink(1, false, true);
//
//}

BOOST_AUTO_TEST_CASE(CompareSinkListWithSinkGotById) {
    jb::pa::MainLoop ml("foo");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
    auto sinks = get_all_sinks(ml);

    for (auto s: sinks){
        BOOST_CHECK(s == get_sink(ml, s.index));
    }

}

BOOST_AUTO_TEST_CASE(CompareSinkListWithSinkGotByName) {
    jb::pa::MainLoop ml("foo");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
    auto sinks = get_all_sinks(ml);

    for (auto s: sinks){
        BOOST_CHECK(s == get_sink(ml, s.name));
    }

}

BOOST_AUTO_TEST_CASE(CompareSinkListWithSinkGotBySink) {
    jb::pa::MainLoop ml("foo");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
    auto sinks = get_all_sinks(ml);

    for (auto s: sinks){
        BOOST_CHECK(s == get_sink(ml, s));
    }

}


BOOST_AUTO_TEST_CASE(TestMuteSink) {
    jb::pa::MainLoop ml("foo");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
    auto sinks = get_all_sinks(ml);
    for (auto s: sinks){

        ml.mute_sink(s.index, !s.muted, true);
        auto new_sink = get_sink(ml, s.index);
        BOOST_CHECK(!!new_sink.muted != !!s.muted);
        ml.mute_sink(s.index, s.muted, true);
        BOOST_CHECK(s == get_sink(ml, s));
    }
}


BOOST_AUTO_TEST_CASE(TestDecreaseVolume) {
    jb::pa::MainLoop ml("bar");
    ml.connect(true);
    BOOST_CHECK(ml.get_context_state() == PA_CONTEXT_READY);
    auto sinks = get_all_sinks(ml);
    for (auto s: sinks){
        if (s.state != PA_SINK_RUNNING){
            continue;
        }
        std::cout << s << std::endl;
        auto new_volume = s.channel_volume; // copy
//        pa_sw_cvolume_divide_scalar(&new_volume, &s.channel_volume, 2);
        auto op = std::make_shared<JustSetVolume>(s, new_volume);
        ml.schedule_operation(op);
        ml.wait_for_all_pending_operations();
//        auto new_sink = get_sink(ml, s.index);
//        BOOST_CHECK(!!new_sink.muted != !!s.muted);
//        ml.mute_sink(s.index, s.muted, true);
//        BOOST_CHECK(s == get_sink(ml, s));
    }
}


BOOST_AUTO_TEST_SUITE_END()
