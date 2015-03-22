//
// Created by jb on 3/15/15.
//

#ifndef _PA_CONTROL_MAINLOOP_H_
#define _PA_CONTROL_MAINLOOP_H_

#include <pulse/pulseaudio.h>
#include <memory>
#include <boost/optional.hpp>
#include <functional>
#include <set>


#include "MainLoopDetail.hxx"
#include "PaOperation.h"
#include "PaSink.h"

namespace jb {
namespace pa {

class PaOperation;

/**
* This class encapsulates most of the interaction with pulse audio server.
*/
class MainLoop {

  friend class PaOperation;

public:


  MainLoop(std::string context_name);

  /**
  * Connects to the server.
  *
  * wait_for_connection - if true function will block until connection is established
  * (or there is conection error -- in which case exception will be raised).
  * server - optional name passed to the server parameter of pa_context_connect.
  * flags - context flafs
  */
  void connect(
    bool wait_for_connection=false,
    boost::optional<std::string> server = boost::optional<std::string>(),
    pa_context_flags_t flags= PA_CONTEXT_NOFLAGS
  );

  /**
  * Runs single iteration of pulse audio main loop.
  *
  * block - if true this function will wait for events.
  */
  void run_loop_iteration(bool block=false);

  /**
  * Returns context state it is refreshed on each call to run_loop_iteration
  */
  pa_context_state_t get_context_state(){return context_state;}

  /**
  * Shedules operation execution
  */
  void schedule_operation(std::shared_ptr<IOperation> operation);

  /**
  * Blocks until all pending operations are processed.
  */
  void wait_for_all_pending_operations();

  boost::optional<PaSink> mute_sink(const PaSink& sink, bool muted, bool wait_for=false){
    return this->mute_sink(sink.index, muted, wait_for);
  }

  boost::optional<PaSink> mute_sink(
    uint32_t sink_index, bool muted, bool wait_for=false);


private:

  const std::string context_name;
  pa_context_state_t context_state;
  const std::unique_ptr<pa_mainloop, MainLoopDetail::DeletePaMainLoop> pa_ml;
  pa_mainloop_api* pa_mlapi; // No need to free this one, as per PA docs
  const std::unique_ptr<pa_context, MainLoopDetail::DeletePaContext> pa_ctx;
  std::set<std::shared_ptr<IOperation>> operations;

private:
  friend void MainLoopDetail::main_loop_state_callback(pa_context *, void *);
  void set_contenxt_state(pa_context_state_t state);
  void update_operations();


};



}}

#endif //_PA_CONTROL_MAINLOOP_H_
