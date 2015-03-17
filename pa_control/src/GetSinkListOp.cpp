//
// Created by jb on 3/17/15.
//

#include "GetSinkListOp.h"

namespace jb{ namespace pa {

namespace GetSinkListOpDetail{

void get_sinks_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata) {

  if (eol > 0){
    return;
  }

  std::vector<PaSink>* sink_list =
    reinterpret_cast<std::vector<PaSink>*>(userdata);



}
}

pa_operation* GetSinkListOp::execute_operation_internal(pa_context *ctx) {
  this->sink_list.clear();

  return pa_context_get_sink_info_list(ctx,
    GetSinkListOpDetail::get_sinks_cb,
    &this->sink_list
  );
}

}}