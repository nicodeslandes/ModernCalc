#pragma once
#include "Calculator.h"
#include "Tokenizer.h"
#include <string>
#include <memory>
#include "optional.hpp"
#include <vector>

namespace Parsing
{
	class ParsingContext;


	class Parser
	{
	public:
		class ExprContext;
		typedef std::shared_ptr<ExprContext> ExprContextPtr;

		CALCULATOR_API ExprContextPtr parse_formula(const std::wstring& str);

		class MultExprContext;
		class OperandContext;
		typedef std::shared_ptr<MultExprContext> MultExprContextPtr;
		typedef std::shared_ptr<OperandContext> OperandContextPtr;
	private:
		ExprContextPtr parse_expr(ParsingContext& ctx);
		OperandContextPtr parse_operand(ParsingContext & ctx);
		ExprContextPtr parse_multexpr(ParsingContext& ctx);
		wchar_t parse_operator(ParsingContext& ctx);

		Tokenizer _tokenizer;
	};

	enum class Operation
	{
		Add,
		Substract,
		Multiply,
		Divide
	};

	class Parser::ExprContext
	{
	public:
		const std::vector<std::tuple<Operation, OperandContextPtr>>& getOperands() const { return _operands; }

	private:
		void addOperand(Token operationToken, OperandContextPtr operand);
		Operation getOperation(Token operationToken);
		std::vector<std::tuple<Operation, OperandContextPtr>> _operands;

		friend class Parser;
	};

	class Parser::OperandContext
	{
	public:
		const std::optional<Token>& getNegateToken() const { return _negateToken; }

		enum class OperandType
		{
			Number,
			Expression
		};

		OperandType getType() const { return _type; }
		Token getNumber() const { return Number.get(); }
		std::shared_ptr<ExprContext> getExpression() const { return Expression; }

	private:
		friend class Parser;
		OperandType _type;
		std::optional<Token> _negateToken;
		std::optional<Token> Number;
		std::shared_ptr<ExprContext> Expression;
	};
}
