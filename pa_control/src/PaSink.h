//
// Created by jb on 3/18/15.
//

#ifndef _PA_CONTROL_PASINK_H_
#define _PA_CONTROL_PASINK_H_

#include <iostream>
#include <pulse/pulseaudio.h>

namespace jb {
namespace pa {

class PaSink {
private:

public:
  PaSink();
  PaSink(const pa_sink_info &info);

public:

  friend std::ostream & operator<<(std::ostream &stream, const PaSink &matrix);
  friend bool operator==(const PaSink&, const PaSink&);

  std::string name, description;
  uint32_t index;
  bool muted;
  pa_cvolume channel_volume;

  pa_volume_t volume;
  pa_sink_state_t state;
  uint32_t volume_steps;


};

}}

#endif //_PA_CONTROL_PASINK_H_
