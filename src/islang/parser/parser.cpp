#include <islang/parser/parser.hpp>

#include <sstream>
#include <islang/parser/scanner.hpp>

namespace islang
{

ast::program* parser::parse(source* src, errorhandler& eh)
{
	ast::program* result;

	std::stringstream is;
	is << *src->buf;

	scanner s(is);
	parser_lr p(s, src, eh, result);

	if(p.parse() != 0)
		return nullptr;

	return result;
}

}
