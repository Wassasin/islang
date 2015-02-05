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
	source_location begin, end;
	
	source_span(source_location begin, source_location end)
		: begin(begin)
		, end(end)
	{}

	source_span(std::size_t bline, std::size_t bcolumn, std::size_t eline, std::size_t ecolumn)
		: begin(bline, bcolumn)
		, end(eline, ecolumn)
	{}
};

}
