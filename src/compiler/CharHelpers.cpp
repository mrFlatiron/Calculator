#include "CharHelpers.h"

bool is_whitespace (char c)
{
  switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      return true;
    default:
      return false;
    }
}

bool is_possible_operator (char c)
{
  switch (c)
    {
    case '-':
    case '+':
    case '/':
    case '*':
    case '^':
      return true;
    default:
      return false;
    }
}

bool is_certainly_operator (char c)
{
  switch (c)
    {
    case '*':
    case '/':
    case '^':
      return true;
    default:
      return false;
    }
}

bool can_be_in_number (char c)
{
  if (is_digit (c) || is_sign (c))
    return true;

  switch (c)
    {
    case '.':
    case 'e':
      return true;
    default:
      return false;
    }
}
