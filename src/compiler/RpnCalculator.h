#ifndef RPNCALCULATOR_H
#define RPNCALCULATOR_H

#include "containers/Vector.h"
#include "compiler/RpnExpr.h"

enum class RpnCalculatorErrorType
{
  InsufficientOperands,
};

inline const char *enum_to_string (RpnCalculatorErrorType type)
{
  switch (type)
    {
    case RpnCalculatorErrorType::InsufficientOperands:
      return "Insufficient operands number";
    }

  ASSERT_RETURN (false, "");
}

struct RpnCalculatorError
{
  RpnCalculatorErrorType type;
  int queue_pos = -1;

  void set_ok () {queue_pos = -1;}
  bool is_ok () const {return queue_pos < 0;}
};

class RpnCalculator
{
public:
  RpnCalculator ();

  bool is_ok () const {return m_error.is_ok ();}
  const RpnCalculatorError &error () const {return m_error;}

  double evaluate (const RpnExpr &expr);
private:
  RpnCalculatorError m_error;
};

#endif // RPNCALCULATOR_H
