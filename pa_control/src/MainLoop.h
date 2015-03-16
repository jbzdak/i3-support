//
// Created by jb on 3/15/15.
//

#ifndef _PA_CONTROL_MAINLOOP_H_
#define _PA_CONTROL_MAINLOOP_H_

#include <pulse/pulseaudio.h>
#include <bits/unique_ptr.h>
#include <functional>


namespace jb {
namespace pa {

namespace MainLoopDetail{

class DeletePaMainLoop{
public:

  void operator()(pa_mainloop* ml){
    pa_mainloop_free(ml);
  }
};

class DeletePaContext{
public:

  void operator()(pa_context* ctx){
    pa_context_disconnect(ctx);
    pa_context_unref(ctx);
  }

};

}

class MainLoop {

public:

  MainLoop(std::string context_name):
    context_name(context_name),
    pa_ml(pa_mainloop_new()),
    pa_mlapi(pa_mainloop_get_api(
      pa_ml.get())
    ),
    pa_ctx(pa_context_new(pa_mlapi.get(), this->context_name.c_str())){

  }

private:


  const std::string context_name;
  pa_context_state_t context_state;

  /**
  *
  */
  std::unique_ptr<pa_mainloop, MainLoopDetail::DeletePaMainLoop> pa_ml;
  std::unique_ptr<pa_mainloop_api> pa_mlapi;
  std::unique_ptr<pa_context, MainLoopDetail::DeletePaContext> pa_ctx;

};

}}

#endif //_PA_CONTROL_MAINLOOP_H_
