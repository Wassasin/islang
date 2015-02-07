#pragma once

#include <ostream>
#include <boost/optional.hpp>
#include <islang/common/source.hpp>

namespace islang
{

class islang_error
{
public:
	enum class severity_e
	{
		ERROR,
		WARNING,
		NOTICE
	};

	virtual boost::optional<source_location> location() const
	{
		return boost::none;
	}

	virtual severity_e severity() const = 0;
	virtual std::ostream& print(std::ostream& os) const = 0;

	virtual ~islang_error() = default;
};

inline std::ostream& operator<<(std::ostream& os, const islang_error& rhs)
{
	return rhs.print(os);
}

class generic_message : public islang_error
{
	severity_e s;
	boost::optional<source_location> src;
	std::string message;

public:
	generic_message(std::string message)
		: s(severity_e::ERROR)
		, message(message)
	{}

	generic_message(severity_e s, std::string message)
		: s(s)
		, src()
		, message(message)
	{}

	generic_message(severity_e s, source_location src, std::string message)
		: s(s)
		, src(src)
		, message(message)
	{}

	virtual severity_e severity() const override
	{
		return s;
	}

	virtual boost::optional<source_location> location() const override
	{
		return src;
	}

	virtual std::ostream& print(std::ostream& os) const override
	{
		return os << message << std::endl;
	}
};

}
