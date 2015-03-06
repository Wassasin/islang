#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <islang/common/source.hpp>
#include <islang/common/identifier.hpp>

namespace islang { namespace ast
{

struct node {
	boost::optional<source_location> src;

	node()
		: src()
	{}
};

struct name : node {
	std::string str;
	boost::optional<identifier_ref> id;

	name(std::string str)
		: str(str)
		, id()
	{}
};

struct constructor : name { using name::name; };
struct type_name : name { using name::name; };
struct property_name : name {using name::name; };

struct type_expr : node {
	type_name t;
	std::vector<type_expr> args;

	type_expr(decltype(t) t)
		: t(t)
		, args()
	{}

	type_expr(decltype(t) t, decltype(args) args)
		: t(t)
		, args(args)
	{}
};

struct datadecl_product : node {
	boost::optional<property_name> name;
	type_expr e;

	datadecl_product(decltype(e) e)
		: name()
		, e(e)
	{}

	datadecl_product(decltype(name) name, decltype(e) e)
		: name(name)
		, e(e)
	{}
};

struct datadecl_coproduct : node {
	constructor constr;
	std::vector<datadecl_product> products;

	datadecl_coproduct(decltype(constr) constr, decltype(products) products)
		: constr(constr)
		, products(products)
	{}
};

struct datadecl : node {
	type_name n;
	std::vector<type_name> arguments;
	std::vector<datadecl_coproduct> coproducts;

	datadecl(decltype(n) n, decltype(arguments) arguments, decltype(coproducts) coproducts)
		: n(n)
		, arguments(arguments)
		, coproducts(coproducts)
	{}
};

struct typedefdecl : node {
	type_name n;
	std::vector<type_name> arguments;
	type_expr t;

	typedefdecl(decltype(n) n, decltype(t) t)
		: n(n)
		, arguments()
		, t(t)
	{}

	typedefdecl(decltype(n) n, decltype(arguments) arguments, decltype(t) t)
		: n(n)
		, arguments(arguments)
		, t(t)
	{}
};

typedef boost::variant<
	datadecl,
	typedefdecl
> decl;

struct program : node {
	std::vector<decl> decls;

	program()
		: decls()
	{}

	program(decltype(decls) decls)
		: decls(decls)
	{}
};

}}
