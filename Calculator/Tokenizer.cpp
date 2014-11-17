#include "pch.h"
#include "Tokenizer.h"
#include "Error.h"
#include "ParsingContext.h"

using namespace std;

Token Tokenizer::getNextToken(ParsingContext& ctx) const
{
	// If a token has been saved (by a previous call to peekNextToken),
	// return it now without moving forward
	if (ctx._savedToken.type != Token::END)
	{
		Token token = ctx._savedToken;

		// Reset the saved token
		ctx._savedToken.type = Token::END;
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
		return Token(&ctx, Token::END, position);

	// Read next character
	auto start = position;
	auto c = str[position++];

	// Check for Number
	if (iswdigit(c)) {
		for (; position < (int) str.length() && iswdigit(str[position]); position++);
		return Token(&ctx, Token::NUMBER, start, position);
	}

	// Check for other, 1-character tokens
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
		ThrowError(L"Unrecognized character at position " << position << ": '" << c << "'");
	}

	return Token(&ctx, tokenType, start, position);
}
