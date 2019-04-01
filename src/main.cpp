#include "containers/Vector.h"

#include "containers/CharBuf.h"
#include "compiler/ExprLexer.h"
#include "compiler/Sentence.h"
#include "compiler/RpnBuilder.h"
#include "compiler/RpnCalculator.h"
#include "compiler/ErrorPrinter.h"

#include <cstring>
#include <cstdio>

int main (int argc, char *argv[])
{
  if (argc != 1)
    {
      printf ("Usage: %s\n", argv[0]);
      return 1;
    }

  while (true)
    {
      printf (">>> ");
      CharBuf<2048> input;

      fgets (input.str (), 2048, stdin);
      input.recalculate_size ();

      if (0 == strcmp (input.c_str (), "quit\n"))
        return 0;

      ExprLexer lexer;

      Sentence ir = lexer.parse (input.c_str ());

      if (!lexer.is_ok ())
        {
          printf ("%s", form_pointing_error (input.c_str (), enum_to_string (lexer.error ().type), lexer.error ().position).c_str ());
          continue;
        }

      RpnBuilder rpn_builder;
      RpnExpr rpn = rpn_builder.build_rpn (ir);

      if (!rpn_builder.is_ok ())
        {
          printf ("%s", form_pointing_error (input.c_str (), enum_to_string (rpn_builder.error ().type), rpn_builder.error ().position).c_str ());
          continue;
        }

      RpnCalculator rpn_calc;
      double value = rpn_calc.evaluate (rpn);

      if (!rpn_calc.is_ok ())
        {
          printf ("Error occured while evaluating RPN expression: %s, position: %d\n", enum_to_string (rpn_calc.error ().type), rpn_calc.error ().queue_pos);
          continue;
        }

      printf ("%.6lf\n", value);
    }

  return 0;
}
