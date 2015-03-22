//
// Created by jb on 3/19/15.
//

#ifndef _PA_CONTROL_SETVOLUMEOP_H_
#define _PA_CONTROL_SETVOLUMEOP_H_

#include <boost/optional.hpp>

#include "PaOperation.h"
#include "PaSink.h"
#include "MainLoop.h"

namespace jb {
namespace pa {


class JustSetVolume: public PaOperation{
public:
  JustSetVolume(uint32_t sink_index, const pa_cvolume desired_volume);
  JustSetVolume(const PaSink& sink, const pa_cvolume desired_volume);
  JustSetVolume(const PaSink& sink, double desired_volume_scale);


protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop *ml) override;

private:
  const uint32_t sink_index;
  const pa_cvolume new_volume;

};



}}


#endif //_PA_CONTROL_SETVOLUMEOP_H_
