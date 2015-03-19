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

SetVolumeOp::SetVolumeOp(uint32_t sink_idx, pa_volume_t desired_volume) :
  CompositeOperation("set volume"),
  sink_idx(sink_idx),
  sink(boost::optional<PaSink>()),
  desired_volume(desired_volume) {
}

SetVolumeOp::SetVolumeOp(PaSink sink, pa_volume_t desired_volume) :
  CompositeOperation("set volume"),
  sink_idx(sink.index),
  sink(sink),
  desired_volume(desired_volume) {
}

std::shared_ptr<IOperation> SetVolumeOp::get_next_operation(std::shared_ptr<IOperation> last) {
  if (last.get() == nullptr) {
    if (this->sink) {
      return std::make_shared<JustSetVolume>(*this->sink, desired_volume);
    }
    return std::make_shared<GetSinkInfo>(this->sink_idx);
  }

  GetSinkInfo *info = dynamic_cast<GetSinkInfo *>(last.get());

  if (info == nullptr) {
    throw std::logic_error("");
  }

  return std::make_shared<JustSetVolume>(info->get_sink(), desired_volume);

}

pa_operation *JustSetVolume::execute_operation_internal(pa_context *ctx, MainLoop *ml) {
  auto op =  pa_context_set_sink_volume_by_index(
    ctx, this->sink.index, &this->new_volume,
    pa_success_callback,
    this);
  return  op;
}

JustSetVolume::JustSetVolume(const PaSink sink, const pa_volume_t desired_volume) :
  PaOperation("just set volume"),
  sink(sink),
  new_volume() {
  throw std::logic_error("");
}

JustSetVolume::JustSetVolume(const PaSink sink, const pa_cvolume desired_volume):
  PaOperation("just set volume"),
  sink(sink),
  new_volume(desired_volume) {}
}}
