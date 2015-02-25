#include "catch.hpp"

#include "ProgramFactory.hpp"
#include "AST/Expressions/LiteralExpression.hpp"

TEST_CASE("Statements", "[statement]")
{
  ProgramFactory factory;

  SECTION("Assignment Statement")
  {
    std::string s = factory.literal("b", 14).id("a").assign("a", "b").str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $1, 14
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
)";
    REQUIRE(s == exp);
  }

  SECTION("Call Statement")
  {
    std::string s = factory.call("fname", 14).str();
    std::string exp = R"(BB1:
j BB2
BB2:
addi $sp, $sp, 0 # vars
sw $fp, -4($sp) #store frame
sw $ra, -8($sp)
addi $sp, $sp, -8
move $fp, $sp
addi $sp, $sp, 0
jal F14
addi $sp, $sp, 0
addi $sp, $sp, 8 # restore frame
lw $8, -12($sp)
lw $ra, -8($sp)
lw $fp, -4($sp)
addi $sp, $sp, 0 #vars
)";
    REQUIRE(s == exp);

    factory.literal("a", 5).literal("b", 10).literal("c", 70);
    s = factory.call("fname", 35, factory.expressions()).str();
    exp = R"(BB1:
j BB2
BB2:
li $1, 5
li $2, 10
li $3, 70
addi $sp, $sp, 0 # vars
sw $fp, -4($sp) #store frame
sw $ra, -8($sp)
addi $sp, $sp, -8
sw $1, -4($29) #copying argument
sw $2, -8($29) #copying argument
sw $3, -12($29) #copying argument
move $fp, $sp
addi $sp, $sp, -12
jal F35
addi $sp, $sp, 12
addi $sp, $sp, 8 # restore frame
lw $8, -12($sp)
lw $ra, -8($sp)
lw $fp, -4($sp)
addi $sp, $sp, 0 #vars
)";
    REQUIRE(s == exp);
  }

  SECTION("For Statement")
  {
    std::string s = factory.id("a")
                      .literal("5", 5)
                      .literal("50", 50)
                      .for_("a", "5", "50", cs6300::ForStatement::TO)
                      .str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $1, 5
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
j BB3
BB3:
li $4, 50
addi $5, $gp, 0 # Load a global
lw $6, 0($5)
slt $3, $4, $6
bne $3, $zero, BB4
j BB5
BB4:
BB5:
j BB6
BB6:
addi $7, $gp, 0 # Load a global
lw $8, 0($7)
addi $9, $8, 1
addi $10, $gp, 0 # Load a global
sw $9, 0($10) #store memory
j BB3
)";
    REQUIRE(s == exp);

    ProgramFactory innerFor;
    innerFor.literal("b", 800).id("a").assign("a", "b");
    s = factory.id("a")
          .literal("60", 60)
          .literal("100", 100)
          .for_("a",
                "60",
                "100",
                cs6300::ForStatement::DOWNTO,
                innerFor.statements())
          .str();
    exp = R"(BB1:
