#pragma once

#include <islang/common/ast.hpp>

namespace islang {

template<typename RESULT_TYPE>
class ast_walker
{
public:
	typedef RESULT_TYPE result_type;

protected:
	virtual result_type zero() = 0;
	virtual void merge(result_type& dest, result_type rhs) = 0;

public:
	virtual ~ast_walker() {}

	virtual result_type operator()(ast::name& /*rhs*/)
	{
		return zero();
	}

	virtual result_type operator()(ast::type_expr& rhs)
	{
		result_type r(zero());

		for(auto arg : rhs.args)
			merge(r, this->operator ()(arg));

		return r;
	}

	virtual result_type operator()(ast::datadecl_product& rhs)
	{
		result_type r(zero());

		if(rhs.name)
			merge(r, this->operator ()(rhs.name.get()));

		merge(r, this->operator ()(rhs.e));
		return r;
	}

	virtual result_type operator()(ast::datadecl_coproduct& rhs)
	{
		result_type r(zero());

		merge(r, this->operator ()(rhs.constr));

		for(auto product : rhs.products)
			merge(r, this->operator ()(product));

		return r;
	}

	virtual result_type operator()(ast::datadecl& rhs)
	{
		result_type r(zero());

		merge(r, this->operator ()(rhs.n));

		for(auto arg : rhs.arguments)
			merge(r, this->operator ()(arg));

		for(auto cp : rhs.coproducts)
			merge(r, this->operator ()(cp));

		return r;
	}

	virtual result_type operator()(ast::typedefdecl& rhs)
	{
		result_type r(zero());

		merge(r, this->operator ()(rhs.n));

		for(auto arg : rhs.arguments)
			merge(r, this->operator ()(arg));

		merge(r, this->operator ()(rhs.t));

		return r;
	}

	virtual result_type operator()(ast::program& rhs)
	{
		result_type r(zero());

		for(auto decl : rhs.decls)
			merge(r, boost::apply_visitor(*this, decl));

		return r;
	}
};

}
