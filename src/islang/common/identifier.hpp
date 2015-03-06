#pragma once

#include <islang/common/source.hpp>

namespace islang {

class identifier_repository;

class identifier_ref
{
private:
	size_t i;

	identifier_ref(size_t i)
		: i(i)
	{}

	friend class identifier_repository;

public:
	identifier_ref(const identifier_ref& rhs)
		: i(rhs.i)
	{}

	bool operator==(identifier_ref rhs) const
	{
		return(i == rhs.i);
	}

	bool operator<(identifier_ref rhs) const
	{
		return(i < rhs.i);
	}
};

class identifier
{
public:
	const std::string name;
	const boost::optional<source_location> loc;
	const identifier_ref ref;

private:
	identifier(std::string name, boost::optional<source_location> loc, identifier_ref ref)
		: name(name)
		, loc(loc)
		, ref(ref)
	{}

	friend class identifier_repository;
};

class identifier_repository
{
private:
	size_t i;
	std::vector<identifier> ids;

private:
	// Disallow copy
	identifier_repository(identifier_repository&) = delete;
	identifier_repository& operator=(identifier_repository&) = delete;

public:
	identifier_repository()
		: i(0)
		, ids()
	{}

	identifier create(std::string name, boost::optional<source_location> loc = boost::none)
	{
		identifier id(name, loc, identifier_ref(i++));
		ids.push_back(id);
		return id;
	}

	identifier operator[](identifier_ref ref) const
	{
		return ids.at(ref.i);
	}
};

}
