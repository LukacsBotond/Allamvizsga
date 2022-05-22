#pragma once

#include "State.h"

class STATE;

class MACHINE
{
private:
  STATE *state;

public:
  MACHINE();
  ~MACHINE();

  bool check();
  std::map<std::string, double> calculate();
  void setState(STATE *state);
};

MACHINE::MACHINE()
{
  state = nullptr;
}

MACHINE::~MACHINE()
{
  delete state;
}

bool MACHINE::check()
{
  return state->check();
}

std::map<std::string, double> MACHINE::calculate()
{
  return state->calculate();
}

void MACHINE::setState(STATE *state)
{
  delete this->state;
  this->state = state;
}