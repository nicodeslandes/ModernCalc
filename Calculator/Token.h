#pragma once

namespace Parsing
{
	class ParsingContext;

	enum class TokenType
	{
		NONE,
		END,
		NUMBER,
		ADD_OPERATOR,
		MULT_OPERATOR,
		OPEN_PARENT,
		CLOSE_PARENT
	};

	struct Token
	{
		Token(const ParsingContext* ctx,
			TokenType type,
			int start,
			int end = -1) :
			_ctx(ctx), type(type), start(start), end(end != -1 ? end : start) {}

		//Token(const Token&) = default;
		//Token& operator=(const Token&) = default;

		std::wstring getText() const;
		wchar_t getFirstChar() const;

		TokenType type;
		int start;
		int end;

		const ParsingContext* _ctx;
	};
}