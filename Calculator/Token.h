#pragma once

namespace Parsing
{
	class ParsingContext;

	enum class TokenType
	{
		NONE,
		END,
		NUMBER,
		IDENTIFIER,
		ADD_OPERATOR,
		MULT_OPERATOR,
		OPEN_PARENT,
		CLOSE_PARENT
	};

	class Token
	{
	public:
		Token(const ParsingContext* ctx,
			TokenType type,
			int start,
			int end = -1);

		TokenType getType() const { return _type; }
		int getPosition() const { return _start; }
		bool isEmpty() const { return _text.empty(); }
		const std::wstring& getText() const { return _text; }
		wchar_t getFirstChar() const;

		static const Token None;
	private:
		Token();
		TokenType _type;
		int _start;
		int _end;

		std::wstring _text;
	};

	template<class T, class U>
	std::basic_ostream<T,U>& operator<<(std::basic_ostream<T, U>& ostream, Token token)
	{
		return ostream << L"'" << token.getText() << L"' (position: " << token.getPosition() << L")";
	}
}