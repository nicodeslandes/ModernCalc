#include "pch.h"
#include "ParsingContext.h"


ParsingContext::ParsingContext(const std::wstring & str) : _str{ str }, _position{}, _savedToken{ nullptr, Token::END, 0 }
{
}
