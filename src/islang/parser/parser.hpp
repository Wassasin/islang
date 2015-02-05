#pragma once

#include <istream>
#include <islang/common/ast.hpp>
#include <islang/error/errorhandler.hpp>

namespace islang
{

class parser
{
public:
	parser() = delete;

	static ast::program* parse(std::istream& is, errorhandler& eh);
};

}
