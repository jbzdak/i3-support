#include <iostream>
#include "src/keybind.hh"

using namespace JB;

int main() {

    KeyBinder& binder = KeyBinder::Get();
    KeyPressInfo info = {"XF86AudioRaiseVolume", 0};
    binder.Bind(info, std::make_unique<PrintKeyPressCallback>(info));
    binder.ExitOn(info);
    binder.Run(false);
    return 0;
}
