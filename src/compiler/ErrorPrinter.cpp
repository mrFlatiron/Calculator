#include "ErrorPrinter.h"
#include <cstring>

std::string form_pointing_error (const char *user_input, const char *error_string, int error_position)
{
  const char *error_prefix = error_string;

  int prefix_size = strlen (error_prefix);

  std::string error_pointer (prefix_size + 2 + error_position + 2, ' ');

  int i = 0;

  for (; i < prefix_size; i++)
    error_pointer[i] = error_prefix[i];

  error_pointer[i] = ':';
  error_pointer[i + 1] = ' ';

  prefix_size += 2;

  error_pointer[prefix_size + error_position - 1] = '~';
  error_pointer[prefix_size + error_position] = '^';
  error_pointer[prefix_size + error_position + 1] = '~';

  int expr_size = strlen (user_input);

  std::string spaced_input (expr_size + prefix_size, ' ');

  for (int i = 0; i < expr_size; i++)
    spaced_input[i + prefix_size] = user_input[i];

  return spaced_input + error_pointer + "\n";
}
