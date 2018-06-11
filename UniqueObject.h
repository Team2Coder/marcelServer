#include "Singleton.h"
class UniqueObject : public Singleton<UniqueObject>
{
  friend class Singleton<UniqueObject>;

private:
  // Ctor/Dtor
  UniqueObject ()
    : _value (0) { }
  ~UniqueObject () { }

public:
  //Public interface for Singleton
  void setValue (int val) { _value = val; }
  int getValue () { return _value; }

private:
  //Member var
  int _value;
};

