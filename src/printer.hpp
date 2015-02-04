#pragma once

#include <ostream>

#include "common/ast.hpp"

class printer
{
	std::ostream& os;

public:
	typedef void result_type;

	printer(std::ostream& os)
		: os(os)
	{}

	result_type operator()(const ast::name& rhs, bool /*toplevel*/ = false) const
	{
		os << rhs.str;
	}

	result_type operator()(const ast::type_expr& rhs, bool toplevel = false) const
	{
		if(rhs.args.size() > 0 && !toplevel)
			os << '(';

		this->operator ()(rhs.t);

		for(auto arg : rhs.args)
		{
			os << ' ';
			this->operator ()(arg);
		}

		if(rhs.args.size() > 0 && !toplevel)
			os << ')';
	}

	result_type operator()(const ast::datadecl_coproduct& rhs, bool /*toplevel*/ = false) const
	{
		this->operator ()(rhs.constr);

		for(auto product : rhs.products)
		{
			os << ' ';
			this->operator ()(product, true);
		}
	}

	result_type operator()(const ast::datadecl& rhs, bool /*toplevel*/ = false) const
	{
		os << "data ";
		this->operator ()(rhs.n);

		for(auto arg : rhs.arguments)
		{
			os << ' ';
			this->operator ()(arg);
		}

		os << " = ";

		{
			bool first = true;
			for(auto cp : rhs.coproducts)
			{
				if(first)
					first = false;
				else
					os << " | ";

				this->operator ()(cp);
			}
		}
	}

	result_type operator()(const ast::typedefdecl& rhs, bool /*toplevel*/ = false) const
	{
		os << "typedef ";
		this->operator ()(rhs.n);

		for(auto arg : rhs.arguments)
		{
			os << ' ';
			this->operator ()(arg);
		}

		os << " = ";
		this->operator ()(rhs.t, true);
	}

	result_type operator()(const ast::program& rhs, bool /*toplevel*/ = false) const
	{
		for(auto decl : rhs.decls)
		{
			boost::apply_visitor(*this, decl);
			os << std::endl;
		}
	}
};

