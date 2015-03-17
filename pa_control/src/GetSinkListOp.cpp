//
// Created by jb on 3/17/15.
//

#include "GetSinkListOp.h"

namespace jb{ namespace pa {

namespace GetSinkListOpDetail{

void get_sinks_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata) {

}
}

pa_operation* GetSinkListOp::execute_operation_internal(pa_context *mainloop) {
  this->sink_list.clear();

  pa_op = pa_context_get_sink_info_list(pa_ctx,
    pa_sinklist_cb,
    output
  );
  return nullptr;
}

}}