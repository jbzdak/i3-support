//
// Created by jb on 3/16/15.
//

#ifndef _PA_CONTROL_PAOPERATION_H_
#define _PA_CONTROL_PAOPERATION_H_

#include <memory>

#include <pulse/pulseaudio.h>
#include <boost/optional.hpp>

#include "pa_errors.h"
#include <iostream>


namespace jb {
namespace pa {

class MainLoop;

enum class OperationState {
  UNSCHEDULED,
  PENDING,
  DONE
};

/*
* This needs PaOperation to be passed as a userdata.
*/
void pa_success_callback (pa_context *c, int success, void *userdata);

class IOperation{
public:
  IOperation(const std::string& name);
  virtual ~IOperation()=default;
  virtual void execute_operation(MainLoop* ml)=0;
  virtual OperationState get_state() {return state;}
  virtual bool is_done() {return state == OperationState::DONE;}
  virtual const std::string& get_name(){return name;}

  /**
  * We take ownership of pointed callback.
  *
  * Callbacks will be called whether state of this instance changes,
  * parameter to the callback will be PaOperation pointed by this pointer.
  */
  virtual void add_callback(std::unique_ptr<std::function<void(IOperation*)>> cb);

protected:
  friend class MainLoop;
  friend void pa_success_callback(pa_context*, int, void*);
  virtual void ping_state(MainLoop *ml)=0;
  virtual void set_state(OperationState state);
private:
  const std::string name;
  OperationState state = OperationState::UNSCHEDULED;
  std::vector<std::unique_ptr<std::function<void(IOperation*)>>> callbacks;

};

class PaOperation : public IOperation{
public:
  PaOperation(const std::string& name);
  virtual void execute_operation(MainLoop* ml);
  virtual ~PaOperation();

protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop* ml) = 0;

  void get_result_guard();
  virtual void ping_state(MainLoop *ml);

private:
  boost::optional<pa_operation*> internal_operation;
};


class CompositeOperation: public IOperation{
public:
  CompositeOperation(std::string name);
  virtual void execute_operation(MainLoop* ml);
  virtual void ping_state(MainLoop *ml);

  virtual ~CompositeOperation() = default;

protected:
  virtual std::shared_ptr<IOperation> get_next_operation(std::shared_ptr<IOperation> last)=0;

private:
  std::shared_ptr<IOperation> last;
};
}}


#endif //_PA_CONTROL_PAOPERATION_H_
