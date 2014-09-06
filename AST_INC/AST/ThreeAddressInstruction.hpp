#ifndef CS6300_THREE_ADDRESS_INSTRUCTION_HPP
#define CS6300_THREE_ADDRESS_INSTRUCTION_HPP

#include <memory>

namespace cs6300
{
class ThreeAddressInstruction
{
  public:
    enum Type
    {
      Add,
      Multiply,
      Or,
      Subtract,
      And,
      Divide,
      IsEqual,
      IsGreater,
      IsGreaterEqual,
      IsLess,
      IsLessEqual,
      IsNotEqual,
      Modulo,
      ReadChar,
      ReadInt,
      Stop,
      WriteBool,
      WriteChar,
      WriteInt,
      WriteStr
    };
    ThreeAddressInstruction(Type op,int dest,int src1,int src2);
  private:
    Type op;
    int dest;
    int src1;
    int src2;
};
}
#endif