j BB2
BB2:
li $1, 60
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
j BB3
BB3:
addi $4, $gp, 0 # Load a global
lw $5, 0($4)
li $6, 100
slt $3, $5, $6
bne $3, $zero, BB4
j BB5
BB4:
BB5:
j BB6
BB6:
li $11, 800
addi $12, $gp, 0 # Load a global
sw $11, 0($12) #store memory
j BB7
BB7:
addi $7, $gp, 0 # Load a global
lw $8, 0($7)
addi $9, $8, -1
addi $10, $gp, 0 # Load a global
sw $9, 0($10) #store memory
j BB3
)";
    REQUIRE(s == exp);
  }

  SECTION("If Statement")
  {
    factory.literal("zero", 0).literal("five", 5).literal("ten", 10).id("a").id(
      "b");

    auto if1 = {
      factory.assign("a", "five").pop(), factory.assign("a", "zero").pop()};
    auto if2 = {
      factory.assign("b", "ten").pop(), factory.assign("b", "five").pop()};
    auto elsestms = {
      factory.assign("a", "five").pop(), factory.assign("b", "ten").pop()};
    std::string s = factory.lt("lt", "five", "ten")
                      .gt("gt", "five", "ten")
                      .if_({{"lt", if1}, {"gt", if2}}, elsestms)
                      .str();
    std::string exp = R"(BB1:
j BB2
BB2:
j BB3
BB3:
li $2, 5
li $3, 10
slt $1, $2, $3
bne $1, $zero, BB4
j BB5
BB4:
j BB6
BB6:
li $2, 5
addi $4, $gp, 0 # Load a global
sw $2, 0($4) #store memory
j BB7
BB7:
li $5, 0
addi $6, $gp, 0 # Load a global
sw $5, 0($6) #store memory
j BB8
BB8:
BB5:
li $2, 5
li $3, 10
slt $7, $3, $2
bne $7, $zero, BB9
j BB10
BB9:
j BB11
BB11:
li $3, 10
addi $8, $gp, 4 # Load a global
sw $3, 0($8) #store memory
j BB12
BB12:
li $2, 5
addi $9, $gp, 4 # Load a global
sw $2, 0($9) #store memory
j BB8
BB10:
j BB13
BB13:
li $2, 5
addi $10, $gp, 0 # Load a global
sw $2, 0($10) #store memory
j BB14
BB14:
li $3, 10
addi $11, $gp, 4 # Load a global
sw $3, 0($11) #store memory
j BB8
)";
    REQUIRE(s == exp);
  }

  SECTION("Repeat Statement")
  {
    auto repeatLoop = {factory.stop().pop(),
                       factory.id("x").literal("a", 14).assign("x", "a").pop()};
    std::string s = factory.literal("1", 1)
                      .literal("2", 2)
                      .gt("gt", "1", "2")
                      .repeat("gt", repeatLoop)
                      .str();
    std::string exp = R"(BB1:
j BB2
BB2:
j BB3
BB3:
li $v0, 10
syscall
j BB4
BB4:
li $1, 14
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
j BB5
BB5:
li $4, 1
li $5, 2
slt $3, $5, $4
bne $3, $zero, BB6
j BB2
BB6:
)";
    REQUIRE(s == exp);
  }

  SECTION("While Statement")
  {
    auto whileLoop = {factory.stop().pop(),
                      factory.id("x").literal("a", 14).assign("x", "a").pop()};
    std::string s = factory.literal("1", 1)
                      .literal("2", 2)
                      .gt("gt", "1", "2")
                      .while_("gt", whileLoop)
                      .str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $4, 1
li $5, 2
slt $3, $5, $4
bne $3, $zero, BB3
j BB4
BB3:
j BB5
BB5:
li $v0, 10
syscall
j BB6
BB6:
li $1, 14
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
j BB2
BB4:
)";
    REQUIRE(s == exp);
  }

  SECTION("Stop Statement")
  {
    std::string s = factory.stop().str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $v0, 10
syscall
)";
    REQUIRE(s == exp);
  }

  SECTION("Return Statement")
  {
    std::string s = factory.literal("a", 14).return_("a").str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $1, 14
move $v1, $1
jr $ra
)";
    REQUIRE(s == exp);
  }

  SECTION("Read Statement")
  {
    std::string s = factory.id("a").id("b").id("c").read({"a", "b", "c"}).str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $v0, 5
syscall
move $1, $v0
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
li $v0, 5
syscall
move $3, $v0
addi $4, $gp, 4 # Load a global
sw $3, 0($4) #store memory
li $v0, 5
syscall
move $5, $v0
addi $6, $gp, 8 # Load a global
sw $5, 0($6) #store memory
)";
    REQUIRE(s == exp);
  }

  SECTION("Write Statement")
  {
    std::string s = factory.literal("a", 14)
                      .literal("b", 799)
                      .literal("c", 12)
                      .write({"a", "b", "c"})
                      .str();
    std::string exp = R"(BB1:
j BB2
BB2:
li $1, 14
li $v0, 1
move $a0, $1
syscall    # writing an integer
li $2, 799
li $v0, 1
move $a0, $2
syscall    # writing an integer
li $3, 12
li $v0, 1
move $a0, $3
syscall    # writing an integer
)";
    REQUIRE(s == exp);
  }
}
