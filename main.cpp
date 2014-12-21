#include <iostream>
#include "src/keybind.hh"

using namespace JB;

int main() {

    KeyBinder& binder = KeyBinder::Get();
    KeyPressInfo info = {"F5", ShiftMask|Mod1Mask};
    binder.Bind(info, std::make_unique<PrintKeyPressCallback>(info));
    binder.ExitOn(info);
    binder.Run(false);
    return 0;
}
