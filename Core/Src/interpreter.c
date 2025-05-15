#include "interpreter.h"

/* expr    := expr + term      | expr - term    | term
 * term    := sfactor * term   | sfactor / term | sfactor
 * sfactor := - pfactor        | pfactor
 * pfactor := factor ^ pfactor | factor
 * factor  := RCONST | ( expr ) | LN ( expr ) | SIN ( expr ) | COS ( expr )
 */

enum error_type error = NONE;
double ans = 0.0;

static void update_error(enum error_type type)
{
	if(error == NONE)
	{
		error = type;
	}
}

int parse_top(char **input, double *retval)
{
	int ret;
	error = NONE;
        has_last_token = 0;
        ret = parse_expr(input, retval);
        if(last_token.type != END)
        {
        	update_error(SYNTAX);
        	return 0;
        }
        else if(ret)
        {
        	ans = *retval;
        }
        return ret;
}
int parse_expr(char **input, double *retval)
{
        struct Token current_token;
        double term_retval;
        if(!parse_term(input, &term_retval))
        {
    		update_error(SYNTAX);
                return 0;
        }
        *retval = term_retval;

        while((current_token = get_next_token(input)).type == PLUS || current_token.type == MINUS)
        {
                if(current_token.type == PLUS)
                {
                        if(!parse_term(input, &term_retval))
                        {
				update_error(SYNTAX);
                                return 0;
                        }
                        *retval += term_retval;
                }
                else if(current_token.type == MINUS)
                {
                        if(!parse_term(input, &term_retval))
                        {
				update_error(SYNTAX);
                                return 0;
                        }
                        *retval -= term_retval;
                }
        }

        unget_token();
        return 1;
}
int parse_term(char **input, double *retval)
{
        struct Token current_token;
        double sfactor_retval;
        if(!parse_sfactor(input, &sfactor_retval))
        {
		update_error(SYNTAX);
                return 0;
        }
        *retval = sfactor_retval;

        while((current_token = get_next_token(input)).type == MULT || current_token.type == DIV)
        {
                if(current_token.type == MULT)
                {
                        if(!parse_term(input, &sfactor_retval))
                        {
				update_error(SYNTAX);
                                return 0;
                        }
                        *retval *= sfactor_retval;
                }
                else if(current_token.type == DIV)
                {
                        if(!parse_term(input, &sfactor_retval))
                        {
				update_error(SYNTAX);
                                return 0;
                        }
                        if(sfactor_retval == 0.0)
                        {
				update_error(DIV_ZERO);
                        	return 0;
                        }
                        *retval /= sfactor_retval;
                }
        }

        unget_token();
        return 1;

}
int parse_sfactor(char **input, double *retval)
{
        struct Token current_token;
        double unary_sign = 1.0;
        double pfactor_retval;

        current_token = get_next_token(input);
        if(current_token.type == MINUS)
        {
                unary_sign = -1.0;
        }
        else
        {
                unget_token();
        }
        if(!parse_pfactor(input, &pfactor_retval))
        {
		update_error(SYNTAX);
                return 0;
        }
        *retval = unary_sign * pfactor_retval;
        return 1;
}
int parse_pfactor(char **input, double *retval)
{
        double pfactor_retval;
        double factor_retval;
        struct Token current_token;

        if(!parse_factor(input, &factor_retval))
        {
		update_error(SYNTAX);
                return 0;
        }

        current_token = get_next_token(input);
        if(current_token.type != EXPONENT)
        {
                unget_token();
                *retval = factor_retval;
                return 1;
        }

        if(!parse_pfactor(input, &pfactor_retval))
        {
		update_error(SYNTAX);
                return 0;
        }

        *retval = powff(factor_retval, pfactor_retval);
        return 1;
}
int parse_factor(char **input, double *retval)
{
        struct Token current_token;

        current_token = get_next_token(input);
        if(current_token.type == LPAREN)
        {
                double expr_retval;
                if(!parse_expr(input, &expr_retval))
                {
			update_error(SYNTAX);
                        return 0;
                }
                current_token = get_next_token(input);
                if(current_token.type != RPAREN)
                {
			update_error(SYNTAX);
                        return 0;
                }
                *retval = expr_retval;
                return 1;
        }
        else if(current_token.type == LN)
        {
        	double expr_retval;

        	current_token = get_next_token(input);
        	if(current_token.type != LPAREN)
        	{
			update_error(SYNTAX);
        		return 0;
        	}

        	if(!parse_expr(input, &expr_retval))
        	{
			update_error(SYNTAX);
        		return 0;
        	}
        	if(expr_retval <= 0)
        	{
			update_error(BAD_LN);
        		return 0;
        	}
		current_token = get_next_token(input);
        	if(current_token.type != RPAREN)
        	{
			update_error(SYNTAX);
        		return 0;
        	}
        	*retval = lnf(expr_retval);
        }
        else if(current_token.type == SIN)
        {
        	double expr_retval;

        	current_token = get_next_token(input);
        	if(current_token.type != LPAREN)
        	{
			update_error(SYNTAX);
        		return 0;
        	}

        	if(!parse_expr(input, &expr_retval))
        	{
			update_error(SYNTAX);
        		return 0;
        	}
        	current_token = get_next_token(input);
        	if(current_token.type != RPAREN)
        	{
			update_error(SYNTAX);
        		return 0;
        	}
        	*retval = sine(expr_retval);
        }
        else if(current_token.type == COS)
        {
        	double expr_retval;

        	current_token = get_next_token(input);
        	if(current_token.type != LPAREN)
        	{
			update_error(SYNTAX);
        		return 0;
        	}

        	if(!parse_expr(input, &expr_retval))
        	{
        		update_error(SYNTAX);
        		return 0;
        	}
        	current_token = get_next_token(input);
        	if(current_token.type != RPAREN)
        	{
        		update_error(SYNTAX);
        		return 0;
        	}
        	*retval = cosine(expr_retval);
        }
        else if(current_token.type == RCONST)
        {
                *retval = current_token.rconst_value;
                return 1;
        }
        else if(current_token.type == ANS)
        {
        	*retval = ans;
        	return 1;
        }
        else
        {
		update_error(SYNTAX);
                return 0;
        }
}
