#include "./include/Machine.h"

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