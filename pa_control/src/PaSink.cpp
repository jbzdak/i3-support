//
// Created by jb on 3/18/15.
//

#include "PaSink.h"

namespace jb { namespace pa{


PaSink::PaSink(const pa_sink_info &info):
  name(info.name),
  description(info.description),
  index(info.index),
  muted(info.mute != 0),
  channel_volume(info.volume),
  volume(info.base_volume),
  state(info.state),
  volume_steps(info.n_volume_steps) {}

std::ostream& operator<< (std::ostream& stream, const PaSink&sink){
  stream << "Sink name: " << sink.name << ", description" << sink.description <<
    ", index: " << sink.index << ", volume: " << sink.volume << ", state: " << sink.state <<
    ", volume steps: " << sink.volume_steps;
  return stream;
}


}
}