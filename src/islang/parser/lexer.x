%{
#include <string>
#include <islang/parser/scanner.hpp>

/* typedef to make the returns for the tokens shorter */
typedef islang::parser_lr::token token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return(token::END)

%}

%option debug 
%option nodefault 
%option yyclass="islang::scanner"
%option noyywrap
%option yylineno
%option c++

%%
data        {
                return(token::KW_DATA);
            }

[a-zA-Z]+   {
                yylval->str = new std::string(yytext);
                return(token::NAME);
            }

:=          {
                return(token::DECLARATION);
            }

\|          {
                return(token::BAR);
            }
   
[ \t\n]+	/* eat up whitespace */

.	        {
                printf("Unrecognized character: %s\n", yytext);
            }
%%
