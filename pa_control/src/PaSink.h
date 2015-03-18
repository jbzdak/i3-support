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

  PaSink(const pa_sink_info &info);

public:

  friend std::ostream &operator<<(std::ostream &stream, const PaSink &matrix);

  const std::string name, description;
  const uint32_t index;
  const bool muted;
  const pa_cvolume channel_volume;

  const pa_volume_t volume;
  const pa_sink_state_t state;
  const uint32_t volume_steps;


};

}}

#endif //_PA_CONTROL_PASINK_H_
