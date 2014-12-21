#include "keybind.hh"

#include <X11/Xlib.h>
#include <iostream>
#include <cstdint>
#include <map>

namespace JB {

void test_bindkeys() {
  Window root;
  XEvent e;
  Display *dpy = XOpenDisplay(0);
  if (!dpy)return;
  root = DefaultRootWindow(dpy);

  uint32_t F1 = XKeysymToKeycode(dpy, XStringToKeysym("F12"));

  auto res = XGrabKey(dpy, F1, Mod1Mask | ShiftMask, root, False, GrabModeAsync, GrabModeAsync);

  for (; ;) {
    XNextEvent(dpy, &e);
    if (e.type == KeyPress) {
      if (e.xkey.keycode == F1) {
        std::cout << "Foo";
        break;
      }
    }
  }

  XUngrabKey(dpy, F1, 0, root);
}

class ButtonImpl{
private:
  unsigned long modifiers;
  unsigned long keycode;

public:
  ButtonImpl():
    ButtonImpl(0, 0){};
  ButtonImpl(unsigned long modifiers, unsigned long keycode):
    modifiers(modifiers), keycode(keycode){};

  static ButtonImpl from_display(const KeyPressInfo& kpinfo, Display* disp){
    unsigned long keycode =  XKeysymToKeycode(disp, XStringToKeysym(kpinfo.key.c_str()));
    return ButtonImpl(
      kpinfo.modifiers,
      keycode
    );
  }

  bool operator<(const ButtonImpl& ref) const {
    if (keycode<ref.keycode){
      return true;
    }
    return modifiers < ref.modifiers;
  }

  bool operator==(const ButtonImpl& ref) const {
    return (keycode == ref.keycode) && (modifiers == ref.modifiers);
  }
};

class KBImpl {
  std::map<unsigned long, ButtonImpl> error_map;
  std::map<ButtonImpl, std::unique_ptr<Callback>> cb_map;
  Display * display;
  const Window root_window;
  KeyPressInfo exit_on;

  static Display* OpenDisplay(){
    Display* res =  XOpenDisplay(0);
    if (res == nullptr){
      throw std::logic_error("Couldn't open display");
    }
    return res;
  }

public:

  KBImpl():
    display(OpenDisplay()),
    root_window(DefaultRootWindow(display)){}

  void bind(KeyPressInfo kbinfo, std::unique_ptr<Callback> cb) {
    ButtonImpl button = ButtonImpl::from_display(kbinfo, display);
    error_map[XNextRequest(display)] = button;
    auto key = XKeysymToKeycode(display, XStringToKeysym(kbinfo.key.c_str()));
    if(!XGrabKey(display, key, kbinfo.modifiers, root_window, True, GrabModeAsync, GrabModeAsync)) {
      throw std::logic_error("Couldn't grab key");
    }
    cb_map[button] = std::move(cb);
  }

  void exit_on_key(KeyPressInfo kbinfo){
    exit_on = kbinfo;
  }

  void scan_for_keys_in_local_thread(){
    XEvent e;
    for(;;) {
      XNextEvent(display, &e);
//      KeyPressInfo kpinfo = {e.xkey.keycode, e.xkey.state};

    }
  }
};


KeyBinder::KeyBinder() :
  internal(std::make_unique<KBImpl>()) {;}

KeyBinder::~KeyBinder() {
}

KeyBinder &KeyBinder::get() {
  static KeyBinder kb;
  return kb;
}

void KeyBinder::bind(KeyPressInfo kbinfo, std::unique_ptr<Callback> cb) {
  internal->bind(kbinfo, std::move(cb));
}

}
