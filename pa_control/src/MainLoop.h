//
// Created by jb on 3/15/15.
//

#ifndef _PA_CONTROL_MAINLOOP_H_
#define _PA_CONTROL_MAINLOOP_H_

#include <pulse/pulseaudio.h>
#include <memory>
#include <boost/optional.hpp>
#include <functional>


#include "MainLoopDetail.hxx"


namespace jb {
namespace pa {

class IPaOperation;

class MainLoop {

  friend class IPaOperation;

public:

  MainLoop(std::string context_name);

  void connect(
    bool wait_for_connection=false,
    boost::optional<std::string> server = boost::optional<std::string>(),
    pa_context_flags_t flags= PA_CONTEXT_NOFLAGS
  );

  void run_loop_iteration(bool block=false);

  pa_context_state_t get_context_state(){return context_state;}

private:
  friend void MainLoopDetail::main_loop_state_callback(pa_context *, void *);
  void set_contenxt_state(pa_context_state_t state);

private:

  const std::string context_name;
  pa_context_state_t context_state;
  const std::unique_ptr<pa_mainloop, MainLoopDetail::DeletePaMainLoop> pa_ml;
  pa_mainloop_api* pa_mlapi; // No need to free this one, as per PA docs
  const std::unique_ptr<pa_context, MainLoopDetail::DeletePaContext> pa_ctx;


};



}}

#endif //_PA_CONTROL_MAINLOOP_H_
