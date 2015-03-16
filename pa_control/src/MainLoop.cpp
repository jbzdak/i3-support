//
// Created by jb on 3/15/15.
//

#include "MainLoop.h"
#include "pa_errors.h"

namespace jb {
namespace pa {

namespace MainLoopDetail {

void main_loop_state_callback(pa_context *c, void *userdata) {
  MainLoop* loop = reinterpret_cast<MainLoop*>(userdata);
  loop->set_contenxt_state(pa_context_get_state(c));
}

}

MainLoop::MainLoop(std::string context_name):
  context_name(context_name),
  pa_ml(pa_mainloop_new()),
  pa_mlapi(pa_mainloop_get_api(
    pa_ml.get())
  ),
  pa_ctx(pa_context_new(pa_mlapi, this->context_name.c_str())),
  context_state(PA_CONTEXT_UNCONNECTED){

    pa_context_set_state_callback(
      pa_ctx.get(),
      MainLoopDetail::main_loop_state_callback,
      reinterpret_cast<void*>(this));

}


void MainLoop::connect(bool wait_for_connection,
  boost::optional<std::string> server_name,
  pa_context_flags_t flags){
  const char * server_name_cstr = nullptr;
  if (server_name){
    server_name_cstr = server_name->c_str();
  }
  pa_context_connect(
    pa_ctx.get(),
    server_name_cstr,
    flags,
    nullptr
  );

  if (! wait_for_connection){
    return;
  }

  while(true){
    this->run_loop_iteration(true);
    switch(context_state){
      default:
        break;
      case PA_CONTEXT_READY:
        return;
      case PA_CONTEXT_FAILED:
      case PA_CONTEXT_TERMINATED:
        throw pa_no_connection("Couldn't obtain context connection");
    }
  }

}


void MainLoop::run_loop_iteration(bool block){
  // I'm not shure whether we should check result of this function
  // or ignore it. pa_mainloop_iterate returns negative value either
  // on quit() request or on error.
  // There is no other information on errors, and I don't know how
  // should I handle these errors.
  pa_mainloop_iterate(pa_ml.get(), block, nullptr);
}

void MainLoop::set_contenxt_state(pa_context_state_t state){
  this->context_state = state;
}

}
}