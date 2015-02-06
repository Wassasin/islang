#include <sstream>
#include <memory>

#include <islang/printer.hpp>
#include <islang/parser/parser.hpp>

#include <islang/error/errorhandler.hpp>
#include <islang/common/termcmd.hpp>

int main()
{
	using namespace islang;

	std::ostringstream os;
	os << "data tree a b := Left a | 321 Right b" << std::endl;
	os << "data days := Monday data | Tuesday | Wednesday | Thursday | Friday | Saturday | Sunday" << std::endl;
	os << "data maybe a := Nothing | Just a" << std::endl;
	os << "data point := Single |" << std::endl;

	source s(os.str());

	errorhandler eh;
	std::unique_ptr<ast::program> prog(parser::parse(s, eh));

	int result = eh.milestone() ? EXIT_FAILURE : EXIT_SUCCESS;

	if(prog)
	{
		std::cerr << termcmd::BOLD << "Managed to salvage the following AST:" << termcmd::RESET << std::endl;
		printer p(std::cout);
		p(*prog);
	}

	return result;
}
