#ifndef CHARHELPERS_H
#define CHARHELPERS_H

bool is_whitespace (char c);

//+ and - can be a part of a number in format 5e-1 or 1e+7
bool is_possible_operator (char c);

inline bool is_sign (char c) {return c == '+' || c == '-';}
bool is_certainly_operator (char c);

bool can_be_in_number (char c);
inline bool is_digit (char c) {return '0' <= c && c <= '9';}
inline double digit_to_double (char c) {return c - '0';}

inline bool is_open_psis (char c) {return c == '(';}
inline bool is_close_psis (char c) {return c == ')';}

#endif // CHARHELPERS_H
