#include "Sentence.h"
#include "TokenType.h"

void Sentence::pb_alloc_operator (Operator op, int pos_in_expr)
{
  symbols.push_back_alloc (TokenType::Op);
  start_pos_in_original_expr.push_back_alloc (pos_in_expr);

  operators.push_back_alloc (op);
}

void Sentence::pb_alloc_value (double value, int pos_in_expr)
{
  symbols.push_back_alloc (TokenType::Value);
  start_pos_in_original_expr.push_back_alloc (pos_in_expr);

  values.push_back_alloc (value);
}
