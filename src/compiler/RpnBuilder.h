#ifndef RPNBUILDER_H
#define RPNBUILDER_H

#include "containers/Vector.h"
#include "compiler/RpnExpr.h"

#include <bitset>

template<typename T>
class Vector;

struct Sentence;

enum class RpnBuilderErrorType
{
  PsisDoNotMatch,
};

inline const char *enum_to_string (RpnBuilderErrorType type)
{
  switch (type)
    {
    case RpnBuilderErrorType::PsisDoNotMatch:
      return "Parenthesis do not match";
    }

  ASSERT_RETURN (false, "");
}

struct RpnBuilderError
{
  RpnBuilderErrorType type;
  int position = -1;

  operator bool () const {return !is_ok ();}
  void set_ok () {position = -1;}
  bool is_ok () const {return position < 0;}
};

class RpnBuilder
{
public:
  RpnBuilder ();

  const RpnBuilderError &error () const {return m_error;}
  bool is_ok () const {return m_error.is_ok ();}
  RpnExpr build_rpn (const Sentence &ir);
private:
  RpnBuilderError m_error;
};

#endif // RPNBUILDER_H
