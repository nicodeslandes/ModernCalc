#include "pch.h"
#include "Tokenizer.h"
#include "Error.h"
#include "ParsingContext.h"

using namespace std;
using namespace Parsing;

Token Tokenizer::getNextToken(ParsingContext& ctx) const
{
	// If a token has been saved (by a previous call to peekNextToken),
	// return it now without moving forward
	if (ctx._savedToken.getType() != TokenType::END)
	{
		Token token = ctx._savedToken;

		// Reset the saved token
		ctx._savedToken = Token::None;
		return token;
	}

	return readNextToken(ctx);
}

Token Tokenizer::peekNextToken(ParsingContext& ctx) const
{
	// Get the next token, and save it so that the next
	// call to getNextToken can return it without moving forward
	auto token = getNextToken(ctx);
	ctx._savedToken = token;
	return token;
}

Token Tokenizer::readNextToken(ParsingContext& ctx) const
{
	auto& position = ctx._position;
	auto& str = ctx._str;


	// Skip whitespaces
	while (position < (int) str.length() && iswblank(str[position]))
		position++;

	// Check for the end of the stream
	if (position >= (int) str.length())
		return Token(&ctx, TokenType::END, position);

	// Read next character
	auto start = position;
	auto c = str[position++];

	// Check for Number
	if (iswdigit(c)) {
		for (; position < (int) str.length() && iswdigit(str[position]); position++);
		return Token(&ctx, TokenType::NUMBER, start, position);
	}

	// Check for other, 1-character tokens
	auto tokenType = TokenType::END;
	switch (c)
	{
	case '+':
	case '-':
		tokenType = TokenType::ADD_OPERATOR;
		break;

	case '*':
	case '/':
		tokenType = TokenType::MULT_OPERATOR;
		break;

	case '(':
		tokenType = TokenType::OPEN_PARENT;
		break;

	case ')':
		tokenType = TokenType::CLOSE_PARENT;
		break;

	default:
		ThrowError(L"Unrecognized character at position " << position << ": '" << c << "'");
	}

	return Token(&ctx, tokenType, start, position);
}
