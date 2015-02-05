%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines
%locations
%define parse.error verbose
%define api.namespace {islang}
%define parser_class_name {parser_lr}

%code requires{
    #include <islang/common/ast.hpp>

    namespace islang {
        class scanner;
        class errorhandler;
    }
    
    using namespace islang;
}

%parse-param { scanner& s  }
%parse-param { errorhandler& eh }
%parse-param { ast::program*& r }

%code{
    #include <iostream>
    #include <cstdlib>

    #include <islang/common/ast.hpp>
    #include <islang/common/source.hpp>
    #include <islang/parser/scanner.hpp>
    #include <islang/error/errorhandler.hpp>
    #include <islang/error/parse_error.hpp>

    #undef yylex
    #define yylex s.yylex
    
    #define moveptr(src, dest) { dest = src; src = nullptr; }
    #define clean(ptr) { if(ptr) { delete ptr; ptr = nullptr; } }
    #define mark(loc, dest) { dest->sp = source_span(loc.begin.line, loc.begin.column, loc.end.line, loc.end.column); }
}

/* return types */
%union {
    std::string* str;
    
    ast::constructor* constructor;
    ast::type_name* type_name;
    
    ast::type_expr* type_expr;
    
    ast::datadecl_coproduct* datadecl_coproduct;
    ast::datadecl* datadecl;
    
    ast::decl* decl;
    
    std::vector<ast::type_name>* type_names;
    std::vector<ast::type_expr>* type_exprs;
    std::vector<ast::datadecl_coproduct>* datadecl_coproducts;
    std::vector<ast::decl>* decls;
}

/* literals */
%token              END         0   "end of file"
%token              KW_DATA         "keyword data"
%token<str>         NAME            "name"
%token              DECLARATION     "symbol ="
%token              BAR             "symbol |"
%token              NEWLINE         "newline"

/* binding of types to non-literals */
%type<constructor>          constructor
%type<type_name>            type_name
%type<type_expr>            type_expr
%type<datadecl_coproduct>   datadecl_coproduct
%type<datadecl>             datadecl
%type<decl>                 decl
%type<type_names>           datadecl_arguments
%type<type_exprs>           type_exprs
%type<datadecl_coproducts>  datadecl_coproducts datadecl_coproducts1
%type<decls>                decls

/* on error clean up */
%destructor { clean($$); } <*>

%%

program
    : nlopt END { r = new ast::program(); mark(@$, r); }
    | decls nlopt END { r = new ast::program(*$1); clean($1); mark(@$, r); }
    ;



decls
    : decl { $$ = new std::vector<ast::decl>(); $$->emplace_back(*$1); clean($1); }
    | decls NEWLINE decl { moveptr($1, $$); $$->emplace_back(*$3); clean($1); clean($3); }
    | decls NEWLINE error { moveptr($1, $$); clean($1); }
    ;

decl
    : datadecl { $$ = new ast::decl(*$1); clean($1); }
    ;

/* datadecl */
datadecl
    : KW_DATA type_name datadecl_arguments DECLARATION datadecl_coproducts { $$ = new ast::datadecl(*$2, *$3, *$5); clean($2); clean($3); clean($5); mark(@$, $$); }
    ;

datadecl_coproduct
    : constructor type_exprs { $$ = new ast::datadecl_coproduct(*$1, *$2); clean($1); clean($2); mark(@$, $$); }
    ;

datadecl_coproducts
    : datadecl_coproducts1 datadecl_coproduct { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;

datadecl_coproducts1
    : /* empty */ { $$ = new std::vector<ast::datadecl_coproduct>(); }
    | datadecl_coproducts1 datadecl_coproduct BAR { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;

datadecl_arguments
    : /* empty */ { $$ = new std::vector<ast::type_name>(); }
    | datadecl_arguments type_name { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;
    
/* type expr */
type_exprs
    : /* empty */ { $$ = new std::vector<ast::type_expr>(); }
    | type_exprs type_expr { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;
    
type_expr
    : NAME type_exprs { $$ = new ast::type_expr(*$1, *$2); clean($1); clean($2); mark(@$, $$); }
    ;

/* low level elements */
type_name
    : NAME { $$ = new ast::type_name(*$1); clean($1); mark(@$, $$); }
    ;
    
constructor
    : NAME { $$ = new ast::constructor(*$1); clean($1); mark(@$, $$); }
    ;
    
nlopt
    : /* empty */
    | NEWLINE
    ;

%%


void islang::parser_lr::error(const location& loc, const std::string& err_message)
{
    source_location begin(loc.begin.line, loc.begin.column);
    source_location end(loc.end.line, loc.end.column);

    

    eh.notify(parse_error(err_message, source_span(begin, end)));
}
