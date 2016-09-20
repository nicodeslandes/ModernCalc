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
		class Expression;
		typedef std::shared_ptr<Expression> ExpressionPtr;

		CALCULATOR_API ExpressionPtr parse_formula(const std::wstring& str);

		class Operand;
		typedef std::shared_ptr<Operand> OperandPtr;
	private:
		ExpressionPtr parse_expr(ParsingContext& ctx);
		OperandPtr parse_operand(ParsingContext & ctx);
		ExpressionPtr parse_multexpr(ParsingContext& ctx);

		Tokenizer _tokenizer;
	};

	enum class Operation
	{
		Add,
		Substract,
		Multiply,
		Divide
	};

	class Parser::Expression
	{
	public:
		const std::vector<std::tuple<Operation, OperandPtr>>& getOperands() const { return _operands; }

	private:
		void addOperand(Token operationToken, OperandPtr operand);
		Operation getOperation(Token operationToken);
		std::vector<std::tuple<Operation, OperandPtr>> _operands;

		friend class Parser;
	};

	class Parser::Operand
	{
	public:
		const std::optional<Token>& getNegateToken() const { return _negateToken; }

		enum class OperandType
		{
			Number,
			Identifier,
			Expression
		};

		OperandType getType() const { return _type; }
		const std::wstring& getNumber() const { return _number.get(); }
		const std::wstring& getIdentifier() const { return _identifier.get(); }
		std::shared_ptr<Expression> getExpression() const { return _expression; }

	private:
		friend class Parser;
		OperandType _type;
		std::optional<Token> _negateToken;
		std::optional<std::wstring> _number;
		std::optional<std::wstring> _identifier;
		std::shared_ptr<Expression> _expression;
	};
}
