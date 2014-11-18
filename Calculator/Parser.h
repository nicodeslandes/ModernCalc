#pragma once
#include "Calculator.h"
#include "Tokenizer.h"
#include <string>
#include <memory>
#include "optional.hpp"

namespace Parsing
{
	class ParsingContext;


	class Parser
	{
	public:
		CALCULATOR_API int parse_formula(const std::wstring& str);

	private:
		int parse_expr(ParsingContext& ctx);
		int evaluateNumber(const Token& number) const;
		int processOperation(int a, int b, wchar_t operation) const;
		int parse_operand(ParsingContext & ctx);
		wchar_t parse_operator(ParsingContext& ctx);
		int parse_multexpr(ParsingContext& ctx);

		Tokenizer _tokenizer;
	};
}
