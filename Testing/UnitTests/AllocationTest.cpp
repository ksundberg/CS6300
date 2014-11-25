#include "gtest/gtest.h"
#include "../BackEnd/Allocation.hpp"
#include "../AST_INC/AST/BasicBlock.hpp"


TEST(AllocationTest, validateSets)
{
    auto emptyBlock = std::make_shared<cs6300::BasicBlock>();
    auto b0 = std::make_shared<cs6300::BasicBlock>();
    auto b1 = std::make_shared<cs6300::BasicBlock>();
    auto b2 = std::make_shared<cs6300::BasicBlock>();
    auto b3 = std::make_shared<cs6300::BasicBlock>();
    auto b4 = std::make_shared<cs6300::BasicBlock>();
    auto b5 = std::make_shared<cs6300::BasicBlock>();
    auto b6 = std::make_shared<cs6300::BasicBlock>();

    emptyBlock->jumpTo = b0;
    b0->jumpTo = b1;
    b1->jumpTo = b2;
    b1->branchTo = b6;
    b2->jumpTo = b3;
    b3->jumpTo = b4;
    b3->branchTo = b5;
    b4->jumpTo = b5;
    b5->jumpTo = b6;
    b5->branchTo = b1;

    b0->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::LoadValue,1,0,0));

    b1->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::LoadValue, 2, 1, 0));
    b1->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::LoadValue, 3, 100, 0));
    b1->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::IsLess, 4, 2, 3));

    b2->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::LoadValue, 5, 5, 0));
    b2->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::Modulo, 7, 2, 5));
    b2->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::StoreMemory, 7, 2, 0));

    b3->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::WriteStr, 7, 0, 0));

    b4->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::WriteStr, 7, 0, 0));

    b5->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::LoadValue, 9, 1, 0));
    b5->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::LoadValue, 10, 1, 0));
    b5->instructions.push_back(cs6300::ThreeAddressInstruction(cs6300::ThreeAddressInstruction::Add, 11, 9, 10));

    auto blocks = std::pair<std::shared_ptr<cs6300::BasicBlock>,
            std::shared_ptr<cs6300::BasicBlock>>(b0, b5);

    cs6300::locRegAlloc(blocks);

    //Validate Dead set

    EXPECT_EQ(b0->m.dead.size(), 1);
    EXPECT_EQ(b1->m.dead.size(), 3);
    EXPECT_EQ(b2->m.dead.size(), 2);
    EXPECT_EQ(b3->m.dead.size(), 0);
    EXPECT_EQ(b4->m.dead.size(), 0);
    EXPECT_EQ(b5->m.dead.size(), 3);


    EXPECT_EQ(*(b0->m.dead.find(1)), 1);

    EXPECT_EQ(*(b1->m.dead.find(2)), 2);
    EXPECT_EQ(*(b1->m.dead.find(3)), 3);
    EXPECT_EQ(*(b1->m.dead.find(4)), 4);

    EXPECT_EQ(*(b2->m.dead.find(5)), 5);
    EXPECT_EQ(*(b2->m.dead.find(7)), 7);

    EXPECT_EQ(*(b5->m.dead.find(9)), 9);
    EXPECT_EQ(*(b5->m.dead.find(10)), 10);
    EXPECT_EQ(*(b5->m.dead.find(11)), 11);

    //Validate Used set

    EXPECT_EQ(b0->m.used.size(), 0);
    EXPECT_EQ(b1->m.used.size(), 2);
    EXPECT_EQ(b2->m.used.size(), 3);
    EXPECT_EQ(b3->m.used.size(), 0);
    EXPECT_EQ(b4->m.used.size(), 0);
    EXPECT_EQ(b5->m.used.size(), 2);

    EXPECT_EQ(*(b1->m.used.find(2)), 2);
    EXPECT_EQ(*(b1->m.used.find(3)), 3);

    EXPECT_EQ(*(b2->m.used.find(2)), 2);
    EXPECT_EQ(*(b2->m.used.find(5)), 5);
    EXPECT_EQ(*(b2->m.used.find(7)), 7);

    EXPECT_EQ(*(b5->m.used.find(9)), 9);
    EXPECT_EQ(*(b5->m.used.find(10)), 10);

    //Validate Alive set

    EXPECT_EQ(b0->m.alive.size(), 0);
    EXPECT_EQ(b1->m.alive.size(), 0);
    EXPECT_EQ(b2->m.alive.size(), 1);
    EXPECT_EQ(b3->m.alive.size(), 0);
    EXPECT_EQ(b4->m.alive.size(), 0);
    EXPECT_EQ(b5->m.alive.size(), 0);

    EXPECT_EQ(*(b2->m.alive.find(2)), 2);
}
