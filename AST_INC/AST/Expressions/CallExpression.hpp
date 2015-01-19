#ifndef CS6300_AST_CALL_EXPRESSION_HPP
#define CS6300_AST_CALL_EXPRESSION_HPP
#include "Expression.hpp"
namespace cs6300
{
class CallExpression : public Expression
{
public:
  CallExpression(std::string n, int, std::vector<std::shared_ptr<Expression> >, std::shared_ptr<Type>);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  int value() const;
  bool isConst() const;
  std::string name()const;
  std::vector<std::string> ASTDot()const;

private:
  int funcLabel;
  std::string _name;
  std::vector<std::shared_ptr<Expression>> actualArguments;
  std::shared_ptr<Type> returnType;
};
}
#endif

