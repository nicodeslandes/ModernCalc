#include "pch.h"
#include "Parser.h"
#include "Token.h"
#include "Error.h"
#include "ParsingContext.h"
#include "Tokenizer.h"

using namespace std;
using namespace Parsing;

int Parser::evaluateNumber(const Token& number) const
{
	auto numberText = number.getText();
	wistringstream stream(numberText);
	int parsed;
	stream >> parsed;
	if (!stream.eof())
	{
		ThrowError(L"Wrong number: " << numberText);
	}

	return parsed;
}

int Parser::processOperation(int a, int b, wchar_t operation) const
{
	switch (operation)
	{
	case L'+': return a + b;
	case L'-': return a - b;
	case L'*': return a * b;
	case L'/': return a / b;
	case L'^': return (int) pow(a, b);
	default:
		ThrowError(L"Unhandled operator: " << operation);
	}
}

// Grammar:
// formula:	: expr<EOF>
// expr		: mult_expr ( ('+' | '-' ) mult_expr)*
// mult_expr: operand ( ('*' | '/' ) operand )*
// operand	: ('-' | '+')? ( Number | '(' expr ')' | Identifier )
// Number : [0-9]+
// Identifier: [a-zA-Z]+

int Parser::parse_operand(ParsingContext& ctx)
{
	int negateFactor = 1;

	auto token = _tokenizer.getNextToken(ctx);
	if (token.type == TokenType::ADD_OPERATOR)
	{
		if (token.getFirstChar() == '-') negateFactor = -1;
		token = _tokenizer.getNextToken(ctx);
	}

	if (token.type == TokenType::NUMBER)
		return negateFactor * evaluateNumber(token);

	if (token.type == TokenType::OPEN_PARENT)
	{
		auto value = parse_expr(ctx);
		token = _tokenizer.getNextToken(ctx);
		if (token.type == TokenType::CLOSE_PARENT)
			return negateFactor * value;

		ThrowError(L"Missing closing parenthesis at position " << token.start);
	}

	ThrowError(L"Unexpected token at position " << token.start << ": '" << token.getText() << "'; number expected");
}

wchar_t Parser::parse_operator(ParsingContext& ctx)
{
	auto operatorToken = _tokenizer.getNextToken(ctx);
	if (operatorToken.type != TokenType::ADD_OPERATOR && operatorToken.type != TokenType::MULT_OPERATOR)
	{
		if (operatorToken.type == TokenType::END)
			return '\0';

		ThrowError(L"Unexpected token at position " << operatorToken.start << ": '" << operatorToken.getText() << "'; number expected");
	}

	return operatorToken.getFirstChar();
}

int Parser::parse_multexpr(ParsingContext& ctx)
{
	auto value = parse_operand(ctx);

	while (true)
	{
		auto nextToken = _tokenizer.peekNextToken(ctx);
		if (nextToken.type != TokenType::MULT_OPERATOR)
			break;

		auto op = parse_operator(ctx);
		auto operandValue = parse_operand(ctx);
		value = processOperation(value, operandValue, op);
	}

	return value;
}

int Parser::parse_expr(ParsingContext& ctx)
{
	auto value = parse_multexpr(ctx);

	while (true)
	{
		auto nextToken = _tokenizer.peekNextToken(ctx);
		if (nextToken.type != TokenType::ADD_OPERATOR)
			break;

		auto op = parse_operator(ctx);
		auto operandValue = parse_multexpr(ctx);
		value = processOperation(value, operandValue, op);
	}

	return value;
}

CALCULATOR_API int Parser::parse_formula(const wstring& str)
{
	ParsingContext ctx(str);
	auto result = parse_expr(ctx);
	auto token = _tokenizer.peekNextToken(ctx);
	if (token.type != TokenType::END)
		ThrowError(L"Unexpected token at position " << token.start << ": '" << token.getText() << "'");

	return result;
}

