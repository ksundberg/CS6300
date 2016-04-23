#include "catch.hpp"
#include "AST/Expressions/AdditionExpression.hpp"
#include "AST/Expressions/LiteralExpression.hpp"
#include "AST/Expressions/MemoryAccessExpression.hpp"

#include "ProgramFactory.hpp"

TEST_CASE("LiteralExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Positive Literals")
  {
    auto s = factory.literal("two", 2).exprstr("two");
    auto exp = R"(BB1:
li $1, 2
)";
    REQUIRE(s == exp);

    s = factory.literal("25", 25).exprstr("25");
    exp = R"(BB1:
li $1, 25
)";
    REQUIRE(s == exp);

    s = factory.literal("a", 'a').exprstr("a");
    exp = R"(BB1:
li $1, 97
)";
    REQUIRE(s == exp);

    s = factory.literal("true", true).exprstr("true");
    exp = R"(BB1:
li $1, 1
)";
    REQUIRE(s == exp);

    s = factory.literal("false", false).exprstr("false");
    exp = R"(BB1:
li $1, 0
)";
    REQUIRE(s == exp);
  }

  SECTION("Negative Literals")
  {
    auto s = factory.literal("two", -2).exprstr("two");
    auto exp = R"(BB1:
li $1, -2
)";
    REQUIRE(s == exp);

    s = factory.literal("25", -25).exprstr("25");
    exp = R"(BB1:
li $1, -25
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("AdditionExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Add Literals")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .add("add", "two", "three")
               .exprstr("add");
    auto exp = R"(BB1:
li $2, 2
li $3, 3
add $1, $2, $3
)";
    REQUIRE(s == exp);
  }

  SECTION("Nested")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .add("add1", "two", "three")
               .add("add2", "two", "three")
               .add("add3", "add1", "add2")
               .exprstr("add3");
    auto exp = R"(BB1:
li $3, 2
li $4, 3
add $2, $3, $4
li $3, 2
li $4, 3
add $5, $3, $4
add $1, $2, $5
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("SubtractExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Sub Literals")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .sub("sub", "two", "three")
               .exprstr("sub");
    auto exp = R"(BB1:
li $2, 2
li $3, 3
sub $1, $2, $3
)";
    REQUIRE(s == exp);
  }

  SECTION("Nested")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .sub("sub1", "two", "three")
               .sub("sub2", "two", "three")
               .sub("sub3", "sub1", "sub2")
               .exprstr("sub3");
    auto exp = R"(BB1:
li $3, 2
li $4, 3
sub $2, $3, $4
li $3, 2
li $4, 3
sub $5, $3, $4
sub $1, $2, $5
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("DivExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Div Literals")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .div("div", "two", "three")
               .exprstr("div");
    auto exp = R"(BB1:
li $2, 2
li $3, 3
div $2, $3
mflo $1
)";
    REQUIRE(s == exp);
  }

  SECTION("Nested")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .div("div1", "two", "three")
               .div("div2", "two", "three")
               .div("div3", "div1", "div2")
               .exprstr("div3");
    auto exp = R"(BB1:
li $3, 2
li $4, 3
div $3, $4
mflo $2
li $3, 2
li $4, 3
div $3, $4
mflo $5
div $2, $5
mflo $1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("MultExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Mult Literals")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .mul("mul", "two", "three")
               .exprstr("mul");
    auto exp = R"(BB1:
