#include "pch.h"
#include "Token.h"
#include "ParsingContext.h"

using namespace std;
using namespace Parsing;

Token::Token(const ParsingContext* ctx,
	Parsing::TokenType type,
	int start,
	int end /* = -1*/) :
	_type(type),
	_start(start),
	_end(end != -1 ? end : start),
	_text(ctx ? ctx->getString().substr(start, end == -1 ? 0: end - start) : wstring())
{}

wchar_t Token::getFirstChar() const
{
	return _text[0];
}

Token::Token():
_type{ Parsing::TokenType::END },
_start{ 0 },
_end{ 0 }
{}

const Token Token::None;