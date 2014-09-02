#include "Expression.hpp"

int cs6300::Expression::getLabel()
{
  if (label == -1)
  {
    label = getNextLabel();
  }
  return label;
}

int cs6300::Expression::getNextLabel()
{
  static int curLabel = 0;
  return ++curLabel;
}

