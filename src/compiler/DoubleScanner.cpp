#include "DoubleScanner.h"
#include "CharHelpers.h"

#include <cstdio>

DoubleScanner::DoubleScanner (CharBuf<256> &buf) :
  m_token_buf (buf)
{

}

void DoubleScanner::clear ()
{
  m_token_buf.clear ();
  m_e_pos = -1;
  m_point_pos = -1;
  m_exp_sign_pos = -1;
}

bool DoubleScanner::append (char c)
{
  if (is_digit (c))
    {
      if (m_token_buf.size () == 0)
        {
          m_token_buf.append (c);
          return true;
        }

      if (m_token_buf.size () == 1 && m_token_buf[0] == '0')
        return false;

      m_token_buf.append (c);
      return true;
    }

  if (c == '.')
    {
      if (m_point_pos >= 0)
        return false;

      if (m_e_pos >= 0)
        return false;

      if (m_token_buf.empty ())
        return false;

      m_token_buf.append (c);
      m_point_pos = m_token_buf.size () - 1;
      return true;
    }

  if (c == 'e')
    {
      if (m_e_pos >= 0)
        return false;

      if (m_token_buf.empty ())
        return false;

      m_token_buf.append (c);
      m_e_pos = m_token_buf.size () - 1;
      return true;
    }

  if (is_sign (c))
    {
      if (m_token_buf.empty ())
        return false;

      if (m_token_buf.back () != 'e')
        return false;

      m_token_buf.append (c);
      m_exp_sign_pos = m_token_buf.size () - 1;
      m_exp_sign = (c == '+') ? sign_t::plus : sign_t::minus;
      return true;
    }

  return false;
}

double DoubleScanner::get_double (bool &is_ok) const
{
  double value;

  if (1 != sscanf (m_token_buf.c_str (), "%lf", &value))
    {
      is_ok = false;
      return 0;
    }

  is_ok = true;
  return value;
}
