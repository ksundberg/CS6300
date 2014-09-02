#include "ThreeAddressInstruction.hpp"

cs6300::ThreeAddressInstruction::ThreeAddressInstruction(Type t,
                                                         int d,
                                                         int s1,
                                                         int s2)
    : op(t)
    , dest(d)
    , src1(s1)
    , src2(s2)
{
}
