#pragma once

#include <islang/error/islang_error.hpp>

namespace islang
{

class parse_error : public islang_error
{
	std::string message;
	source_span loc;

public:
	parse_error(std::string message, source_span loc)
		: message(message)
		, loc(loc)
	{}

	virtual boost::optional<source_span> location() const override
	{
		return loc;
	}

	virtual severity_e severity() const override
	{
		return severity_e::ERROR;
	}

	virtual std::ostream& print(std::ostream& os) const override
	{
		return os << message << std::endl;
	}

	virtual ~parse_error() = default;
};

}
