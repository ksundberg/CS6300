#ifndef REGISTERPOOL_H
#define REGISTERPOOL_H
#include <memory>
#include <string>
#include <iostream>
#include <vector>

class Register;

class RegisterPool
{
public:
  static std::shared_ptr<RegisterPool> getInstance()
  {
    if (!m_instance)
      m_instance = std::shared_ptr<RegisterPool>(new RegisterPool());
    return m_instance;
  }
  std::shared_ptr<Register> allocate();
  std::shared_ptr<Register> getGp() { return m_gp; }
  std::shared_ptr<Register> getFp() { return m_fp; }
  void release(std::string);

private:
  RegisterPool();
  std::vector<std::string> avail;
  std::shared_ptr<Register> m_gp;
  std::shared_ptr<Register> m_fp;
  static std::shared_ptr<RegisterPool> m_instance;
};

class Register
{
public:
  virtual ~Register() {}

protected:
  Register(std::string name) : m_name(name) {}

  std::string m_name;
  friend std::ostream& operator<<(std::ostream& out,
                                  std::shared_ptr<Register> r);
  friend class RegisterPool;
};

class AllocatedRegister : public Register
{
public:
  virtual ~AllocatedRegister()
  {
    auto rp = RegisterPool::getInstance();
    if (rp) rp->release(m_name);
  }

private:
  AllocatedRegister(std::string name) : Register(name) {}
  friend class RegisterPool;
};
class SpecialRegister : public Register
{
public:
  virtual ~SpecialRegister() {}

private:
  SpecialRegister(std::string name) : Register(name) {}
  friend class RegisterPool;
};

inline std::ostream& operator<<(std::ostream& out, std::shared_ptr<Register> r)
{
  if (r)
  {
    out << r->m_name;
  }
  return out;
};

#endif
