//
// Created by jb on 3/16/15.
//

#include "PaOperation.h"

namespace jb {
namespace pa {


void PaOperation::execute_operation(MainLoop& ml){
  internal_operation = execute_operation_internal(ml.pa_ml.get());
  state = OperationState::PENDING;
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
    }
  }
}
}}