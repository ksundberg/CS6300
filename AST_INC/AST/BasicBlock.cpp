#include "BasicBlock.hpp"
#include "SymbolTable.hpp"

#include <algorithm>
#include <iterator>

std::string cs6300::BasicBlock::getNextLabel(bool reset)
{
  static size_t blockNumber = 0;
  if (reset)
  {
    blockNumber = 0;
    return "";
  }
  return std::to_string(++blockNumber);
}

std::string cs6300::BasicBlock::getLabel()
{
  if (label.empty()) label = std::string("BB") + getNextLabel();
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
  case ThreeAddressInstruction::CallFunction
    : // CallFunction should have no allocation
  case ThreeAddressInstruction::StoreFrame:
  case ThreeAddressInstruction::RestoreFrame:
    break;
  case ThreeAddressInstruction::CopyArgument:
  case ThreeAddressInstruction::WriteStr: // none
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::LoadMemory:
  case ThreeAddressInstruction::LoadLabel:
    m.dead.insert(tal.dest);
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::LoadMemoryOffset:
  case ThreeAddressInstruction::LoadValue: // LoadValue has constants
  case ThreeAddressInstruction::LoadString
    : // LoadString uses a constant label value
    m.dead.insert(tal.dest);
    break;
  case ThreeAddressInstruction::StoreMemory
    : // special case store memory dest is src1
    m.used.insert(tal.dest);
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::AddValue:
    m.dead.insert(tal.dest);
    m.used.insert(tal.src1);
    break;
  case ThreeAddressInstruction::Comment:
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
    case ThreeAddressInstruction::
      CallFunction: // CallFunction should have no allocation
    case ThreeAddressInstruction::StoreFrame:
    case ThreeAddressInstruction::RestoreFrame:
      break;
    case ThreeAddressInstruction::LoadMemoryOffset:
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];
      break;
    case ThreeAddressInstruction::LoadValue: // LoadValue has constants
    case ThreeAddressInstruction::LoadString: // LoadString uses a constant label value
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];
      break;
    case ThreeAddressInstruction::CopyArgument:
    case ThreeAddressInstruction::WriteStr: // none
      if (i.src1 && m.count(i.src1)) i.src1 = m[i.src1];
      break;
    case ThreeAddressInstruction::StoreMemory:
    case ThreeAddressInstruction::StoreParameter:
    case ThreeAddressInstruction::AddValue:
    case ThreeAddressInstruction::LoadLabel:
    case ThreeAddressInstruction::LoadMemory:
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];
      if (i.src1 && m.count(i.src1)) i.src1 = m[i.src1];
      break;
    case ThreeAddressInstruction::Comment:
      break;
    default:
      if (i.dest && m.count(i.dest)) i.dest = m[i.dest];

      if (i.src1 && m.count(i.src1)) i.src1 = m[i.src1];

      if (i.src2 && m.count(i.src2)) i.src2 = m[i.src2];
    }
  }
  if (branchTo && m.count(branchOn))
  {
    branchOn = m[branchOn];
  }
}
