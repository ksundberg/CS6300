#include "catch.hpp"

#include "ProgramFactory.hpp"
#include "AST/Expressions/LiteralExpression.hpp"

TEST_CASE("Statements", "[statement]")
{
  ProgramFactory factory;

  SECTION("Assignment Statement")
  {
    std::string s = factory.id("b").assign("b", 14).str();
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
    std::string exp = R"(BB3:
j BB4
BB4:
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

    std::vector<std::shared_ptr<cs6300::Expression>> args = {
      std::make_shared<cs6300::LiteralExpression>(5),
      std::make_shared<cs6300::LiteralExpression>(10),
      std::make_shared<cs6300::LiteralExpression>(70)};
    s = factory.call("fname", 35, args).str();
    exp = R"(BB5:
j BB6
BB6:
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
    ProgramFactory innerFor;
    std::string s = factory.id("a")
                      .for_("a", 5, 50, cs6300::ForStatement::TO, innerFor.stms)
                      .str();
    std::string exp = R"(BB7:
j BB8
BB8:
li $1, 5
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
j BB9
BB9:
li $4, 50
addi $5, $gp, 0 # Load a global
lw $6, 0($5)
slt $3, $4, $6
bne $3, $zero, BB10
j BB11
BB10:
BB11:
j BB12
BB12:
addi $7, $gp, 0 # Load a global
lw $8, 0($7)
addi $9, $8, 1
addi $10, $gp, 0 # Load a global
sw $9, 0($10) #store memory
j BB9
)";
    REQUIRE(s == exp);

    innerFor.id("b").assign("b", 800);
    s = factory.id("a")
          .for_("a", 60, 100, cs6300::ForStatement::DOWNTO, innerFor.stms)
          .str();
    exp = R"(BB13:
j BB14
BB14:
li $1, 60
addi $2, $gp, 0 # Load a global
sw $1, 0($2) #store memory
j BB15
BB15:
addi $4, $gp, 0 # Load a global
lw $5, 0($4)
li $6, 100
slt $3, $5, $6
bne $3, $zero, BB16
j BB17
BB16:
BB17:
j BB18
BB18:
li $11, 800
addi $12, $gp, 0 # Load a global
sw $11, 0($12) #store memory
j BB19
BB19:
addi $7, $gp, 0 # Load a global
lw $8, 0($7)
addi $9, $8, -1
addi $10, $gp, 0 # Load a global
sw $9, 0($10) #store memory
j BB15
)";
    REQUIRE(s == exp);
  }
}
