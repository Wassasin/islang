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

}
