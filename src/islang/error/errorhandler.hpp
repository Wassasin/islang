#pragma once

#include <sstream>
#include <islang/common/termcmd.hpp>
#include <islang/error/islang_error.hpp>

namespace islang
{

class errorhandler
{
private:
	std::ostringstream ss;
	std::size_t error_count, warning_count, notice_count;

public:
	errorhandler()
		: ss()
		, error_count(0)
		, warning_count(0)
		, notice_count(0)
	{}

	void notify(const islang_error& err)
	{
		auto loc = err.location();
		ss << termcmd::BOLD;
		if(loc)
			ss << loc->src->name << ':' << loc->span.begin.line << ':' << loc->span.begin.column << ": ";
		else
			ss << "internal: ";

		switch(err.severity())
		{
		case islang_error::severity_e::ERROR:
			error_count++;
			ss << termcmd::RED << "error: ";
			break;
		case islang_error::severity_e::WARNING:
			warning_count++;
			ss << termcmd::YELLOW << "warning: ";
			break;
		case islang_error::severity_e::NOTICE:
			notice_count++;
			ss << termcmd::GREEN << "notice: ";
			break;
		}

		ss << termcmd::RESET;
		ss << err;

		if(loc)
			ss << *loc;
	}

	bool should_stop() const
	{
		return (error_count > 0);
	}

	bool milestone() const
	{
		if(!should_stop())
			return true;

		std::cerr << ss.str();

		return false;
	}
};

}
