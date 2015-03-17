#include "MainLoop.h"

namespace jb {
namespace pa {
namespace MainLoopDetail {

class DeletePaMainLoop {
public:

  void operator()(pa_mainloop *ml) {
    pa_mainloop_free(ml);
  }
};

class DeletePaContext {
public:

  void operator()(pa_context *ctx) {
    pa_context_disconnect(ctx);
    pa_context_unref(ctx);
  }

};

void main_loop_state_callback(pa_context *, void *);

}}}