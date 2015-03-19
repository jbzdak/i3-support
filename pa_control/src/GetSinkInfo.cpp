//
// Created by jb on 3/19/15.
//

#include <stdint-gcc.h>
#include <bits/stringfwd.h>
#include <pulse/operation.h>
#include "GetSinkInfo.h"
#include "PaSink.h"

jb::pa::GetSinkInfo::GetSinkInfo(uint32_t sink_id):
  sink_id(sink_id),
  sink_name(boost::optional<std::string>()){}

jb::pa::GetSinkInfo::GetSinkInfo(std::string sink_name):
  sink_id(boost::optional<uint32_t>()),
  sink_name(sink_name){}

jb::pa::GetSinkInfo::GetSinkInfo(jb::pa::PaSink sink):
  sink_id(sink.index),
  sink_name(sink.name){}

pa_operation *jb::pa::GetSinkInfo::execute_operation_internal(pa_context *ctx, jb::pa::MainLoop *ml) {
  return nullptr;
}
