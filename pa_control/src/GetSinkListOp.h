//
// Created by jb on 3/17/15.
//

#ifndef _PA_CONTROL_GETSINKLISTOP_H_
#define _PA_CONTROL_GETSINKLISTOP_H_

#include "PaOperation.h"
#include "PaSink.h"

#include <vector>
#include <iostream>


namespace jb {
namespace pa {

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
