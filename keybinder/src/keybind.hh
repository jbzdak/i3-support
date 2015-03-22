#include <stdint.h>
#include <string>
#include <memory>
#include <X11/X.h>

namespace JB {

class KBImpl;

struct Callback {
  virtual ~Callback(){};
  virtual void operator()() = 0;
};

struct KeyPressInfo {

  friend class KBimpl;

  std::string key;
  uint16_t modifiers;

  KeyPressInfo(std::string key, uint16_t modifiers):
    key(key), modifiers(modifiers){;}

};

struct PrintKeyPressCallback : public Callback{
  KeyPressInfo kpinfo;
  PrintKeyPressCallback(KeyPressInfo kpinfo): kpinfo(kpinfo){;};
  virtual ~PrintKeyPressCallback(){};
  virtual void operator()();
};


class KeyBinder {
private:
  std::unique_ptr<KBImpl> internal;
  KeyBinder();
  ~KeyBinder();
public:

  void Bind(KeyPressInfo kpinfo, std::unique_ptr<Callback> cb);

  void Bind(KeyPressInfo kpinfo, std::unique_ptr<std::function<void()>> cb);

  void ExitOn(KeyPressInfo kpinfo);

  void Run(bool async=false);

public:
  static KeyBinder& Get();

};

}