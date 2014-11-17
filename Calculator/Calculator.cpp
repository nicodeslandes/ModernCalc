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
		END,
		NUMBER,
		ADD_OPERATOR,
		MULT_OPERATOR,
		OPEN_PARENT,
		CLOSE_PARENT
	};

	Token(TokenType type, int start, int end = -1) : type(type), start(start), end(end != -1 ? end : start) {}
	TokenType type;
	int start;
	int end;
};

#define ThrowError(message) { \
	std::wstringstream err; \
	err << message << ends; \
	throw ParsingError(err.str()); \
}

struct ParsingContext
{
	ParsingContext(const wstring& str) : position{ 0 }, str{ str }, nextToken{ Token::END, 0 } { }
	int position;
	const wstring& str;
	Token nextToken;

	Token readNextToken()
	{
		while (position < (int) str.length() && iswblank(str[position])) position++;
		if (position >= (int) str.length()) return Token(Token::END, position);

		auto c = str[position];
		if (iswdigit(c)) {
			int start = position;
			for (position++; position < (int) str.length() && iswdigit(str[position]); position++);
			return Token(Token::NUMBER, start, position);
		}

		auto tokenType = Token::END;

		switch (c)
		{
		case '+':
		case '-':
			tokenType = Token::ADD_OPERATOR;
			break;

		case '*':
		case '/':
			tokenType = Token::MULT_OPERATOR;
			break;

		case '(':
			tokenType = Token::OPEN_PARENT;
			break;

		case ')':
			tokenType = Token::CLOSE_PARENT;
			break;

		default:
			ThrowError("Unrecognized character at position " << position << ": '" << c << "'");
		}

		int start = position++;
		return Token(tokenType, start, position);
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

int parse_expr(ParsingContext& ctx);

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
//			| '(' expr ')'

int parse_operand(ParsingContext& ctx)
{
	auto token = ctx.getNextToken();
	if (token.type == Token::NUMBER)
		return evaluateNumber(ctx, token);

	if (token.type == Token::OPEN_PARENT)
	{
		auto value = parse_expr(ctx);
		token = ctx.getNextToken();
		if (token.type == Token::CLOSE_PARENT)
			return value;

		ThrowError("Missing closing parenthesis at position " << token.start);
	}

	ThrowError("Unexpected token at position " << token.start << ": '" << ctx.getTokenText(token) << "'; number expected");
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
	ParsingContext ctx(formula);

	auto result = parse_expr(ctx);
	auto token = ctx.peekNextToken();
	if (token.type != Token::END)
		ThrowError("Unexpected token at position " << token.start << ": '" << ctx.getTokenText(token) << "'");
	return result;
}