#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "common/source.hpp"

namespace ast
{

struct node {
	boost::optional<source_span> sp;

	node()
		: sp()
	{}
};

struct name : node {
	std::string str;

	name(std::string str)
		: str(str)
	{}
};

struct constructor : name { using name::name; };
struct type_name : name { using name::name; };
struct property_name : name { using name::name; };

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
	boost::optional<property_name> n;
	type_expr t;

	datadecl_product(decltype(t) t)
		: n()
		, t(t)
	{}

	datadecl_product(property_name n, decltype(t) t)
		: n(n)
		, t(t)
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
	name n;
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

struct program : node {
	std::vector<boost::variant<
		datadecl,
		typedefdecl
	>> decls;

	program(decltype(decls) decls)
		: decls(decls)
	{}
};

}
