#ifndef OPERATOR_H
#define OPERATOR_H

#include <array>

#include "gold/debug_assert.h"

enum class Operator
{
  UnaryMinus,
  UnaryPlus,
  Minus,
  Plus,
  Mult,
  Div,
  Power
};

constexpr std::array<Operator, 7> ascending_priority_order =
{
 Operator::Minus,
 Operator::Plus,
 Operator::UnaryMinus,
 Operator::UnaryPlus,
 Operator::Div,
 Operator::Mult,
 Operator::Power
};

double apply_operator (Operator op, double number);
double apply_operator (Operator op, double left, double right);
int get_arity (Operator op);
Operator append_unary (Operator op);
int priority (Operator op);
bool is_left_associative (Operator op);

inline Operator char_to_operator (char c)
{
  switch (c)
    {
    case '-':
      return Operator::Minus;
    case '+':
      return Operator::Plus;
    case '*':
      return Operator::Mult;
    case '/':
      return Operator::Div;
    case '^':
      return Operator::Power;
    default:
      ASSERT_RETURN (false, Operator::Minus);
    }
}

#endif // OPERATOR_H
