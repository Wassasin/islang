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
%parse-param { source& src }
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
    ast::property_name* property_name;
    
    ast::type_expr* type_expr;
    
    ast::datadecl_product* datadecl_product;
    ast::datadecl_coproduct* datadecl_coproduct;
    ast::datadecl* datadecl;
    
    boost::optional<ast::decl>* decl_opt;
    
    std::vector<ast::type_name>* type_names;
    std::vector<ast::type_expr>* type_exprs;
    std::vector<ast::datadecl_product>* datadecl_products;
    std::vector<ast::datadecl_coproduct>* datadecl_coproducts;
    std::vector<ast::decl>* decls;
}

/* literals */
%token              END         0   "end of file"
%token              KW_DATA         "keyword data"
%token<str>         NAME            "name"
%token              DECLARATION     "assignment symbol ="
%token              BAR             "bar symbol |"
%token              COLON           "colon symbol :"
%token              COMMA           "comma ,"
%token              PARENSL         "left parenthesis ("
%token              PARENSR         "right parenthesis )"
%token              BRACEL          "left brace {"
%token              BRACER          "right brace }"
%token              NEWLINE         "newline"
%token              UNKNOWN         "unrecognized character"

/* binding of types to non-literals */
%type<constructor>          constructor
%type<type_name>            type_name
%type<property_name>        property_name
%type<type_expr>            type_expr type_expr_naked
%type<datadecl_product>     datadecl_product datadecl_product_named
%type<datadecl_coproduct>   datadecl_coproduct
%type<datadecl>             datadecl
%type<decl_opt>             decl
%type<type_names>           datadecl_arguments
%type<type_exprs>           type_exprs0
%type<datadecl_products>    datadecl_products0 datadecl_products1 datadecl_products_named_list datadecl_products_named_list0 datadecl_products_named_list1
%type<datadecl_coproducts>  datadecl_coproducts1 datadecl_coproducts0_followup datadecl_coproducts1_followup
%type<decls>                decls

/* on error clean up */
%destructor { clean($$); } <*>

%%

program
    : nlsopt END { r = new ast::program(); mark(@$, r); }
    | decls nlsopt END { r = new ast::program(*$1); clean($1); mark(@$, r); }
    ;

decls
    : decl { $$ = new std::vector<ast::decl>(); if(*$1) { $$->emplace_back(**$1); } clean($1); }
    | decls nls decl { moveptr($1, $$); if(*$3) { $$->emplace_back(**$3); } clean($1); clean($3); }
    ;

decl
    : datadecl { $$ = new boost::optional<ast::decl>(*$1); clean($1); }
    | errors { $$ = new boost::optional<ast::decl>(); yyerrok; }
    ;

/* datadecl */
datadecl
    : KW_DATA type_name datadecl_arguments DECLARATION datadecl_coproducts1 { $$ = new ast::datadecl(*$2, *$3, *$5); clean($2); clean($3); clean($5); mark(@$, $$); }
    ;

datadecl_product
    : type_expr { $$ = new ast::datadecl_product(*$1); clean($1); mark(@$, r); }
    ;

datadecl_product_named
    : type_expr_naked { $$ = new ast::datadecl_product(*$1); clean($1); mark(@$, r); }
    | property_name COLON type_expr_naked { $$ = new ast::datadecl_product(*$1, *$3); clean($1); clean($3); mark(@$, r); }
    ;

datadecl_products0
    : /* empty */ { $$ = new std::vector<ast::datadecl_product>(); }
    | datadecl_products1 { moveptr($1, $$); clean($1); }
    ;

datadecl_products1
    : datadecl_products0 datadecl_product { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    | datadecl_products0 datadecl_products_named_list { moveptr($1, $$); $$->insert($$->end(), $2->begin(), $2->end()); clean($1); clean($2); }
    ;

datadecl_products_named_list
    : BRACEL datadecl_products_named_list0 datadecl_product_named BRACER { moveptr($2, $$); $$->emplace_back(*$3); clean($2); clean($3); }
    ;

datadecl_products_named_list0
    : /* empty */ { $$ = new std::vector<ast::datadecl_product>(); }
    | datadecl_products_named_list1 COMMA { moveptr($1, $$); clean($1); }
    ;

datadecl_products_named_list1
    : datadecl_products_named_list0 datadecl_product_named { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;

datadecl_coproduct
    : constructor datadecl_products0 { $$ = new ast::datadecl_coproduct(*$1, *$2); clean($1); clean($2); mark(@$, $$); }
    ;

datadecl_coproducts1
    : datadecl_coproducts0_followup datadecl_coproduct { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;

datadecl_coproducts0_followup
    : /* empty */ { $$ = new std::vector<ast::datadecl_coproduct>(); }
    | datadecl_coproducts1_followup { moveptr($1, $$); clean($1); }
    ;

datadecl_coproducts1_followup
    : datadecl_coproducts0_followup datadecl_coproduct BAR { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;

datadecl_arguments
    : /* empty */ { $$ = new std::vector<ast::type_name>(); }
    | datadecl_arguments type_name { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;
    
/* type expr */
type_expr
    : NAME { $$ = new ast::type_expr(*$1); clean($1); mark(@$, $$); }
    | PARENSL type_expr_naked PARENSR { moveptr($2, $$); clean($2); }
    ;

type_expr_naked
    : NAME type_exprs0 { $$ = new ast::type_expr(*$1, *$2); clean($1); clean($2); mark(@$, $$); }
    ;

type_exprs0
    : /* empty */ { $$ = new std::vector<ast::type_expr>(); }
    | type_exprs0 type_expr { moveptr($1, $$); $$->emplace_back(*$2); clean($1); clean($2); }
    ;

/* low level elements */
type_name
    : NAME { $$ = new ast::type_name(*$1); clean($1); mark(@$, $$); }
    ;

property_name
    : NAME { $$ = new ast::property_name(*$1); clean($1); mark(@$, $$); }
    ;
    
constructor
    : NAME { $$ = new ast::constructor(*$1); clean($1); mark(@$, $$); }
    ;
    
nlsopt
    : /* empty */
    | nls
    ;

nls
    : NEWLINE
    | nls NEWLINE
    ;

errors
    : error
    | errors error

%%

void islang::parser_lr::error(const location& loc, const std::string& err_message)
{
    source_span span(loc.begin.line, loc.begin.column, loc.end.line, loc.end.column);
    eh.notify(parse_error(err_message, source_location(src, span)));
}
