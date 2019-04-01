#include "RpnExpr.h"
#include "compiler/TokenType.h"
#include "Operator.h"

void RpnExpr::pb_alloc (Operator op)
{
  if (m_operators_stack.empty ())
    {
      m_operators_stack.push_back_alloc (from_operator (op));
      return;
    }

  while (true)
    {
      RpnExprOperator rpn_op = m_operators_stack.back ();

      bool need_break = false;

      switch (rpn_op)
        {
        case RpnExprOperator::OpenPsis:
        case RpnExprOperator::ClosePsis:
          need_break = true;
          break;
        default:
          break;
        }

      if (need_break)
        break;

      Operator top_op = to_operator (rpn_op);

      if (priority (top_op) > priority (op)
          || (priority (top_op) == priority (op)
              && is_left_associative (top_op)))
        {
          m_out_queue.emplace_back_alloc (m_operators_stack.pop_back ());
          continue;
        }

      break;
    }

  m_operators_stack.push_back_alloc (from_operator (op));
}

bool RpnExpr::pb_alloc(TokenType psis)
{
  ASSERT_RETURN (psis == TokenType::OpenPsis || psis == TokenType::ClosePsis, false);

  if (psis == TokenType::OpenPsis)
    {
      m_operators_stack.push_back_alloc (from_token_type (psis));
      return true;
    }

  //right psis

  while (true)
    {
      if (m_operators_stack.empty ())
        return false;

      RpnExprOperator rpn_op = m_operators_stack.back ();

      if (rpn_op != RpnExprOperator::OpenPsis)
        {
          m_out_queue.emplace_back_alloc (m_operators_stack.pop_back ());
          continue;
        }

      break;
    }

  m_operators_stack.pop_back ();

  return true;
}

void RpnExpr::pb_alloc (double value)
{
  m_out_queue.emplace_back_alloc (value);
}
