#include <islang/parser/parser.hpp>

#include <islang/parser/scanner.hpp>

namespace islang
{

ast::program* parser::parse(std::istream& is, errorhandler& eh)
{
	ast::program* result;

	scanner s(is);
	parser_lr p(s, eh, result);

	if(p.parse() != 0)
		return nullptr;

	return result;
}

}
