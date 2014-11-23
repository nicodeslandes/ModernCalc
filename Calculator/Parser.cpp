#include "pch.h"
#include "Parser.h"
#include "Token.h"
#include "Error.h"
#include "ParsingContext.h"
#include "Tokenizer.h"

using namespace std;
using namespace Parsing;

// Grammar:
// formula:	: expr<END>
// expr		: mult_expr ( ('+' | '-' ) mult_expr)*
// mult_expr: operand ( ('*' | '/' ) operand )*
// operand	: ('-' | '+')? ( Number | '(' expr ')' | Identifier )
// Number : [0-9]+
// Identifier: [a-zA-Z]+

CALCULATOR_API Parser::ExpressionPtr Parser::parse_formula(const wstring& str)
{
	ParsingContext ctx(str);
	auto result = parse_expr(ctx);
	auto token = _tokenizer.peekNextToken(ctx);
	if (token.getType() != TokenType::END)
		ThrowError(L"Unexpected token: " << token);

	return result;
}

Parser::ExpressionPtr Parser::parse_expr(ParsingContext& ctx)
{
	auto expression = make_shared<Expression>();

	// Initialize the operation token to a dummy token
	Token operationToken = Token::None;

	do
	{
		if (operationToken.getType() != TokenType::END)
		{
			// If we've just peeked a new operator, consume it now
			_tokenizer.getNextToken(ctx);
		}

		auto operandExpression = parse_multexpr(ctx);
		auto operand = make_shared<Operand>();
		operand->_type = Operand::OperandType::Expression;
		operand->_expression = operandExpression;
		expression->addOperand(operationToken, operand);

		operationToken = _tokenizer.peekNextToken(ctx);

	} while (operationToken.getType() == TokenType::ADD_OPERATOR);

	return expression;
}

Parser::ExpressionPtr Parser::parse_multexpr(ParsingContext& ctx)
{
	auto multExpression = make_shared<Expression>();

	// Initialize the operation token to a dummy token
	Token operationToken = Token::None;

	do
	{
		if (operationToken.getType() != TokenType::END)
		{
			// If we've just peeked a new operator, consume it now
			_tokenizer.getNextToken(ctx);
		}

		auto operand = parse_operand(ctx);
		multExpression->addOperand(operationToken, operand);

		operationToken = _tokenizer.peekNextToken(ctx);
	} while (operationToken.getType() == TokenType::MULT_OPERATOR);

	return multExpression;
}

Parser::OperandPtr Parser::parse_operand(ParsingContext& ctx)
{
	auto operandContext = make_shared<Operand>();

	auto token = _tokenizer.getNextToken(ctx);
	if (token.getType() == TokenType::ADD_OPERATOR)
	{
		operandContext->_negateToken = token;
		token = _tokenizer.getNextToken(ctx);
	}

	if (token.getType() == TokenType::NUMBER)
	{
		operandContext->_type = Operand::OperandType::Number;
		operandContext->_number = token.getText();
		return operandContext;
	}

	if (token.getType() == TokenType::IDENTIFIER)
	{
		operandContext->_type = Operand::OperandType::Identifier;
		auto& tokenText = token.getText();
		wstring identifier;
		std::transform(begin(tokenText), end(tokenText), std::back_inserter(identifier), towupper);
		operandContext->_identifier = identifier;
		return operandContext;
	}

	if (token.getType() == TokenType::OPEN_PARENT)
	{
		auto expression = parse_expr(ctx);
		token = _tokenizer.getNextToken(ctx);
		if (token.getType() == TokenType::CLOSE_PARENT)
		{
			operandContext->_type = Operand::OperandType::Expression;
			operandContext->_expression = expression;
			return operandContext;
		}


		ThrowError(L"Missing closing parenthesis at position " << token.getPosition());
	}

	ThrowError(L"Unexpected token at position " << token.getPosition() << ": '" << token.getText() << "'; number expected");
}

wchar_t Parser::parse_operator(ParsingContext& ctx)
{
	auto operatorToken = _tokenizer.getNextToken(ctx);
	if (operatorToken.getType() != TokenType::ADD_OPERATOR && operatorToken.getType() != TokenType::MULT_OPERATOR)
	{
		if (operatorToken.getType() == TokenType::END)
			return '\0';

		ThrowError(L"Unexpected token at position " << operatorToken.getPosition() << ": '" << operatorToken.getText() << "'; number expected");
	}

	return operatorToken.getFirstChar();
}


Operation Parser::Expression::getOperation(Token operationToken)
{
	if (operationToken.getType() == TokenType::END)
		return Operation::Add;

	switch (operationToken.getFirstChar())
	{
	case '+': return Operation::Add;
	case '-': return Operation::Substract;
	case '*': return Operation::Multiply;
	case '/': return Operation::Divide;
	default: ThrowError("Unexpected addition operator: " << operationToken);
	}
}

void Parsing::Parser::Expression::addOperand(Token operationToken, OperandPtr operand)
{
	auto operation = getOperation(operationToken);
	_operands.push_back(make_tuple(operation, operand));
}
