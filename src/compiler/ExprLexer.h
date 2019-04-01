#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include "containers/CharBuf.h"

#include "DoubleScanner.h"
#include "TokenType.h"

enum class Operator;
struct Sentence;

enum class ExprLexerErrorType
{
  InvalidCharacter,
  InvalidNumber,
};

inline const char *enum_to_string (ExprLexerErrorType type)
{
  switch (type)
    {
    case ExprLexerErrorType::InvalidCharacter:
      return "Invalid character";
    case ExprLexerErrorType::InvalidNumber:
      return "Invalid number";
    }

  return "";
}

struct ExprLexerError
{
  int position = -1;
  ExprLexerErrorType type;

  void set_ok () {position = -1;}
  bool is_ok () const {return position < 0;}
};


enum class ExprLexerState
{
  Start,
  ConsumingToken
};

class ExprLexer
{
public:
  ExprLexer ();

  Sentence parse (const char *expr);
  bool is_ok () const;
  const ExprLexerError &error () const {return m_error;}
private:
  ExprLexerError m_error;

  const char *m_expr = nullptr;
  int m_expr_pos = -1;

  ExprLexerState m_state = ExprLexerState::Start;
  Sentence *m_retval = nullptr;

  CharBuf<256> m_token_buf;
  DoubleScanner m_double_scanner;
private:
  void to_init_state ();
  void clear_token_buf ();
  void set_error (ExprLexerErrorType type);
  void set_error (ExprLexerErrorType type, int pos);
  bool process_char (char c);
  bool process_char_start (char c);
  bool process_char_consuming_token (char c);

  bool process_token_buf ();
  bool append_to_possible_number (char c);
  Operator append_arity_qualificator (Operator src) const;

  void set_state (ExprLexerState state);
};

#endif // EXPRPARSER_H
