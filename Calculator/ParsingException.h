#pragma once
#include <string>

class ParsingException
{
public:
	ParsingException(const std::wstring&& message) : _message(message) { }
	const std::wstring& getMessage() const { return _message; }

private:
	std::wstring _message;
};