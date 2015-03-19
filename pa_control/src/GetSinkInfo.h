//
// Created by jb on 3/19/15.
//

#ifndef _PA_CONTROL_GETSINKINFO_H_
#define _PA_CONTROL_GETSINKINFO_H_

#include "PaOperation.h"
#include "PaSink.h"
#include "MainLoop.h"

namespace jb {
namespace pa {


class GetSinkInfo : public PaOperation {
public:
  GetSinkInfo(uint32_t sink_id);

protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop *ml);

  GetSinkInfo(std::string sink_name);
  GetSinkInfo(PaSink sink);

private:
  const boost::optional<uint32_t> sink_id;
  const boost::optional<std::string> sink_name;
};

}}
#endif //_PA_CONTROL_GETSINKINFO_H_
