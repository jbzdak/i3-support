//
// Created by jb on 3/17/15.
//

#ifndef _PA_CONTROL_GETSINKLISTOP_H_
#define _PA_CONTROL_GETSINKLISTOP_H_

#include "PaOperation.h"

namespace jb {
namespace pa {

struct PaSink{

};

class GetSinkListOp : public PaOperation{
public:

protected:
  virtual pa_operation *execute_operation_internal(pa_context *mainloop);

private:
  std::vector<PaSink> sink_list;

};
}}


#endif //_PA_CONTROL_GETSINKLISTOP_H_
