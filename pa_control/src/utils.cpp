//
// Created by jb on 3/21/15.
//

#include "utils.h"

pa_cvolume jb::pa::scale_volumes(const pa_cvolume& original_vol, double new_volume){
  pa_cvolume new_vol = original_vol;

  for(uint32_t ii=0; ii < new_vol.channels; ii++){
    auto linear = pa_sw_volume_to_linear(new_vol.values[ii]);
    new_vol.values[ii] = pa_sw_volume_from_linear(linear*new_volume);
  }

  return new_vol;
}