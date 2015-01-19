#ifndef CS6300_THREE_ADDRESS_INSTRUCTION_HPP
#define CS6300_THREE_ADDRESS_INSTRUCTION_HPP

#include <memory>
#include <iostream>

namespace cs6300
{
class ThreeAddressInstruction
{
public:
  enum Type
  {
    Add,
    AddValue,
    And,
    CallFunction,
    CopyArgument,
    Divide,
    IsEqual,
    IsGreater,
    IsGreaterEqual,
    IsLess,
    IsLessEqual,
    IsNotEqual,
    LoadLabel,
    LoadMemory,
    LoadMemoryOffset,
    LoadValue,
    Modulo,
    Multiply,
    Not,
    Or,
    ReadChar,
    ReadInt,
    RestoreFrame,
    Return,
    Stop,
    StoreFrame,
    StoreMemory,
    StoreParameter,
    Subtract,
    UnaryMinus,
    WriteBool,
    WriteChar,
    WriteInt,
    WriteStr
  };
  ThreeAddressInstruction(Type op, int dest, int src1, int src2);
  Type op;
  int dest;
  int src1;
  int src2;
};
std::ostream& operator<<(std::ostream&, ThreeAddressInstruction);

inline bool operator<(const ThreeAddressInstruction& lhs,
                      const ThreeAddressInstruction& rhs)
{
  int result1 = lhs.op;
  result1 = (result1 << 3) ^ lhs.src1;
  result1 = (result1 << 3) ^ lhs.src2;
  int result2 = rhs.op;
  result2 = (result2 << 3) ^ rhs.src1;
  result2 = (result2 << 3) ^ rhs.src2;
  return result1 < result2;
}
}

#endif
