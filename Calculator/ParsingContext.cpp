#include "pch.h"
#include "ParsingContext.h"

using namespace Parsing;

ParsingContext::ParsingContext(const std::wstring & str) : _str{ str }, _position{}, _savedToken{ nullptr, TokenType::END, 0 }
{
}
