#pragma once

#include <deque>
#include <map>

#include <islang/error/errorhandler.hpp>
#include <islang/common/ast_walker.hpp>

namespace islang
{

class semantic_analysis : ast_unit_walker
{
private:
	class context
	{
		// Stack with reverse order traversal
		std::deque<std::map<std::string, identifier_ref>> s;

	public:
		context() {}

		void create_layer()
		{
			s.emplace_front();
		}

		void forget_layer()
		{
			s.pop_front();
		}

		boost::optional<identifier_ref> get(std::string name) const
		{
			for(const auto& layer : s) // Iterate from most specific contextlayer to global contextlayer.
			{
				auto it = layer.find(name);
				if(it != layer.end())
					return it->second;
			}

			return boost::none;
		}

		boost::optional<identifier_ref> get_local(std::string name) const
		{
			auto layer = s.front();
			auto it = layer.find(name);
			if(it != layer.end())
				return it->second;

			return boost::none;
		}

		void insert(identifier id)
		{
			s.front().emplace(id.name, id.ref);
		}

		void insert_global(identifier id)
		{
			s.back().emplace(id.name, id.ref);
		}

		boost::optional<identifier_ref> operator[](std::string name) const
		{
			return get(name);
		}

		void erase_local(std::string name)
		{
			s.front().erase(name);
		}
	};

	identifier_repository& ir;
	errorhandler& eh;

	context typec;
	context exprc;

	semantic_analysis(identifier_repository& ir, errorhandler& eh)
		: ir(ir)
		, eh(eh)
		, typec()
		, exprc()
	{
		typec.create_layer();
		exprc.create_layer();
	}

	using ast_unit_walker::operator ();

	identifier create(ast::name& rhs)
	{
		identifier id(ir.create(rhs.str, rhs.src));
		rhs.id = id.ref;
		return id;
	}

	void insert(identifier id, std::string type, context& c) const
	{
		auto ref_local = c.get_local(id.name);
		if(ref_local)
		{
			eh.notify(generic_message("identifier for "+type+" already in use:", id.loc, islang_error::severity_e::ERROR));

			auto orig_loc = ir[*ref_local].loc;
			if(orig_loc)
				eh.notify(generic_message("original declaration here:", orig_loc, islang_error::severity_e::NOTICE));

			c.erase_local(id.name); // Fatal error, but overwrite the original identifier in any case.
		}
		else
		{
			auto ref = c.get(id.name);
			if(ref)
			{
				eh.notify(generic_message("identifier for "+type+" shadowed:", id.loc, islang_error::severity_e::WARNING));

				auto orig_loc = ir[*ref].loc;
				if(orig_loc)
					eh.notify(generic_message("earlier declaration here:", orig_loc, islang_error::severity_e::NOTICE));

				// Continue as if nothing happened.
			}
		}

		c.insert(id);
	}

	void link(ast::type_name& name, std::string type, const context& c) const
	{
		auto ref = c[name.str];

		if(!ref)
		{
			eh.notify(generic_message(std::string("undeclared identifier in ")+type, name.src));
			return;
		}

		name.id = *ref;
	}

	result_type operator ()(ast::type_expr& rhs)
	{
		link(rhs.t, "type expression", typec);

		for(ast::type_expr& te : rhs.args)
			this->operator() (te);

		return zero();
	}

	result_type operator ()(ast::typedefdecl& rhs)
	{
		insert(create(rhs.n), "typedef", typec);

		typec.create_layer();
		for(ast::type_name& arg : rhs.arguments)
			insert(create(arg), "typedef argument", typec);

		this->operator() (rhs.t);

		typec.forget_layer();
		return zero();
	}

	result_type operator ()(ast::property_name& rhs)
	{
		insert(create(rhs), "property name", exprc);
		return zero();
	}

	result_type operator ()(ast::datadecl_coproduct& rhs)
	{
		insert(create(rhs.constr), "constructor", exprc);

		exprc.create_layer();
		for(ast::datadecl_product prod : rhs.products)
			this->operator() (prod);
		exprc.forget_layer();

		return zero();
	}

	result_type operator ()(ast::datadecl& rhs)
	{
		insert(create(rhs.n), "data declaration", typec);

		typec.create_layer();
		for(ast::type_name& arg : rhs.arguments)
			insert(create(arg), "data declaration argument", typec);

		for(ast::datadecl_coproduct& coprod : rhs.coproducts)
			this->operator() (coprod);

		typec.forget_layer();

		return zero();
	}

public:
	static void analyse(ast::program& prog, identifier_repository& ir, errorhandler& eh)
	{
		semantic_analysis sa(ir, eh);
		sa.operator ()(prog);
	}
};

}
