#pragma once

#include "State.h"
#include <map>
#include <string>

class STATE;

class MACHINE
{
private:
  STATE *state;

public:
  MACHINE();
  ~MACHINE();

  bool check();
  void calculate();
  void setState(STATE *state);
  std::map<std::string, double> getResult();
  std::string getMainResult();
};
