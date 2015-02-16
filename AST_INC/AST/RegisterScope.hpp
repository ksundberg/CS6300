#ifndef CS6300_REGISTER_SCOPE_HPP
#define CS6300_REGISTER_SCOPE_HPP
namespace cs6300
{

struct RegisterScope
{
  std::set<int> dead;
  std::set<int> used;
  std::set<int> alive;

  void clear()
  {
    dead.clear();
    used.clear();
    alive.clear();
  }
};
}
#endif
