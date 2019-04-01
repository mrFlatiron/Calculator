#include "RpnExprOperator.h"
#include "Operator.h"
#include "compiler/TokenType.h"

Operator to_operator (RpnExprOperator rpn_op)
{
  switch (rpn_op)
    {
    case RpnExprOperator::OpenPsis:
    case RpnExprOperator::ClosePsis:
      ASSERT_RETURN (false, Operator::Minus);
    case RpnExprOperator::UnaryMinus:
      return Operator::UnaryMinus;
    case RpnExprOperator::UnaryPlus:
      return Operator::UnaryPlus;
    case RpnExprOperator::Minus:
      return Operator::Minus;
    case RpnExprOperator::Plus:
      return Operator::Plus;
    case RpnExprOperator::Div:
      return Operator::Div;
    case RpnExprOperator::Mult:
      return Operator::Mult;
    case RpnExprOperator::Power:
      return Operator::Power;
    }

  ASSERT_RETURN (false, Operator::Minus);
}

TokenType to_token_type (RpnExprOperator rpn_op)
{
  switch (rpn_op)
    {
    case RpnExprOperator::OpenPsis:
      return TokenType::OpenPsis;
    case RpnExprOperator::ClosePsis:
      return TokenType::ClosePsis;
    case RpnExprOperator::UnaryMinus:
    case RpnExprOperator::UnaryPlus:
    case RpnExprOperator::Minus:
    case RpnExprOperator::Plus:
    case RpnExprOperator::Div:
    case RpnExprOperator::Mult:
    case RpnExprOperator::Power:
      ASSERT_RETURN (false, TokenType::ClosePsis);
    }

  ASSERT_RETURN (false, TokenType::ClosePsis);
}

RpnExprOperator from_operator (Operator op)
{
  switch (op)
    {
    case Operator::UnaryMinus:
      return RpnExprOperator::UnaryMinus;
    case Operator::UnaryPlus:
      return RpnExprOperator::UnaryPlus;
    case Operator::Minus:
      return RpnExprOperator::Minus;
    case Operator::Plus:
      return RpnExprOperator::Plus;
    case Operator::Mult:
      return RpnExprOperator::Mult;
    case Operator::Div:
      return RpnExprOperator::Div;
    case Operator::Power:
      return RpnExprOperator::Power;
    }

  ASSERT_RETURN (false, RpnExprOperator::ClosePsis);
}

RpnExprOperator from_token_type (TokenType tt)
{
  switch (tt)
    {
    case TokenType::Value:
    case TokenType::Op:
      ASSERT_RETURN (false, RpnExprOperator::ClosePsis);
    case TokenType::OpenPsis:
      return RpnExprOperator::OpenPsis;
    case TokenType::ClosePsis:
      return RpnExprOperator::ClosePsis;
    }

  ASSERT_RETURN (false, RpnExprOperator::ClosePsis);
}
