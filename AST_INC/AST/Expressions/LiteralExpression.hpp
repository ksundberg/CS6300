#ifndef CS6300_AST_LITERAL_EXPRESSION_HPP
#define CS6300_AST_LITERAL_EXPRESSION_HPP
#include "Expression.hpp"
namespace cs6300
{
class LiteralExpression : public Expression
{
public:
  LiteralExpression(char a);
  LiteralExpression(int a);
  LiteralExpression(bool b);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  int value() const;
  bool isConst() const;
  std::string name() const;
  std::vector<std::string> ASTDot() const;

private:
  std::shared_ptr<Type> m_type;
  int m_value;
};
}
#endif
