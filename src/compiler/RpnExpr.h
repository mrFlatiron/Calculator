#ifndef RPNEXPR_H
#define RPNEXPR_H

#include "containers/Vector.h"
#include "RpnExprOperator.h"

enum class Operator;
enum class TokenType;



class RpnExpr
{
public:
  RpnExpr () {}

private:
  friend class RpnBuilder;
  friend class RpnCalculator;

  enum class ElType {Value, Operator};
  struct El
  {
    ElType type;
    RpnExprOperator oper;
    double value = 0;

    El (RpnExprOperator oper_arg) : type (ElType::Operator), oper (oper_arg) {}
    El (double value_arg) : type (ElType::Value), value (value_arg) {}
  };

  Vector<El> m_out_queue;

  Vector<RpnExprOperator> m_operators_stack;
private:
  void pb_alloc (Operator op);
  bool pb_alloc (TokenType psis);
  void pb_alloc (double value);
};

#endif // RPNEXPR_H
