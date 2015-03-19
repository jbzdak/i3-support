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
  JustSetVolume(const PaSink sink, const pa_volume_t desired_volume);
  JustSetVolume(const PaSink sink, const pa_cvolume desired_volume);


protected:
  virtual pa_operation *execute_operation_internal(pa_context *ctx, MainLoop *ml) override;

private:
  const PaSink sink;
  const pa_cvolume new_volume;

};

class SetVolumeOp : public CompositeOperation{

public:

  SetVolumeOp(uint32_t sink_idx, pa_volume_t desired_volume);
  SetVolumeOp(PaSink sink, pa_volume_t desired_volume);


protected:
  virtual std::shared_ptr<IOperation> get_next_operation(std::shared_ptr<IOperation> last) override;

private:
  const uint32_t sink_idx;
  const boost::optional<PaSink> sink;
  const pa_volume_t desired_volume;



};

}}


#endif //_PA_CONTROL_SETVOLUMEOP_H_
