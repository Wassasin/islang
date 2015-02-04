#include <sstream>
#include <memory>

#include <islang/common/ast.hpp>
#include <islang/printer.hpp>

#include <islang/parser/parser.hpp>

int main()
{
	using namespace islang;

	std::stringstream is;
	is << "data tree a b := Left a | Right b" << std::endl;
	is << "data days := Monday | Tuesday | Wednesday | Thursday | Friday | Saturday | Sunday" << std::endl;

	{
		std::unique_ptr<ast::program> prog(parser::parse(is));

		printer p(std::cout);
		p(*prog);
	}

	return 0;
}