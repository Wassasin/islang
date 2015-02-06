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
	parser_lr::location_type *yylloc;
	size_t line, column;

public:
	scanner(std::istream& in)
		: yyFlexLexer(&in)
		, yylval(nullptr)
		, line(1)
		, column(1)
	{}

	int yylex(parser_lr::semantic_type* lval, parser_lr::location_type* lloc)
	{
		yylval = lval;
		yylloc = lloc;

		return(yylex());
	}

private:
	void update_loc()
	{
		yylloc->begin.line = line;
		yylloc->begin.column = column;

		int length = YYLeng();
		if(length > 0)
			column += length;

		yylloc->end.line = line; // We do not have multiline tokens
		yylloc->end.column = column;
	}

	int yylex();
};

}
