#include "Operator.h"

#include <cmath>

int priority (Operator op)
{
  switch (op)
    {

    case Operator::Minus:
    case Operator::Plus:
      return 0;
    case Operator::UnaryMinus:
    case Operator::UnaryPlus:
      return 1;
    case Operator::Div:
    case Operator::Mult:
      return 2;
    case Operator::Power:
      return 3;
    }

  return 0;
}

bool is_left_associative (Operator op)
{
  switch (op)
    {
    case Operator::Minus:
    case Operator::Plus:
    case Operator::Mult:
    case Operator::Div:
      return true;
    case Operator::Power:
    case Operator::UnaryMinus:
    case Operator::UnaryPlus:
      return false;
    }

  return false;
}

Operator append_unary (Operator op)
{
  switch (op)
    {
    case Operator::Minus:
      return Operator::UnaryMinus;
    case Operator::Plus:
      return Operator::UnaryPlus;
    default:
      break;
    }

  ASSERT_RETURN (false, op);
}

int get_arity (Operator op)
{
  switch (op)
    {
    case Operator::Minus:
    case Operator::Plus:
    case Operator::Mult:
    case Operator::Div:
    case Operator::Power:
      return 2;
    case Operator::UnaryMinus:
    case Operator::UnaryPlus:
      return 1;
    }

  ASSERT_RETURN (false, 0);
}

double apply_operator (Operator op, double left, double right)
{
  switch (op)
    {
    case Operator::Minus:
      return left - right;
    case Operator::Plus:
      return left + right;
    case Operator::Mult:
      return left * right;
    case Operator::Div:
      return left / right;
    case Operator::Power:
      return pow (left, right);
    case Operator::UnaryMinus:
    case Operator::UnaryPlus:
      ASSERT_RETURN (false, left);
    }

  ASSERT_RETURN (false, left);
}

double apply_operator (Operator op, double number)
{
  switch (op)
    {
    case Operator::Minus:
    case Operator::Plus:
    case Operator::Mult:
    case Operator::Div:
    case Operator::Power:
      ASSERT_RETURN (false, number);
    case Operator::UnaryMinus:
      return -number;
    case Operator::UnaryPlus:
      return number;
    }

  ASSERT_RETURN (false, number);
}
