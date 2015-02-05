%{
#include <string>
#include <islang/parser/scanner.hpp>

typedef islang::parser_lr::token token;
#define yyterminate() return(token::END)
#define YY_USER_ACTION { update_loc(); }

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
   
[\n+]       {
                column = 1;
                return(token::NEWLINE);
            }
   
[ \t]+      /* eat up whitespace */

.	        {
                printf("Unrecognized character: %s\n", yytext);
            }
%%
