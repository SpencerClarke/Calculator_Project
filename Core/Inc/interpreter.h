#include "lexer.h"
#include "transcendentals.h"

/* expr    := term + expr      | term - expr    | term
 * term    := sfactor * term   | sfactor / term | sfactor
 * sfactor := factor ^ sfactor | factor
 * factor  := RCONST           | ( expr )
 */


enum error_type {DIV_ZERO, SYNTAX, BAD_LN, NONE};

extern enum error_type error;
extern double ans;

int parse_top(char **input, double *retval);
int parse_expr(char **input, double *retval);
int parse_term(char **input, double *retval);
int parse_sfactor(char **input, double *retval);
int parse_pfactor(char **input, double *retval);
int parse_factor(char **input, double *retval);
