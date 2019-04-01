#ifndef DEBUG_ASSERT_H
#define DEBUG_ASSERT_H

#ifdef __GNUC__
#include <csignal>
#endif

#ifndef DEBUG_TRIG_LIMIT
#define DEBUG_TRIG_LIMIT 3
#endif

#ifdef __GNUC__

  #define RAISE_SIGTRAP (std::raise (SIGTRAP))

#else

  #ifdef __WIN32__

    #define RAISE_SIGTRAP (__debugbreak ())

  #else

    #define RAISE_SIGTRAP

  #endif //__WIN32__

#endif //__GNUC__

#ifdef DEBUG
  #define DEBUG_PAUSE(reason) {static int DEBUG_triggered = 0; \
    DEBUG_triggered++; \
    if (DEBUG_triggered <= DEBUG_TRIG_LIMIT) RAISE_SIGTRAP;}
#else
  #define DEBUG_PAUSE(reason)
#endif

#define DEBUG_ASSERT(assertion) if (!(assertion)) DEBUG_PAUSE ("");

#define MACRO_OVERLOAD_2(_1, _2, ACTUAL_MACRO, ...) ACTUAL_MACRO

#define ASSERT_RETURN_VOID(assertion) if (!(assertion)) {DEBUG_PAUSE (""); return;}
#define ASSERT_RETURN_SMTH(assertion, retval) if (!(assertion)) {DEBUG_PAUSE (""); return retval;}

#define ASSERT_RETURN(...) MACRO_OVERLOAD_2(__VA_ARGS__, ASSERT_RETURN_SMTH, ASSERT_RETURN_VOID) (__VA_ARGS__)

#endif // DEBUG_ASSERT_H
