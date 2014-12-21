#include <stdint.h>
#include <string>
#include <memory>
#include <X11/X.h>

namespace JB {

class KBImpl;

void test_bindkeys();

struct Callback {
  virtual ~Callback() = 0;
  virtual void operator()() = 0;
};

struct KeyPressInfo {

  friend class KBimpl;

  std::string key;
  uint16_t modifiers;

};

class KeyBinder {
private:
  std::unique_ptr<KBImpl> internal;
public:
  KeyBinder();

  ~KeyBinder();

  void bind(KeyPressInfo kpinfo, std::unique_ptr<Callback> cb);

public:
  static KeyBinder &get();

};

}