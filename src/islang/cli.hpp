#pragma once

#include <sstream>
#include <memory>
#include <fstream>

#include <boost/program_options.hpp>

#include <islang/printer.hpp>
#include <islang/parser/parser.hpp>

#include <islang/error/errorhandler.hpp>
#include <islang/common/termcmd.hpp>

namespace islang
{

class cli
{
private:
	cli() = delete;

	struct options
	{
		boost::optional<std::string> outputfile;
		std::vector<std::string> inputfiles;
	};

	static int interpret(options& opt, int argc, char** argv)
	{
		std::string outputfile;

		boost::program_options::options_description o_general("General options");
		o_general.add_options()
				("help,h", "display this message")
				("outputfile,o", boost::program_options::value(&outputfile), "file the generated code is written to, otherwise to std::cout");

		boost::program_options::options_description o_hidden("Hidden options");
		o_hidden.add_options()
				("inputfile,i", boost::program_options::value(&opt.inputfiles));

		boost::program_options::variables_map vm;
		boost::program_options::positional_options_description pos;
		pos.add("inputfile", -1);

		boost::program_options::options_description options("Allowed options");
		options.add(o_general);
		options.add(o_hidden);

		try
		{
			boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pos).run(), vm);
		} catch(boost::program_options::unknown_option &e)
		{
			std::cerr << "Unknown option --" << e.get_option_name() << ", see --help." << std::endl;
			return EXIT_FAILURE;
		}

		try
		{
			boost::program_options::notify(vm);
		} catch(const boost::program_options::required_option &e)
		{
			std::cerr << "You forgot this: " << e.what() << std::endl;
			return EXIT_FAILURE;
		}

		if(vm.count("help"))
		{
			std::cout
					<< "Inductive Scripting Language. [https://github.com/Wassasin/islang]" << std::endl
					<< "Usage: ./islang [options] <input file> [<input file> ...]" << std::endl
					<< std::endl
					<< o_general;

			return EXIT_FAILURE;
		}

		if(!vm.count("inputfile"))
		{
			std::cerr << "Please specify an input file, see --help. " << std::endl;
			return EXIT_FAILURE;
		}

		if(outputfile != "")
			opt.outputfile = outputfile;

		return EXIT_SUCCESS;
	}

	static inline void read_stream(std::istream& is, std::ostream& os)
	{
		std::string str;
		while(std::getline(is, str))
			os << str << '\n';
	}

	static int act(const options& opt)
	{
		errorhandler eh;

		for(auto inputfile : opt.inputfiles)
		{
			std::ostringstream ss;

			if(inputfile == "-")
			{
				read_stream(std::cin, ss);
				inputfile = "standard input";
			}
			else
			{
				std::ifstream is(inputfile);
				if(!is.is_open())
				{
					eh.notify(generic_message(std::string("input error, could not open file: ") + inputfile));
					continue;
				}
				read_stream(is, ss);
			}

			source s(ss, inputfile);
			std::unique_ptr<ast::program> prog(parser::parse(&s, eh));

			if(eh.should_stop())
				continue;

			if(prog)
			{
				std::cerr << termcmd::BOLD << "Managed to produce the following AST:" << termcmd::RESET << std::endl;
				printer p(std::cout); // TODO: select proper output stream / file
				p(*prog);
			}
		}

		if(opt.outputfile)
			std::cerr << "Would've written to " << opt.outputfile.get() << std::endl;
		else
			std::cerr << "Would've written to std::cout" << std::endl;

		if(!eh.milestone())
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
	}

public:
	static int run(int argc, char** argv)
	{
		options opt;
		int result = interpret(opt, argc, argv);

		if(result != EXIT_SUCCESS)
			return result;

		return act(opt);
	}
};

}
