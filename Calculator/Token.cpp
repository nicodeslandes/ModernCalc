#include "pch.h"
#include "Token.h"
#include "ParsingContext.h"

using namespace std;

wstring Token::getText() const
{
	auto startPtr = &_ctx->getString()[start];
	return wstring(startPtr, startPtr + end - start);
}

wchar_t Token::getFirstChar() const
{
	return _ctx->getString()[start];
}

