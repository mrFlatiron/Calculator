#include "RpnCalculator.h"
#include "RpnExpr.h"
#include "Operator.h"

RpnCalculator::RpnCalculator ()
{

}

double RpnCalculator::evaluate (const RpnExpr &expr)
{
  using ElType = RpnExpr::ElType;

  Vector<double> values;

  const Vector<RpnExpr::El> &queue = expr.m_out_queue;


  int size = queue.size ();
  for (int i = 0; i < size; i++)
    {
      if (queue[i].type == ElType::Value)
        {
          values.push_back_alloc (queue[i].value);
          continue;
        }

      Operator op = to_operator (queue[i].oper);
      int arity = get_arity (op);

      if (values.size () < arity)
        {
          m_error.queue_pos = i;
          m_error.type = RpnCalculatorErrorType::InsufficientOperands;
          return 0;
        }

      switch (arity)
        {
        case 1:
          {
            values.back () = apply_operator (op, values.back ());
            break;
          }
        case 2:
          {
            double right = values.pop_back ();
            double &left = values.back ();
            left = apply_operator (op, left, right);
            break;
          }
        }
    }

  if (values.size () != 1)
    {
      m_error.queue_pos = size -1;
      m_error.type = RpnCalculatorErrorType::InsufficientOperands;
      return 0;
    }

  ASSERT_RETURN (values.size () == 1, 0);

  return values.back ();
}
