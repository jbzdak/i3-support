//
// Created by jb on 3/19/15.
//

#ifndef _PA_CONTROL_GETSINKINFO_H_
#define _PA_CONTROL_GETSINKINFO_H_

#include "PaOperation.h"
#include "PaSink.h"
#include "MainLoop.h"

#include <boost/variant.hpp>

namespace jb {
namespace pa {

namespace GetSinkInfoDetail{

void pa_sink_info_cb_t(
  pa_context *c, const pa_sink_info *i, int eol, void *userdata);

}


class GetSinkInfo : public PaOperation {
  friend void GetSinkInfoDetail::pa_sink_info_cb_t(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
public:
  GetSinkInfo(uint32_t sink_id);
  GetSinkInfo(std::string sink_name);
  GetSinkInfo(PaSink sink);

  const PaSink& get_sink(){
    get_result_guard();
    return sink;
  }

protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop *ml);

private:
  const boost::variant<uint32_t, std::string> sink_identifier;
  PaSink sink;
};

}}
#endif //_PA_CONTROL_GETSINKINFO_H_
