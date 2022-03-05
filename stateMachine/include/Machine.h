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

void MACHINE::calculate(){

  //TODO IMPLEMENT
  return;
}

void MACHINE::setState(STATE *state)
{
  this->state = state;
}