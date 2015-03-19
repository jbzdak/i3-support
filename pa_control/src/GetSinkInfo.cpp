//
// Created by jb on 3/19/15.
//

#include <stdint-gcc.h>
#include <bits/stringfwd.h>
#include <pulse/operation.h>
#include "GetSinkInfo.h"
#include "PaSink.h"

namespace jb{ 
namespace pa {

namespace GetSinkInfoDetail{

void pa_sink_info_cb_t(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
  if (eol > 0){
    return;
  }
  GetSinkInfo* info = reinterpret_cast<GetSinkInfo*>(userdata);
  info->sink = PaSink(*i);
}

class GetInfoVisitor : public boost::static_visitor<pa_operation*>{

public:

  pa_operation* operator()(const uint32_t index) const{
    return pa_context_get_sink_info_by_index(
      ctx, index, pa_sink_info_cb_t, info
    );
  }

  pa_operation* operator()(const std::string name) const{
    return pa_context_get_sink_info_by_name(
      ctx, name.c_str(), pa_sink_info_cb_t, info
    );
  }

  GetInfoVisitor(pa_context* ctx, GetSinkInfo*info):
    ctx(ctx), info(info){}

private:
  pa_context* ctx;
  GetSinkInfo* info;
};
}

using namespace GetSinkInfoDetail;

GetSinkInfo::GetSinkInfo(uint32_t sink_id) :
  PaOperation("get sink info"),
  sink_identifier(sink_id) {
};

GetSinkInfo::GetSinkInfo(std::string sink_name) :
  PaOperation("get sink info"),
  sink_identifier(sink_name) {
};

GetSinkInfo::GetSinkInfo(PaSink sink) :
  PaOperation("get sink info"),
  sink_identifier(sink.index) {
};

pa_operation *GetSinkInfo::execute_operation_internal(
  pa_context *ctx, MainLoop *ml) {
  return boost::apply_visitor(GetInfoVisitor(ctx, this), sink_identifier);
}


}}