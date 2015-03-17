//
// Created by jb on 3/16/15.
//

#ifndef _PA_CONTROL_PAOPERATION_H_
#define _PA_CONTROL_PAOPERATION_H_

#include <memory>

#include <pulse/pulseaudio.h>
#include <boost/optional.hpp>

#include "pa_errors.h"


namespace jb {
namespace pa {

class MainLoop;

enum class OperationState {
  UNSCHEDULED,
  PENDING,
  DONE
};

class PaOperation {
public:
  friend class MainLoop;
  virtual ~PaOperation();
  virtual void execute_operation(MainLoop* ml);
  virtual OperationState get_state() {return state;}

  /**
  * We take ownership of pointed callback.
  *
  * Callbacks will be called whether state of this instance changes,
  * parameter to the callback will be this.
  */
  void add_callback(std::unique_ptr<std::function<void(PaOperation*)>> cb);

  bool is_done() {return state == OperationState::DONE;}

protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop* ml) = 0;

  void get_result_guard();

private:
  OperationState state = OperationState::UNSCHEDULED;
  boost::optional<pa_operation*> internal_operation;
  std::vector<std::unique_ptr<std::function<void(PaOperation*)>>> callbacks;
  void ping_state();
  void set_state(OperationState state);
};


}}


#endif //_PA_CONTROL_PAOPERATION_H_
