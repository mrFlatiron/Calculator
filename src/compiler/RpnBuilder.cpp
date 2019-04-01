#include "RpnBuilder.h"
#include "TokenType.h"
#include "Sentence.h"
#include "RpnExpr.h"

RpnBuilder::RpnBuilder ()
{

}

RpnExpr RpnBuilder::build_rpn (const Sentence &ir)
{
  RpnExpr rpn;

  m_error.set_ok ();

  int size = ir.symbols.size ();

  int values_counter = 0;
  int op_counter = 0;

  for (int i = 0; i < size; i++)
    {
      switch (ir.symbols[i])
        {
        case TokenType::OpenPsis:
        case TokenType::ClosePsis:
          {
            if (!rpn.pb_alloc (ir.symbols[i]))
              {
                m_error.type = RpnBuilderErrorType::PsisDoNotMatch;
                m_error.position = ir.start_pos_in_original_expr[i];
                return RpnExpr ();
              }
            break;
          }
        case TokenType::Value:
          rpn.pb_alloc (ir.values[values_counter]);
          values_counter++;
          break;
        case TokenType::Op:
          rpn.pb_alloc (ir.operators[op_counter]);
          op_counter++;
          break;
        }
    }

  int remaining_operators = rpn.m_operators_stack.size ();

  for (int i = remaining_operators - 1; i >= 0; i--)
    rpn.m_out_queue.emplace_back_alloc (rpn.m_operators_stack.pop_back ());

  return rpn;
}
