#pragma once

namespace islang
{

struct source_location
{
	std::size_t line, column;
};

struct source_span
{
	source_location start, end;
};

}
