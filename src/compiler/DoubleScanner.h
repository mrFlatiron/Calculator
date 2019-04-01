#ifndef DOUBLESCANNER_H
#define DOUBLESCANNER_H

#include "containers/CharBuf.h"


class DoubleScanner
{
public:
  DoubleScanner (CharBuf<256> &buf);

  void clear ();
  bool append (char c);
  double get_double (bool &is_ok) const;
private:
  enum class sign_t
  {
    plus,
    minus
  };
  CharBuf<256> &m_token_buf;

  int m_e_pos = -1;
  int m_point_pos = -1;

  int m_exp_sign_pos = -1;
  sign_t m_exp_sign;
};

#endif // DOUBLESCANNER_H
