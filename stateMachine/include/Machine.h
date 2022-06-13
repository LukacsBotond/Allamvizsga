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
  void calculate();
  void setState(STATE *state);
  std::map<std::string, double> getResult();
  std::string getMainResult();
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

void MACHINE::calculate()
{
  state->calculate();
}

void MACHINE::setState(STATE *state)
{
  delete this->state;
  this->state = state;
}

std::map<std::string, double> MACHINE::getResult()
{
  return state->getResults();
}

std::string MACHINE::getMainResult()
{
  return state->mainResult;
}