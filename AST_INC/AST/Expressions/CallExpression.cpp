#include "CallExpression.hpp"

cs6300::CallExpression::CallExpression(
    std::string n, std::vector<std::shared_ptr<Expression>> args)
    : Expression()
    , name(n)
    , actualArguments(args)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::CallExpression::emit() const
{
  return nullptr;
}

std::shared_ptr<cs6300::Type> cs6300::CallExpression::type() const
{
  return nullptr;
}

int cs6300::CallExpression::value() const { return 0; }

bool cs6300::CallExpression::isConst() const { return false; }

