#include <islang/parser/parser.hpp>

#include <islang/parser/scanner.hpp>

namespace islang
{

ast::program* parser::parse(std::istream& is)
{
	ast::program* result;

	scanner s(is);
	parser_lr p(s, result);

	if(p.parse() != 0)
		throw std::runtime_error("Parse failed");

	return result;
}

}
