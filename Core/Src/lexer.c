#include "lexer.h"

int has_last_token = 0;
struct Token last_token;

static struct Token _get_next_token(char **input)
{
	struct Token out;
	char *new_input = *input;
	char c;
	if(has_last_token)
	{
		has_last_token = 0;
	
	}
	out.rconst_value = 0.0;
	c = *(new_input++);
	if(c == '\0')
	{
		out.type = END;
		return out;
	}
	else if(c == '(')
	{
		out.type = LPAREN;
		*input = new_input;
		return out;
	}
	else if(c == ')')
	{
		out.type = RPAREN;
		*input = new_input;
		return out;
	}
	else if(c == '+')
	{
		out.type = PLUS;
		*input = new_input;
		return out;
	}
	else if(c == '-')
	{
		out.type = MINUS;
		*input = new_input;
		return out;
	}
	else if(c == '*')
	{
		out.type = MULT;
		*input = new_input;
		return out;
	}
	else if(c == '/')
	{
		out.type = DIV;
		*input = new_input;
		return out;
	}
	else if(c == '^')
	{
		out.type = EXPONENT;
		*input = new_input;
		return out;
	}
	else if(c == 'A')
	{
		if(*(new_input++) != 'n')
		{
			out.type = ERR;
			return out;
		}
		if(*(new_input++) != 's')
		{
			out.type = ERR;
			return out;
		}
		out.type = ANS;
		*input = new_input;
		return out;
	}
	else if(c == 's')
	{
		if(*(new_input++) != 'i')
		{
			out.type = ERR;
			return out;
		}
		if(*(new_input++) != 'n')
		{
			out.type = ERR;
			return out;
		}
		out.type = SIN;
		*input = new_input;
		return out;
	}
	else if(c == 'c')
	{
		if(*(new_input++) != 'o')
		{
			out.type = ERR;
			return out;
		}
		if(*(new_input++) != 's')
		{
			out.type = ERR;
			return out;
		}
		out.type = COS;
		*input = new_input;
		return out;
	}
	else if(c == 'l')
	{
		if(*(new_input++) != 'n')
		{
			out.type = ERR;
			return out;
		}
		out.type = LN;
		*input = new_input;
		return out;
	}
	else if(c == 'p') /* pi */
	{
		out.type = RCONST;
		out.rconst_value = 3.14159265358979;
		*input = new_input;
		return out;
	}
	else if(c == 'e') /* e */
	{
		out.type = RCONST;
		out.rconst_value = 2.71828182845905;
		*input = new_input;
		return out;
	}
	else if(c >= '0' && c <= '9')
	{
		double integral_part = (c - '0');
		double decimal_part = 0.0;
		double magnitude = 1.0;
		out.type = RCONST;

		while((c = *new_input) >= '0' && c <= '9')
		{

			integral_part *= 10;
			integral_part += (c - '0');
			new_input++;
		}
		if(c != '.')
		{
			out.rconst_value = integral_part;
			*input = new_input;
			return out;
		}
		new_input++;
		while((c = *new_input) >= '0' && c <= '9')
		{
			magnitude *= 10;
			decimal_part += (*new_input - '0') / magnitude;
			new_input++;
		}
		out.rconst_value = integral_part + decimal_part;
		*input = new_input;
		return out;
	}
	else
	{
		out.type = ERR;
		return out;
	}
}

struct Token get_next_token(char **input)
{
	struct Token out;
	
	if(has_last_token)
	{
		has_last_token = 0;
		return last_token;
	}

	out = _get_next_token(input);	
	last_token = out;

	return out;
}
void unget_token(void)
{
	has_last_token = 1;
}
