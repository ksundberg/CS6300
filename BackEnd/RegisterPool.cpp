#include "RegisterPool.hpp"
#include "ProcessLog.hpp"

std::shared_ptr<RegisterPool> RegisterPool::m_instance;

RegisterPool::RegisterPool()
  : m_gp(new SpecialRegister("$gp")), m_fp(new SpecialRegister("$fp"))
{
  avail.push_back("$s0");
  avail.push_back("$s1");
  avail.push_back("$s2");
  avail.push_back("$s3");
  avail.push_back("$s4");
  avail.push_back("$s5");
  avail.push_back("$s6");
  avail.push_back("$s7");
  avail.push_back("$t0");
  avail.push_back("$t1");
  avail.push_back("$t2");
  avail.push_back("$t3");
  avail.push_back("$t4");
  avail.push_back("$t5");
  avail.push_back("$t6");
  avail.push_back("$t7");
  avail.push_back("$t8");
  avail.push_back("$t9");
}
std::shared_ptr<Register> RegisterPool::allocate()
{
  if (avail.empty())
  {
    ProcessLog::getInstance()->logError("Out of registers");
    return std::shared_ptr<Register>();
  }
  auto reg = avail.back();
  avail.pop_back();
  return std::shared_ptr<Register>(new AllocatedRegister(reg));
}

void RegisterPool::release(std::string reg)
{
  avail.push_back(reg);
}
