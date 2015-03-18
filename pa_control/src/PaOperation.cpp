//
// Created by jb on 3/16/15.
//

#include "PaOperation.h"
#include "MainLoop.h"

namespace jb {
namespace pa {

void PaOperation::execute_operation(MainLoop* ml){
  internal_operation = execute_operation_internal(ml->pa_ctx.get(), ml);
  set_state(OperationState::PENDING);
}

PaOperation::~PaOperation() {
  if (this->internal_operation){
    pa_operation_unref(*this->internal_operation);
  }
}

void PaOperation::get_result_guard(){
  if (state != OperationState::DONE){
    throw pa_operation_pending("Operation has not completed yet");
  }
}

void PaOperation::ping_state(){
  if (state == OperationState::PENDING){
    if(pa_operation_get_state(*internal_operation) == PA_OPERATION_DONE){
      pa_operation_unref(*internal_operation);
      internal_operation.reset();
      set_state(OperationState::DONE);
    }
  }
}

void PaOperation::add_callback(std::unique_ptr<std::function<void(PaOperation*)>> cb) {
  this->callbacks.push_back(std::move(cb));
}

void PaOperation::set_state(OperationState state) {
  if (this->state == state){
    return;
  }
  this->state = state;
  for(auto& cb: callbacks){
    cb->operator()(this);
  }
}

void pa_success_callback (pa_context *c, int success, void *userdata){
  PaOperation* operation = reinterpret_cast<PaOperation*>(userdata);
  // TODO: There is no error checking here, probably should add it
  operation->set_state(OperationState::DONE);
}
}}
