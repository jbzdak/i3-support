//
// Created by jb on 3/16/15.
//

#ifndef _PA_CONTROL_PAOPERATION_H_
#define _PA_CONTROL_PAOPERATION_H_

#include <pulse/pulseaudio.h>
#include <boost/optional.hpp>
#include "MainLoop.h"
#include "pa_errors.h"

namespace jb {
namespace pa {

enum class OperationState {
  UNSCHEDULED,
  PENDING,
  DONE
};

class PaOperation {
public:
  friend class MainLoop;
  virtual ~PaOperation();
  virtual void execute_operation(MainLoop& ml);
  virtual OperationState get_state() {return state;}

protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx) = 0;

  void get_result_guard();

private:
  OperationState state = OperationState::UNSCHEDULED;
  boost::optional<pa_operation*> internal_operation;

  void ping_state();
};


}}


#endif //_PA_CONTROL_PAOPERATION_H_
