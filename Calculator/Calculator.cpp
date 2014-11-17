// Calculator.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "Calculator.h"
#include <sstream>

using namespace std;

struct Token
{
	enum TokenType
	{
		NUMBER,
		ADD_OPERATOR,
		MULT_OPERATOR,
		END
	};

	Token(TokenType type, int start = 0, int end = 0) : type(type), start(start), end(end) {}
	TokenType type;
	int start;
	int end;
};

class ParsingError
{
public:
	ParsingError(const wstring&& message) : _message(message) { }
	const wstring& getMessage() const { return _message; }

private:
	wstring _message;
};

#define ThrowError(message) { \
	std::wstringstream err; \
	err << message << ends; \
	throw ParsingError(err.str()); \
}

struct ParsingContext
{
	ParsingContext(const wstring& str) : position(0), str(str)	{ }
	int position;
	const wstring& str;
	Token nextToken{ Token::END };

	Token readNextToken()
	{
		while (position < (int) str.length() && iswblank(str[position])) position++;
		if (position >= (int) str.length()) return Token(Token::END);

		auto c = str[position];
		if (iswdigit(c)) {
			int start = position;
			for (position++; position < (int) str.length() && iswdigit(str[position]); position++);
			return Token(Token::NUMBER, start, position);
		}
		if (c == '+' || c == '-')
		{
			int start = position++;
			return Token(Token::ADD_OPERATOR, start, position);
		}

		if (c == '*' || c == '/')
		{
			int start = position++;
			return Token(Token::MULT_OPERATOR, start, position);
		}

		ThrowError("Unrecognized character: " << c);
	}

	Token getNextToken()
	{
		if (nextToken.type != Token::END)
		{
			Token token = nextToken;
			nextToken.type = Token::END;
			return token;
		}

		return readNextToken();
	}

	Token peekNextToken()
	{
		auto token = getNextToken();
		nextToken = token;
		return token;
	}

	wstring getTokenText(const Token& token) const
	{
		auto startPtr = &str[token.start];
		return wstring(startPtr, startPtr + token.end - token.start);
	}
};

int evaluateNumber(const ParsingContext& ctx, const Token& number)
{
	auto numberText = ctx.getTokenText(number);
	return _wtoi(numberText.c_str());
}

int processOperation(int a, int b, wchar_t operation)
{
	switch (operation)
	{
	case L'+': return a + b;
	case L'-': return a - b;
	case L'*': return a * b;
	case L'/': return a / b;
	case L'^': return (int) pow(a, b);
	default:
		ThrowError("Unhandled operator: " << operation);
	}
}

// Grammar:
// expr		: mult_expr ( ('+' | '-' ) mult_expr)*
// mult_expr: operand ( ('*' | '/' ) operand )*
// operand	: Number

int parse_operand(ParsingContext& ctx)
{
	auto operand = ctx.getNextToken();
	if (operand.type != Token::NUMBER)
	{
		ThrowError("Unexpected token at position " << operand.start << ": '" << ctx.getTokenText(operand) << "'; number expected");
	}

	return evaluateNumber(ctx, operand);
}

wchar_t parse_operator(ParsingContext& ctx)
{
	auto operatorToken = ctx.getNextToken();
	if (operatorToken.type != Token::ADD_OPERATOR && operatorToken.type != Token::MULT_OPERATOR)
	{
		if (operatorToken.type == Token::END)
			return '\0';

		ThrowError("Unexpected token at position " << operatorToken.start << ": '" << ctx.getTokenText(operatorToken) << "'; number expected");
	}

	return ctx.str[operatorToken.start];
}

int parse_multexpr(ParsingContext& ctx)
{
	auto value = parse_operand(ctx);

	while (true)
	{
		auto nextToken = ctx.peekNextToken();
		if (nextToken.type != Token::MULT_OPERATOR)
			break;

		auto op = parse_operator(ctx);
		auto operandValue = parse_operand(ctx);
		value = processOperation(value, operandValue, op);
	}

	return value;
}

int parse_expr(ParsingContext& ctx)
{
	auto value = parse_multexpr(ctx);

	while (true)
	{
		auto nextToken = ctx.peekNextToken();
		if (nextToken.type != Token::ADD_OPERATOR)
			break;

		auto op = parse_operator(ctx);
		auto operandValue = parse_multexpr(ctx);
		value = processOperation(value, operandValue, op);
	}

	return value;
}

int calculate(const std::wstring& formula)
{
	ParsingContext ctx = {formula};

	try
	{
		return parse_expr(ctx);
	}
	catch(const ParsingError& error)
	{
		return 0;
	}
}