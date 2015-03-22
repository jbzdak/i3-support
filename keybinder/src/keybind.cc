#include "keybind.hh"

#include <X11/Xlib.h>
#include <iostream>
#include <cstdint>
#include <map>

std::ostream& operator<< (std::ostream& ostream, const jb::kb::KeyPressInfo& kpinfo){
  ostream << "KeyPress(" << kpinfo.key << ", " << kpinfo.modifiers << ");";
  return ostream;
}

namespace jb {
namespace kb {

class ButtonImpl {
private:
  unsigned long modifiers;
  unsigned long keycode;

public:
  ButtonImpl() :
    ButtonImpl(0, 0) {
  };

  ButtonImpl(unsigned long modifiers, unsigned long keycode) :
    modifiers(modifiers), keycode(keycode) {
  };

  static ButtonImpl from_keyevent(const XEvent &e) {
    return ButtonImpl(
      e.xkey.state,
      e.xkey.keycode
    );
  }

  static ButtonImpl from_display(const KeyPressInfo &kpinfo, Display *disp) {
    unsigned long keycode = XKeysymToKeycode(disp, XStringToKeysym(kpinfo.key.c_str()));
    return ButtonImpl(
      kpinfo.modifiers,
      keycode
    );
  }

  bool operator<(const ButtonImpl &ref) const {
    if (keycode < ref.keycode) {
      return true;
    }
    return modifiers < ref.modifiers;
  }

  bool operator==(const ButtonImpl &ref) const {
    return (keycode == ref.keycode) && (modifiers == ref.modifiers);
  }
};

class KBImpl {
  std::map<unsigned long, ButtonImpl> error_map;
  std::map<ButtonImpl, std::unique_ptr<Callback>> cb_map;
  Display *display;
  const Window root_window;
  ButtonImpl exit_on;

  static Display *OpenDisplay() {
    Display *res = XOpenDisplay(0);
    if (res == nullptr) {
      throw std::logic_error("Couldn't open display");
    }
    return res;
  }

public:

  KBImpl() :
    display(OpenDisplay()),
    root_window(DefaultRootWindow(display)) {
  }

  void bind(KeyPressInfo kbinfo, std::unique_ptr<Callback> cb) {
    ButtonImpl button = ButtonImpl::from_display(kbinfo, display);
    error_map[XNextRequest(display)] = button;
    auto key = XKeysymToKeycode(display, XStringToKeysym(kbinfo.key.c_str()));
    if (!XGrabKey(display, key, kbinfo.modifiers, root_window, True, GrabModeAsync, GrabModeAsync)) {
      throw std::logic_error("Couldn't grab key");
    }
    cb_map[button] = std::move(cb);
  }

  void exit_on_key(KeyPressInfo kbinfo) {
    exit_on = ButtonImpl::from_display(kbinfo, display);
  }

  void scan_for_keys_in_local_thread() {
    XEvent e;
    for (; ;) {
      XNextEvent(display, &e);
      if(e.type == KeyRelease){
        continue;
      }
      ButtonImpl button = ButtonImpl::from_keyevent(e);

      // TODO: Handle Events registered for ModifierAny
      // TODO: Ungrab all keys
      cb_map[button]->operator()();

      if (button == exit_on) {
        return;
      }
    }
  }
};


KeyBinder::KeyBinder() :
  internal(std::make_unique<KBImpl>()) {}

KeyBinder::~KeyBinder() {}

KeyBinder &KeyBinder::Get() {
  static KeyBinder kb;
  return kb;
}

void KeyBinder::Bind(KeyPressInfo kbinfo, std::unique_ptr<Callback> cb) {
  internal->bind(kbinfo, std::move(cb));
}

void KeyBinder::ExitOn(KeyPressInfo kpinfo) {
  internal->exit_on_key(kpinfo);
}

void KeyBinder::Run(bool async) {
  internal->scan_for_keys_in_local_thread();
}

void PrintKeyPressCallback::operator()() {
  std::cout << "Key pressed" << this->kpinfo << std::endl;
}

}
}
