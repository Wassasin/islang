#pragma once

#include <istream>
#include <islang/common/ast.hpp>

namespace islang
{

class parser
{
public:
	parser() = delete;

	static ast::program* parse(std::istream& is);
};

}
