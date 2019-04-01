#include "ExprLexer.h"
#include "Sentence.h"
#include "TokenType.h"
#include "CharHelpers.h"
#include "Operator.h"

#include "gold/debug_assert.h"

#include <limits>

ExprLexer::ExprLexer ()
  : m_double_scanner (m_token_buf)
{

}

Sentence ExprLexer::parse (const char *expr)
{
  Sentence retval;

  to_init_state ();

  m_retval = &retval;

  const char *expr_ptr = expr;

  m_expr = expr;

  while (*expr_ptr)
    {
      m_expr_pos = expr_ptr - expr;
      char c = *expr_ptr;
      if (!process_char (c))
        break;
      expr_ptr++;
    }

  m_retval = nullptr;

  if (!m_error.is_ok ())
    return Sentence ();

  return retval;
}

bool ExprLexer::is_ok () const
{
  return m_error.is_ok ();
}

bool ExprLexer::process_char (char c)
{
  switch (m_state)
    {
    case ExprLexerState::Start:
      return process_char_start (c);
    case ExprLexerState::ConsumingToken:
      return process_char_consuming_token (c);
    }

  ASSERT_RETURN (false, true);
}

bool ExprLexer::process_char_start (char c)
{
  if (is_whitespace (c))
    return true;

  m_state = ExprLexerState::ConsumingToken;

  clear_token_buf ();
  return process_char_consuming_token (c);
}

bool ExprLexer::process_char_consuming_token (char c)
{
  if (is_whitespace (c))
    {
      bool is_ok = process_token_buf ();

      set_state (ExprLexerState::Start);
      return is_ok;
    }

  if (is_open_psis (c) || is_close_psis (c) || is_certainly_operator (c))
    {
      if (!m_token_buf.empty ())
        {
          if (!process_token_buf ())
            return false;

          clear_token_buf ();
        }

      m_token_buf.append (c);

      bool is_ok = process_token_buf ();

      set_state (ExprLexerState::Start);
      return is_ok;
    }

  if (is_sign (c))
    {
      if (try_append_to_possible_number (c))
        return true;

      if (!m_token_buf.empty ())
        {
          if (!process_token_buf ())
            return false;
        }

      clear_token_buf ();
      m_token_buf.append (c);

      bool is_ok = process_token_buf ();

      set_state (ExprLexerState::Start);
      return is_ok;
    }

  if (!can_be_in_number (c))
    {
      set_error (ExprLexerErrorType::InvalidCharacter);
      return false;
    }

  if (!try_append_to_possible_number (c))
    {
      set_error (ExprLexerErrorType::InvalidNumber);
      return false;
    }

  return true;
}

bool ExprLexer::process_token_buf ()
{
  ASSERT_RETURN (!m_token_buf.empty (), false);

  if (m_token_buf.size () == 1)
    {
      char c = m_token_buf[0];

      if (is_open_psis (c))
        {
          m_retval->symbols.push_back_alloc (TokenType::OpenPsis);
          m_retval->start_pos_in_original_expr.push_back_alloc (m_expr_pos);

          set_state (ExprLexerState::Start);
          return true;
        }

      if (is_close_psis (c))
        {
          m_retval->symbols.push_back_alloc (TokenType::ClosePsis);
          m_retval->start_pos_in_original_expr.push_back_alloc (m_expr_pos);

          set_state (ExprLexerState::Start);
          return true;
        }

      if (is_possible_operator (c))
        {
          Operator op = append_arity_qualificator (char_to_operator (c));

          m_retval->pb_alloc_operator (op, m_expr_pos);

          set_state (ExprLexerState::Start);
          return true;
        }
    }

  double value;
  bool ok;

  value = m_double_scanner.get_double (ok);

  if (!ok)
    {
      set_error (ExprLexerErrorType::InvalidNumber, m_expr_pos - m_token_buf.size () - 1);
      return false;
    }

  m_retval->pb_alloc_value (value, m_expr_pos - m_token_buf.size () - 1);

  return true;
}

bool ExprLexer::try_append_to_possible_number (char c)
{
  return m_double_scanner.try_append (c);
}

Operator ExprLexer::append_arity_qualificator (Operator src) const
{
  switch (src)
    {
    case Operator::Mult:
    case Operator::Div:
    case Operator::Power:
      return src;
    case Operator::UnaryMinus:
    case Operator::UnaryPlus:
      ASSERT_RETURN (false, src);
    case Operator::Minus:
    case Operator::Plus:
      if (m_retval->symbols.empty ()
          || m_retval->symbols.back () == TokenType::OpenPsis)
        return append_unary (src);
    }

  return src;
}

void ExprLexer::set_state (ExprLexerState state)
{
  m_state = state;

  clear_token_buf ();
}

void ExprLexer::to_init_state ()
{
  m_error.set_ok ();
  m_expr = nullptr;
  m_expr_pos = -1;
  m_state = ExprLexerState::Start;
  m_retval = nullptr;
  clear_token_buf ();
}

void ExprLexer::clear_token_buf ()
{
  m_double_scanner.clear ();
}

void ExprLexer::set_error (ExprLexerErrorType type)
{
  m_error.type = type;
  m_error.position = m_expr_pos;
}

void ExprLexer::set_error (ExprLexerErrorType type, int pos)
{
  m_error.type = type;
  m_error.position = pos;
}
