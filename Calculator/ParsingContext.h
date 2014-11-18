#pragma once
#include <string>
#include "Token.h"

namespace Parsing
{
	class ParsingContext
	{
	public:
		ParsingContext(const std::wstring& str);
		const std::wstring& getString() const { return _str; }
	private:
		int _position;
		const std::wstring& _str;
		Token _savedToken;

		friend class Tokenizer;
	};
}
