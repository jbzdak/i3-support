//
// Created by jb on 3/16/15.
//

#include "PaOperation.h"
#include "MainLoop.h"
#include <iostream>
#include <sstream>
namespace jb {
namespace pa {

IOperation::IOperation(const std::string& name):
  name(name){};

PaOperation::PaOperation(const std::string& name):
  IOperation(name){};

CompositeOperation::CompositeOperation(std::string name):
  IOperation(name){}


void PaOperation::execute_operation(MainLoop* ml){
  auto pa_op =  execute_operation_internal(ml->pa_ctx.get(), ml);
  if(pa_op == nullptr){
    std::stringstream err;
    err << "Error in operation \"" << this->get_name() << "\".";
    err << "pulse_audio returned null as a pa_operation* ";
    err << "which (as far as I can guess is an error condition)";
    err << std::endl;
    auto str = err.str();
    throw std::logic_error(str.c_str());
  }
  internal_operation = pa_op;
  set_state(OperationState::PENDING);
}

PaOperation::~PaOperation() {
  if (this->internal_operation){
    pa_operation_unref(*this->internal_operation);
  }
}

void PaOperation::get_result_guard(){
  if (get_state() != OperationState::DONE){
    throw pa_operation_pending("Operation has not completed yet");
  }
}

void PaOperation::ping_state(MainLoop *ml){
  if (get_state() == OperationState::PENDING){
    if (!internal_operation){
      throw std::logic_error("");
    }
    if(pa_operation_get_state(*internal_operation) == PA_OPERATION_DONE){
      pa_operation_unref(*internal_operation);
      internal_operation.reset();
      set_state(OperationState::DONE);
    }
  }
}

void IOperation::add_callback(std::unique_ptr<std::function<void(IOperation*)>> cb) {
  this->callbacks.push_back(std::move(cb));
}

void IOperation::set_state(OperationState state) {
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


void CompositeOperation::execute_operation(MainLoop *ml) {
  last = get_next_operation(nullptr);
  ml->schedule_operation(last);
}

void CompositeOperation::ping_state(MainLoop *ml) {
  if(last->is_done()){
    auto next = get_next_operation(last);
    if (next){
      ml->schedule_operation(next);
      last = next;
    }else{
      set_state(OperationState::DONE);
    }
  }
}

std::shared_ptr<IOperation> CompositeOperation::get_next_operation(std::shared_ptr<IOperation> last) {
  return std::shared_ptr<IOperation>();
}


}}
