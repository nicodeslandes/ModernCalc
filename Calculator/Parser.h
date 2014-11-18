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

		class ExprContext;
		class OperandContext
		{
		public:
			std::optional<Token> NegateToken;

			enum class OperandType
			{
				Number,
				Expression
			};

			std::optional<Token> Number;
			std::shared_ptr<ExprContext> Expression;

			//Token Indentifier;

		};
	protected:
		virtual int processOperand(const OperandContext& context) { return 0; }
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
