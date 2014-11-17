#pragma once
#include <sstream>
#include "ParsingException.h"

#define ThrowError(message) { \
	std::wstringstream err; \
	err << message << ends; \
	throw ParsingException(err.str()); \
}
