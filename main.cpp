
#include "keybinder/src/keybind.hh"
#include "pa_control/src/MainLoop.h"
#include "pa_control/src/GetSinkListOp.h"
#include "pa_control/src/SetVolumeOp.h"

void set_volume_to(jb::pa::MainLoop& main_loop, double volume){
  auto sink_list_op = std::make_shared<jb::pa::GetSinkListOp>();
  main_loop.schedule_operation(sink_list_op);
  main_loop.wait_for_all_pending_operations();
  for(jb::pa::PaSink sink: sink_list_op->get_sink_list()){
    if(sink.state == PA_SINK_RUNNING){
      pa_cvolume new_volume = sink.channel_volume;
      for(uint32_t ii = 0; ii < new_volume.channels; ii++){
        new_volume.values[ii] = pa_sw_volume_from_linear(volume);
      }
      main_loop.schedule_operation(
        std::make_shared<jb::pa::JustSetVolume>(sink, new_volume)
      );
    }
  }
}

struct VolumeIncrement: public jb::kb::Callback{
  jb::pa::MainLoop& main_loop;
  double& current_volume;
  const double volume_increment;

  VolumeIncrement(jb::pa::MainLoop& main_loop, double& current_volume, double volume_increment):
    main_loop(main_loop), current_volume(current_volume), volume_increment(volume_increment){};

  void operator()(){
    current_volume+=volume_increment;
    if (current_volume < pa_sw_volume_to_linear(PA_VOLUME_MUTED)){
      current_volume = pa_sw_volume_to_linear(PA_VOLUME_MUTED);
    }else if (current_volume > pa_sw_volume_to_linear(PA_VOLUME_MAX)){
      current_volume = pa_sw_volume_to_linear(PA_VOLUME_MAX);
    }
    std::cout << "Set volume to" << current_volume << std::endl;
    set_volume_to(main_loop, current_volume);
  }
};

int main() {
  double current_volume=pa_sw_volume_to_linear(PA_VOLUME_NORM);
  jb::pa::MainLoop main_loop("foo");
  main_loop.connect(true);
  set_volume_to(main_loop, current_volume);
  jb::kb::KeyBinder& binder = jb::kb::KeyBinder::Get();

  binder.Bind({"XF86AudioLowerVolume", 0}, std::make_unique<VolumeIncrement>(main_loop, current_volume, -.1));
  binder.Bind({"XF86AudioRaiseVolume", 0}, std::make_unique<VolumeIncrement>(main_loop, current_volume, .1));

  binder.Run(false);

  return 0;
}