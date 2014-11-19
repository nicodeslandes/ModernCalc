#include "pch.h"
#include "ExpressionEvaluator.h"
#include "Error.h"

using namespace std;
using namespace Parsing;

CALCULATOR_API int ExpressionEvaluator::Evaluate(Parser::ExprContextPtr expression) const
{
	return evaluateExpr(expression);
}

int ExpressionEvaluator::evaluateExpr(Parser::ExprContextPtr expression) const
{
	int result = 0;
	bool first = true;

	for (auto& operand: expression->getOperands())
	{
		auto operandValue = evaluateMultExpr(get<1>(operand));

		if (first)
		{
			result = operandValue;
			first = false;
			continue;
		}

		auto operation = get<0>(operand);
		result = processOperation(result, operandValue, operation);
	}

	return result;
}

int ExpressionEvaluator::evaluateMultExpr(Parser::MultExprContextPtr expression) const
{
	int result = 0;
	bool first = true;

	for (auto& operand : expression->getOperands())
	{
		auto operandValue = evaluateOperand(get<1>(operand));

		if (first)
		{
			result = operandValue;
			first = false;
			continue;
		}

		auto operation = get<0>(operand);
		result = processOperation(result, operandValue, operation);
	}

	return result;
}

int ExpressionEvaluator::evaluateOperand(Parser::OperandContextPtr operand) const
{
	auto& negateToken = operand->getNegateToken();
	int negateFactor = (negateToken && negateToken->getFirstChar() == '-') ? -1 : 1;

	if (operand->getType() == Parser::OperandContext::OperandType::Number)
		return negateFactor * evaluateNumber(operand->getNumber());

	return negateFactor * evaluateExpr(operand->getExpression());
}

int ExpressionEvaluator::evaluateNumber(Token numberToken) const
{
	auto numberText = numberToken.getText();
	wistringstream stream(numberText);
	int parsed;
	stream >> parsed;
	if (!stream.eof())
	{
		ThrowError(L"Wrong number: " << numberToken);
	}

	return parsed;
}

int ExpressionEvaluator::processOperation(int a, int b, Operation operation) const
{
	switch (operation)
	{
	case Operation::Add: return a + b;
	case Operation::Substract: return a - b;
	case Operation::Multiply: return a * b;
	case Operation::Divide: return a / b;
	default:
		ThrowError(L"Unhandled operator: " << (int)operation);
	}
}
