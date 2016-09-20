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
		ParsingContext& operator=(const ParsingContext&) = delete;
		int getPosition() { return _position; }
	private:
		int _position;
		const std::wstring& _str;
		Token _savedToken;

		friend class Tokenizer;
	};
}
