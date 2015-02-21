#include "catch.hpp"
#include "AST/Expressions/LiteralExpression.hpp"

TEST_CASE("LiteralExpression", "[expression]")
{
  cs6300::LiteralExpression char_exp('q');
  cs6300::LiteralExpression int_exp(42);
  cs6300::LiteralExpression bool_exp(false);

  SECTION("all literal expressions are constant")
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
