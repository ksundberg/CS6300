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
      LoadValue,
      Modulo,
      Multiply,
      Not,
      Or,
      ReadChar,
      ReadInt,
      Stop,
      StoreMemory,
      Subtract,
      UnaryMinus,
      WriteBool,
      WriteChar,
      WriteInt,
      WriteStr
    };
    ThreeAddressInstruction(Type op,int dest,int src1,int src2);
    Type op;
    int dest;
    int src1;
    int src2;
};
std::ostream& operator<<(std::ostream&,ThreeAddressInstruction);

}
#endif
