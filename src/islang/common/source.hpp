#pragma once

namespace islang
{

struct source_location
{
	std::size_t line, column;

	source_location(std::size_t line, std::size_t column)
		: line(line)
		, column(column)
	{}
};

struct source_span
{
	source_location start, end;
	
	source_span(std::size_t sline, std::size_t scolumn, std::size_t eline, std::size_t ecolumn)
		: start(sline, scolumn)
		, end(eline, ecolumn)
	{}
};

}
