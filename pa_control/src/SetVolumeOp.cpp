//
// Created by jb on 3/19/15.
//

#include <stdint-gcc.h>
#include <pulse/volume.h>
#include <pulse/operation.h>
#include "SetVolumeOp.h"
#include "PaSink.h"
#include "GetSinkInfo.h"

namespace jb {
namespace pa {

pa_operation *JustSetVolume::execute_operation_internal(pa_context *ctx, MainLoop *ml) {
  auto op =  pa_context_set_sink_volume_by_index(
    ctx, this->sink.index, &this->new_volume,
    pa_success_callback,
    this);
  return  op;
}

JustSetVolume::JustSetVolume(const PaSink sink, const pa_cvolume desired_volume):
  PaOperation("just set volume"),
  sink(sink),
  new_volume(desired_volume) {}
}}
