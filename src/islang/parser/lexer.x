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
%option c++

%%
data        { return(token::KW_DATA); }
typedef     { return(token::KW_TYPEDEF); }
[a-zA-Z]+   {
                yylval->str = new std::string(yytext);
                return(token::NAME);
            }
=           { return(token::DECLARATION); }
\|          { return(token::BAR); }
\:          { return(token::COLON); }
\,          { return(token::COMMA); }
\(          { return(token::PARENSL); }
\)          { return(token::PARENSR); }
\{          { return(token::BRACEL); }
\}          { return(token::BRACER); }
\n          {
                line++;
                column = 1;
                return(token::NEWLINE);
            }
[ \t]+      /* eat up whitespace */
.           { return(token::UNKNOWN); }
%%
