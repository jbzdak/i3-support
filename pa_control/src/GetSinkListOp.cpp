//
// Created by jb on 3/17/15.
//

#include "GetSinkListOp.h"

#include <tuple>

namespace jb{ namespace pa {

namespace GetSinkListOpDetail{

typedef std::tuple<std::vector<PaSink>*, MainLoop*> callback_usrdata;

void get_sinks_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata) {

  callback_usrdata* usrdata =
    reinterpret_cast<callback_usrdata*>(userdata);

  if (eol > 0){
    delete usrdata;
    return;
  }

  std::vector<PaSink>*sink_list = nullptr;
  MainLoop* ml = nullptr;

  std::tie(sink_list, ml) = *usrdata;

  sink_list->push_back(PaSink(
    *l
  ));

}
}

using namespace GetSinkListOpDetail;

pa_operation * GetSinkListOp::execute_operation_internal(
  pa_context *ctx, MainLoop* ml) {
  this->sink_list.clear();

  callback_usrdata* usrdata = new callback_usrdata(&this->sink_list, ml);

  return pa_context_get_sink_info_list(ctx,
    GetSinkListOpDetail::get_sinks_cb,
    usrdata
  );
}

GetSinkListOp::GetSinkListOp():
  PaOperation("get sink list info"){}
}}

