//
// Created by jb on 3/17/15.
//

#ifndef _PA_CONTROL_GETSINKLISTOP_H_
#define _PA_CONTROL_GETSINKLISTOP_H_

#include "PaOperation.h"

#include <vector>

namespace jb {
namespace pa {

class PaSink{
private:

  MainLoop& ml;

public:

  PaSink(MainLoop &ml, const pa_sink_info& info);

public:

  const std::string name, description;
  const uint32_t index;
  const bool muted;

  const pa_volume_t volume;
  const pa_sink_state_t state;
  const uint32_t volume_steps;

};

class GetSinkListOp : public PaOperation{
public:

  std::vector<PaSink> get_sink_list(){
    get_result_guard();
    return sink_list;
  }

protected:
  virtual pa_operation *execute_operation_internal(pa_context *mainloop, MainLoop* ml);

private:
  std::vector<PaSink> sink_list;

};
}}


#endif //_PA_CONTROL_GETSINKLISTOP_H_
