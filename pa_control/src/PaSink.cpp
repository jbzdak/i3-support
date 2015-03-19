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

bool operator==(const PaSink& s1, const PaSink& s2) {
  if (s1.index != s2.index){
    return false;
  }
  if (s1.name != s2.name){
    return false;
  }
  if (s1.muted != s2.muted){
    return false;
  }
  if (s1.volume != s2.volume){
    return false;
  }
  if (!pa_cvolume_equal(&s1.channel_volume, &s2.channel_volume)){
    return false;
  }
  return true;
}

PaSink::PaSink():
  name(), description(),
  index(-1), muted(true),
  channel_volume(), volume(-1),
  state(PA_SINK_UNLINKED),
  volume_steps(-1){};
}
}