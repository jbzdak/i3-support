//
// Created by jb on 3/18/15.
//

#include <stdint-gcc.h>
#include "SetSinkMuteOp.h"
#include "MainLoop.h"

using namespace jb::pa;

pa_operation *jb::pa::SetSinkMuteOp::execute_operation_internal(pa_context *ctx, jb::pa::MainLoop *ml) {
  return pa_context_set_sink_mute_by_index(ctx, sink_index, muted,
    pa_success_callback,
    this
  );
}

jb::pa::SetSinkMuteOp::SetSinkMuteOp(uint32_t sink_index, bool muted):
  PaOperation("mute sink"),
  sink_index(sink_index),
  muted(muted){}
