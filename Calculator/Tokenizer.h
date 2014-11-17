#pragma once
#include <string>
#include "Token.h"

class Tokenizer
{
public:
	Tokenizer(const std::wstring& str);
	Token getNextToken();
	Token peekNextToken();
	std::wstring getTokenText(const Token& token) const;
	wchar_t getTokenStartChar(const Token& token) const;

private:
	Token readNextToken();

	int position;
	const std::wstring& str;
	Token savedToken;
};
