//
// Created by jb on 3/18/15.
//

#ifndef _PA_CONTROL_SETSINKMUTEOP_H_
#define _PA_CONTROL_SETSINKMUTEOP_H_

#include "PaOperation.h"
#include "MainLoop.h"


namespace jb {
namespace pa {

class SetSinkMuteOp : public PaOperation{
public:
  SetSinkMuteOp(uint32_t sink_index, bool muted);


  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop *ml) override;

private:
  const uint32_t sink_index;
  const bool muted;
};


}
}


#endif //_PA_CONTROL_SETSINKMUTEOP_H_
