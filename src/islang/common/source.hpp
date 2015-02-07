#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <boost/optional.hpp>

#include <islang/common/termcmd.hpp>

namespace islang
{

struct source
{
	/* Immutable, noncopyable */
	const std::unique_ptr<const std::string> buf;
	const std::string name;

	source(const std::ostringstream& ss, std::string name)
		: buf(new std::string(ss.str()))
		, name(name)
	{}

	source(const std::ostringstream& ss)
		: source(ss, "internal buffer")
	{}
};

struct source_point
{
	std::size_t line, column;

	source_point(std::size_t line, std::size_t column)
		: line(line)
		, column(column)
	{}
};

struct source_span
{
	source_point begin, end;
	
	source_span(source_point begin, source_point end)
		: begin(begin)
		, end(end)
	{}

	source_span(std::size_t bline, std::size_t bcolumn, std::size_t eline, std::size_t ecolumn)
		: begin(bline, bcolumn)
		, end(eline, ecolumn)
	{}
};

struct source_location
{
	source* src;
	source_span span;

	source_location(source* src, source_span span)
		: src(src)
		, span(span)
	{}
};

inline std::ostream& operator<<(std::ostream& os, const source_location& rhs)
{
	std::istringstream is(*rhs.src->buf);

	assert(rhs.span.begin.line == rhs.span.end.line);

	std::string line;
	for(size_t i = 1; i <= rhs.span.begin.line; ++i)
		if(!std::getline(is, line))
			throw std::runtime_error("Source span does not correspond to source (not enough lines)");

	os << line << std::endl;

	for(size_t i = 1; i < rhs.span.begin.column; ++i)
		os << ' ';

	os << termcmd::BOLD << termcmd::GREEN;
	os << '^';

	for(size_t i = rhs.span.begin.column+1; i < rhs.span.end.column; ++i)
		os << '~';

	os << termcmd::RESET << std::endl;

	return os;
}

}
