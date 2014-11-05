#include "Function.hpp"

bool cs6300::FunctionSignature::operator==(const FunctionSignature& other)const
{
  if(name!=other.name) return false;
  if(returnType!=other.returnType) return false;
  if(args.size() != other.args.size()) return false;
  for(size_t i = 0;i<args.size();++i)
  {
    if(args[i].second != other.args[i].second) return false;
  }

  return true;
}

bool cs6300::FunctionSignature::operator<(const FunctionSignature& other)const
{
  if(name<other.name) return true;
  if(name>other.name) return false;

  if(returnType<other.returnType) return true;
  if(returnType>other.returnType) return false;

  if(args.size() < other.args.size()) return true;
  if(args.size() > other.args.size()) return false;
  
  for(size_t i = 0;i<args.size();++i)
  {
    if(args[i].first < other.args[i].first) return true;
    if(args[i].first > other.args[i].first) return false;
    
    if(args[i].second < other.args[i].second) return true;
    if(args[i].second > other.args[i].second) return false;
  }

  return false;
}

int cs6300::FunctionSignature::getLabel() const
{
  if (label == -1)
  {
    label = getNextLabel();
  }
  return label;
}

int cs6300::FunctionSignature::getNextLabel()
{
  static int curLabel = 0;
  return ++curLabel;
}
