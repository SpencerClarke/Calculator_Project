enum Token_Type
{
	RCONST,
	PLUS,
	MINUS,
	MULT,
	DIV,
	LPAREN,
	ANS,
	RPAREN,
	EXPONENT,
	END,
	LN,
	SIN,
	COS,
	ERR
};

struct Token
{
	enum Token_Type type;
	double rconst_value;
};

extern int has_last_token;
extern struct Token last_token;

struct Token get_next_token(char **input);
void unget_token(void); /* Can only put back 1 token */
