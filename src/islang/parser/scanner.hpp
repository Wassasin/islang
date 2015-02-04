#pragma once

#include <cstddef>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Preprocessor result, not distributed
#include "parser_lr.hh"

namespace islang
{

class scanner : public yyFlexLexer
{
private:
	parser_lr::semantic_type *yylval;

public:
	scanner(std::istream& in)
		: yyFlexLexer(&in)
		, yylval(nullptr)
	{}

	int yylex(parser_lr::semantic_type *lval)
	{
		yylval = lval;
		return( yylex() );
	}

private:
	int yylex();
};

}