li $2, 2
li $3, 3
mult $2, $3
mflo $1
)";
    REQUIRE(s == exp);
  }

  SECTION("Nested")
  {
    auto s = factory.literal("two", 2)
               .literal("three", 3)
               .mul("mul1", "two", "three")
               .mul("mul2", "two", "three")
               .mul("mul3", "mul1", "mul2")
               .exprstr("mul3");
    auto exp = R"(BB1:
li $3, 2
li $4, 3
mult $3, $4
mflo $2
li $3, 2
li $4, 3
mult $3, $4
mflo $5
mult $2, $5
mflo $1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("AndExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("And Literals")
  {
    auto s = factory.literal("true", 1)
               .literal("false", 0)
               .and_("truth", "true", "true")
               .exprstr("truth");
    auto exp = R"(BB1:
li $2, 1
li $2, 1
and $1, $2, $2
)";
    REQUIRE(s == exp);
  }

  SECTION("Nested")
  {
    auto s = factory.literal("true", 1)
               .literal("false", 0)
               .and_("truth", "true", "true")
               .and_("truth2", "truth", "true")
               .and_("fake", "truth2", "false")
               .exprstr("fake");
    auto exp = R"(BB1:
li $4, 1
li $4, 1
and $3, $4, $4
li $4, 1
and $2, $3, $4
li $5, 0
and $1, $2, $5
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("OrExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Or Literals")
  {
    auto s = factory.literal("true", 1)
               .literal("false", 0)
               .or_("truth", "true", "true")
               .exprstr("truth");
    auto exp = R"(BB1:
li $2, 1
li $2, 1
or $1, $2, $2
)";
    REQUIRE(s == exp);
  }

  SECTION("Nested")
  {
    auto s = factory.literal("true", 1)
               .literal("false", 0)
               .or_("truth", "true", "false")
               .or_("truth2", "false", "true")
               .or_("truth3", "truth", "truth2")
               .exprstr("truth3");
    auto exp = R"(BB1:
li $3, 1
li $4, 0
or $2, $3, $4
li $4, 0
li $3, 1
or $5, $4, $3
or $1, $2, $5
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("EqExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Eq Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).eq("eq", "a", "b").exprstr(
        "eq");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
seq $1, $2, $3
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("NeqExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Neq Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).neq("neq", "a", "b").exprstr(
        "neq");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
sne $1, $2, $3
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("GtExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Gt Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).gt("gt", "a", "b").exprstr(
        "gt");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
slt $1, $3, $2
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("GteExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Gte Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).gte("gte", "a", "b").exprstr(
        "gte");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
sge $1, $2, $3
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("LtExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Lt Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).lt("lt", "a", "b").exprstr(
        "lt");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
slt $1, $2, $3
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("LteExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Lte Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).lte("lte", "a", "b").exprstr(
        "lte");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
sle $1, $2, $3
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("ModExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Mod Literals")
  {
    auto s =
      factory.literal("a", 15).literal("b", 30).mod("mod", "a", "b").exprstr(
        "mod");

    auto exp = R"(BB1:
li $2, 15
li $3, 30
div $2, $3
mfhi $1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("NegExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Neg Literals")
  {
    auto s = factory.literal("a", 15).neg("neg", "a").exprstr("neg");

    auto exp = R"(BB1:
li $1, 15
sub $2, $zero, $1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("CallExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Call empty")
  {
    auto s = factory.id("a").callexpr("call", "a", 32).exprstr("call");

    auto exp = R"(BB1:
addi $sp, $sp, 0 # vars
sw $fp, -4($sp) #store frame
sw $ra, -8($sp)
addi $sp, $sp, -8
move $fp, $sp
addi $sp, $sp, 0
jal F32
addi $sp, $sp, 0
addi $sp, $sp, 8 # restore frame
lw $8, -12($sp)
lw $ra, -8($sp)
lw $fp, -4($sp)
addi $sp, $sp, 0 #vars
move $1, $v1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("LoadExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Load empty")
  {
    auto s = factory.id("a").load("load", "a").exprstr("load");

    auto exp = R"(BB1:
addi $1, $gp, 0 # Load a global
lw $2, 0($1)
)";
    REQUIRE(s == exp);
  }

  SECTION("Load empty")
  {
    auto s = factory.id("a").load("load", "a", true).exprstr("load");

    auto exp = R"(BB1:
addi $1, $gp, 0 # Load a global
addi $2, $1, 0
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("MemoryAccessExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Global Memory Access")
  {
    auto s = factory.memaccess("x", cs6300::GLOBAL, 35).exprstr("x");

    auto exp = R"(BB1:
addi $1, $gp, 35 # Load a global
)";
    REQUIRE(s == exp);
  }

  SECTION("Stack Memory Access")
  {
    auto s = factory.memaccess("x", cs6300::STACK, 35).exprstr("x");

    auto exp = R"(BB1:
addi $1, $sp, -35 # Load a variable
)";
    REQUIRE(s == exp);
  }

  SECTION("Frame Memory Access")
  {
    auto s = factory.memaccess("x", cs6300::FRAME, 35).exprstr("x");

    auto exp = R"(BB1:
addi $1, $fp, -35 # Load a parameter
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("NotExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Not")
  {
    auto s = factory.literal("a", 15).not_("not", "a").exprstr("not");

    auto exp = R"(BB1:
li $1, 15
not $2, $1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("PredecessorExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Predecessor")
  {
    auto s = factory.literal("a", 15).pred("pred", "a").exprstr("pred");

    auto exp = R"(BB1:
li $1, 15
addi $2, $1, -1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("SuccessorExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("Successor")
  {
    auto s = factory.literal("a", 15).succ("succ", "a").exprstr("succ");

    auto exp = R"(BB1:
li $1, 15
addi $2, $1, 1
)";
    REQUIRE(s == exp);
  }
}

TEST_CASE("StringExpression", "[expression]")
{
  ProgramFactory factory;
  SECTION("String")
  {
    auto s = factory.string("str", "I am a string!").exprstr("str");

    auto exp = R"(BB1:
la $1, SL0 # load string expression
)";
    REQUIRE(s == exp);
  }
}
