#include "BasicBlock.hpp"
#include "SymbolTable.hpp"

#include <algorithm>
#include <iterator>

std::string cs6300::BasicBlock::getLabel()
{

  static size_t blockNumber = 0;
  if (label.empty()) label = std::string("BB") + std::to_string(++blockNumber);
  return label;
}

void cs6300::BasicBlock::initSets()
{
  m.clear();
  for (auto i : instructions)
  {
    auto t = scope(i);
    m.used.insert(t.used.begin(), t.used.end());
    m.dead.insert(t.dead.begin(), t.dead.end());
  }

  if (branchOn) m.dead.insert(branchOn);

  std::vector<int> t;
  std::set_difference(m.used.begin(),
                      m.used.end(),
                      m.dead.begin(),
                      m.dead.end(),
                      std::back_inserter(t));
  m.alive = std::set<int>(t.begin(), t.end());
}

cs6300::RegisterScope cs6300::BasicBlock::scope(
  cs6300::ThreeAddressInstruction tal)
{
  cs6300::RegisterScope m;
  switch (tal.op)
  {
  case ThreeAddressInstruction::LoadMemory:
    m.dead.insert(tal.dest);
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::LoadMemoryOffset:
  case ThreeAddressInstruction::LoadValue: // LoadValue has constants
    m.dead.insert(tal.dest);
    break;
  case ThreeAddressInstruction::
    StoreMemory: // special case store memory dest is src1
    m.used.insert(tal.dest);
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::AddValue:
    m.dead.insert(tal.dest);
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::WriteStr: // none
    break;
  default:
    if (tal.dest) m.dead.insert(tal.dest);
    if (tal.src1) m.used.insert(tal.src1);
    if (tal.src2) m.used.insert(tal.src2);
  }

  return m;
}

void cs6300::BasicBlock::remap(std::map<int, int> m)
{
  for (auto& i : instructions)
  {
    switch (i.op)
    {
    case ThreeAddressInstruction::LoadMemoryOffset:
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];
      break;
    case ThreeAddressInstruction::LoadValue: // LoadValue has constants
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];
      break;
    case ThreeAddressInstruction::StoreMemory:
    case ThreeAddressInstruction::AddValue:
    case ThreeAddressInstruction::LoadMemory:
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];
      if (i.src1 && m.count(i.src1)) i.src1 = m[i.src1];
      break;
    case ThreeAddressInstruction::WriteStr: // none
      break;
    default:
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];

      if (i.src1 && m.count(i.src1)) i.src1 = m[i.src1];

      if (i.src2 && m.count(i.src2)) i.src2 = m[i.src2];
    }
  }
}
