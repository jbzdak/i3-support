//
// Created by jb on 3/17/15.
//

#include "GetSinkListOp.h"

#include <tuple>

namespace jb{ namespace pa {

namespace GetSinkListOpDetail{

typedef std::tuple<std::vector<PaSink>&, MainLoop&> callback_usrdata;

void get_sinks_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata) {

  if (eol > 0){
    return;
  }

  callback_usrdata& usrdata =
    *reinterpret_cast<callback_usrdata*>(userdata);

  std::vector<PaSink>& sink_list = std::get<0>(usrdata);

  sink_list.push_back(PaSink(
    std::get<1>(usrdata),
    *l
  ));

}
}

pa_operation * GetSinkListOp::execute_operation_internal(
  pa_context *ctx, MainLoop* ml) {
  this->sink_list.clear();

  return pa_context_get_sink_info_list(ctx,
    GetSinkListOpDetail::get_sinks_cb,
    &this->sink_list
  );
}

PaSink::PaSink(MainLoop &ml, const pa_sink_info &info):
  ml(ml),
  name(info.name),
  description(info.description),
  index(info.index),
  muted(info.mute != 0),
  volume(info.base_volume),
  state(info.state),
  volume_steps(info.n_volume_steps) {}

}}