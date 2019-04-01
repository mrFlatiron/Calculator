#ifndef RPNEXPROPERATOR_H
#define RPNEXPROPERATOR_H

enum class Operator;
enum class TokenType;

enum class RpnExprOperator
{
  OpenPsis,
  ClosePsis,
  UnaryMinus,
  UnaryPlus,
  Minus,
  Plus,
  Div,
  Mult,
  Power
};

Operator to_operator (RpnExprOperator rpn_op);
TokenType to_token_type (RpnExprOperator rpn_op);

RpnExprOperator from_operator (Operator op);
RpnExprOperator from_token_type (TokenType tt);

#endif // RPNEXPROPERATOR_H
