#pragma once

namespace islang
{

enum class termcmd
{
	RESET = 0,
	BOLD = 1,
	RED = 31,
	YELLOW = 33,
	GREEN = 32
};

inline std::ostream& operator<<(std::ostream& os, termcmd rhs)
{
	return os << "\033[" << (int)rhs << "m";
}

}
