#pragma once
#include <string>
#include "Token.h"

namespace Parsing
{
	class ParsingContext;

	class Tokenizer
	{
	public:
		Token getNextToken(ParsingContext& ctx) const;
		Token peekNextToken(ParsingContext& ctx) const;

	private:
		Token readNextToken(ParsingContext& ctx) const;
	};
}
