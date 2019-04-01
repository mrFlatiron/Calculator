#ifndef PARSEDEXPR_H
#define PARSEDEXPR_H

#include "containers/Vector.h"

enum class TokenType;
enum class Operator;

//Sentence does not contain unary minus and unary plus.
struct Sentence
{
  Vector<TokenType> symbols;
  Vector<int> start_pos_in_original_expr;

  Vector<double> values;

  Vector<Operator> operators;

  void pb_alloc_operator (Operator op, int pos_in_expr);
  void pb_alloc_value (double value, int pos_in_expr);
};

#endif // PARSEDEXPR_H
