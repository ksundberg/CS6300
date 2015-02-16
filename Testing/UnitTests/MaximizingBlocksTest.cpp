#include "catch.hpp"
#include "Optimizations/MaximizeBlocks/MaximizeBlocks.hpp"
#include "Optimizations/MaximizeBlocks/VisitedBlocks.hpp"
#include "Optimizations/MaximizeBlocks/NumParents.hpp"

void singletonCleanUp()
{
  // Clean up after singleton
  auto vb = cs6300::VisitedBlocks::instance();
  auto np = cs6300::NumParents::instance();
  np->reset();
  vb->reset();
}

TEST_CASE("Complex", "[maximizeBlocks]")
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

  b0->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 1, 0, 0));

  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 2, 1, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 3, 100, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::IsLess, 4, 2, 3));

  b2->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 5, 5, 0));
  b2->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::Modulo, 7, 2, 5));
  b2->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::StoreMemory, 7, 2, 0));

  b3->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::WriteStr, 7, 0, 0));

  b4->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::WriteStr, 7, 0, 0));

  b5->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 9, 1, 0));
  b5->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 10, 1, 0));
  b5->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::Add, 11, 9, 10));
  b5->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::StoreMemory, 11, 1, 0));

  auto blocks = cs6300::FlowGraph(b0, b6);

  cs6300::maximizeBlocks(blocks);

  REQUIRE(4 == b2->instructions.size());
  REQUIRE(b4 == b2->jumpTo);

  // Check order, jumpTos, and branches
  REQUIRE(b1 == b0->jumpTo);
  REQUIRE(b2 == b1->jumpTo);
  REQUIRE(b6 == b1->branchTo);
  REQUIRE(b4 == b2->jumpTo);
  REQUIRE(b5 == b2->branchTo);
  REQUIRE(b5 == b4->jumpTo);
  REQUIRE(b6 == b5->jumpTo);
  REQUIRE(b1 == b5->branchTo);

  singletonCleanUp();
}

TEST_CASE("Simple", "[maximizeBlocks]")
{
  auto emptyBlock = std::make_shared<cs6300::BasicBlock>();
  auto b0 = std::make_shared<cs6300::BasicBlock>();
  auto b1 = std::make_shared<cs6300::BasicBlock>();

  emptyBlock->jumpTo = b0;
  b0->jumpTo = b1;

  b0->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 1, 0, 0));

  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 2, 1, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 3, 100, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::IsLess, 4, 2, 3));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 5, 5, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::Modulo, 7, 2, 5));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::StoreMemory, 7, 2, 0));

  auto blocks = cs6300::FlowGraph(b0, b1);

  REQUIRE(1 == b0->instructions.size());
  REQUIRE(nullptr != b0->jumpTo);

  cs6300::maximizeBlocks(blocks);

  REQUIRE(7 == b0->instructions.size());
  REQUIRE(nullptr == b0->jumpTo);

  singletonCleanUp();
}

TEST_CASE("Check Block Merge Order", "[maximizeBlocks]")
{

  auto emptyBlock = std::make_shared<cs6300::BasicBlock>();
  auto b0 = std::make_shared<cs6300::BasicBlock>();
  auto b1 = std::make_shared<cs6300::BasicBlock>();
  auto b2 = std::make_shared<cs6300::BasicBlock>();

  emptyBlock->jumpTo = b0;
  b0->jumpTo = b1;
  b1->jumpTo = b2;

  b0->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 1, 0, 0));

  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 2, 1, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 3, 100, 0));
  b1->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::IsLess, 4, 2, 3));

  b2->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::LoadValue, 5, 5, 0));
  b2->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::Modulo, 7, 2, 5));
  b2->instructions.push_back(cs6300::ThreeAddressInstruction(
    cs6300::ThreeAddressInstruction::StoreMemory, 7, 2, 0));

  auto blocks = cs6300::FlowGraph(b0, b2);

  REQUIRE(1 == b0->instructions.size());
  REQUIRE(nullptr != b0->jumpTo);

  cs6300::maximizeBlocks(blocks);

  REQUIRE(7 == b0->instructions.size());
  REQUIRE(nullptr == b0->jumpTo);

  REQUIRE(cs6300::ThreeAddressInstruction::LoadValue == b0->instructions[0].op);
  REQUIRE(cs6300::ThreeAddressInstruction::LoadValue == b0->instructions[1].op);
  REQUIRE(cs6300::ThreeAddressInstruction::LoadValue == b0->instructions[2].op);
  REQUIRE(cs6300::ThreeAddressInstruction::IsLess == b0->instructions[3].op);
  REQUIRE(cs6300::ThreeAddressInstruction::LoadValue == b0->instructions[4].op);
  REQUIRE(cs6300::ThreeAddressInstruction::Modulo == b0->instructions[5].op);
  REQUIRE(cs6300::ThreeAddressInstruction::StoreMemory ==
          b0->instructions[6].op);

  singletonCleanUp();
}
