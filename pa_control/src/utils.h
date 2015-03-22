//
// Created by jb on 3/21/15.
//

#ifndef _PA_CONTROL_UTILS_H_
#define _PA_CONTROL_UTILS_H_

#include <pulse/pulseaudio.h>

namespace jb {
namespace pa {

/**
* This function scales volume, so each channel is linearly scaled by
* new_volume.
*/
pa_cvolume scale_volumes(pa_cvolume& original_vol, double new_volume);

}}

#endif //_PA_CONTROL_UTILS_H_
