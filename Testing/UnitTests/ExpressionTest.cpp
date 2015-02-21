#include "catch.hpp"
#include "AST/Expressions/AdditionExpression.hpp"
#include "AST/Expressions/LiteralExpression.hpp"
#include "AST/Expressions/MemoryAccessExpression.hpp"

TEST_CASE("LiteralExpression", "[expression]")
{
  cs6300::LiteralExpression char_exp('q');
  cs6300::LiteralExpression int_exp(42);
  cs6300::LiteralExpression bool_exp(false);

  SECTION("constants are correct")
  {
    REQUIRE(char_exp.isConst());
    REQUIRE(int_exp.isConst());
    REQUIRE(bool_exp.isConst());
  }

  SECTION("constructed with correct type")
  {
    REQUIRE(char_exp.type() == cs6300::BuiltInType::getChar());
    REQUIRE(int_exp.type() == cs6300::BuiltInType::getInt());
    REQUIRE(bool_exp.type() == cs6300::BuiltInType::getBool());
  }

  SECTION("values are correct")
  {
    REQUIRE(char_exp.value() == 'q');
    REQUIRE(int_exp.value() == 42);
    REQUIRE(bool_exp.value() == false);
  }

  SECTION("names are correct")
  {
    REQUIRE(char_exp.name() == R"("'q'")");
    REQUIRE(int_exp.name() == "42");
    REQUIRE(bool_exp.name() == "0");
  }

  SECTION("AST is correct")
  {
    REQUIRE(char_exp.ASTDot().size() == 0);
    REQUIRE(int_exp.ASTDot().size() == 0);
    REQUIRE(bool_exp.ASTDot().size() == 0);
  }

  SECTION("emit has correct blocks")
  {
    std::shared_ptr<cs6300::BasicBlock> blocks;

    blocks = char_exp.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 1);
    auto tal = blocks->instructions[0];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::LoadValue);
    REQUIRE(tal.src1 == 'q');
    REQUIRE(tal.src2 == 0);

    blocks = int_exp.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 1);
    tal = blocks->instructions[0];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::LoadValue);
    REQUIRE(tal.src1 == 42);
    REQUIRE(tal.src2 == 0);

    blocks = bool_exp.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 1);
    tal = blocks->instructions[0];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::LoadValue);
    REQUIRE(tal.src1 == false);
    REQUIRE(tal.src2 == 0);
  }
}

TEST_CASE("AdditionExpression", "[expression]")
{
  cs6300::AdditionExpression lit_exp(
    std::make_shared<cs6300::LiteralExpression>(4),
    std::make_shared<cs6300::LiteralExpression>(20));
  cs6300::AdditionExpression lit_int(
    std::make_shared<cs6300::LiteralExpression>(4), -10);
  cs6300::AdditionExpression mem_lit(
    std::make_shared<cs6300::MemoryAccessExpression>(1, 2),
    std::make_shared<cs6300::LiteralExpression>(20));
  cs6300::AdditionExpression mem_int(
    std::make_shared<cs6300::MemoryAccessExpression>(1, 2), -10);

  SECTION("constants are correct")
  {
    REQUIRE(lit_exp.isConst());
    REQUIRE(lit_int.isConst());
    REQUIRE(!mem_lit.isConst());
    REQUIRE(!mem_int.isConst());
  }

  SECTION("constructed with correct type")
  {
    REQUIRE(lit_exp.type() == cs6300::BuiltInType::getInt());
    REQUIRE(lit_int.type() == cs6300::BuiltInType::getInt());
    REQUIRE(mem_lit.type() == nullptr);
    REQUIRE(mem_int.type() == nullptr);
  }

  SECTION("values are correct")
  {
    REQUIRE(lit_exp.value() == 24);
    REQUIRE(lit_int.value() == -6);
    REQUIRE(mem_lit.value() == 0);
    REQUIRE(mem_int.value() == 0);
  }

  SECTION("names are correct")
  {
    REQUIRE(lit_exp.name() == R"("+")");
    REQUIRE(lit_int.name() == R"("+")");
    REQUIRE(mem_lit.name() == R"("+")");
    REQUIRE(mem_int.name() == R"("+")");
  }

  SECTION("emit has correct blocks")
  {
    std::shared_ptr<cs6300::BasicBlock> blocks;

    blocks = lit_exp.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 3);
    auto tal = blocks->instructions[2];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::Add);

    blocks = lit_int.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 3);
    tal = blocks->instructions[2];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::Add);

    blocks = mem_lit.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 3);
    tal = blocks->instructions[2];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::Add);

    blocks = mem_int.emit();
    REQUIRE(blocks.get() != nullptr);
    REQUIRE(blocks->instructions.size() == 3);
    tal = blocks->instructions[2];
    REQUIRE(tal.op == cs6300::ThreeAddressInstruction::Add);
  }
}
