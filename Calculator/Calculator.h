#pragma once

#ifdef CALCULATOR_EXPORTS
#define CALCULATOR_API __declspec(dllexport)
#else
#define CALCULATOR_API __declspec(dllimport)
#endif

#include <string>

CALCULATOR_API int calculate(const std::wstring& formula);


class ParsingError
{
public:
	ParsingError(const std::wstring&& message) : _message(message) { }
	const std::wstring& getMessage() const { return _message; }

private:
	std::wstring _message;
};