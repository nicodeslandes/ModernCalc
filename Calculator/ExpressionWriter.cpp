#include "pch.h"
#include "ExpressionWriter.h"
#include "Error.h"

using namespace std;
using namespace Parsing;

class ExpressionWriter::WritingContext
{
public:
	wostringstream Stream;
};

CALCULATOR_API std::wstring ExpressionWriter::Write(Parsing::Parser::ExprContextPtr expression) const
{
	WritingContext ctx;
	WriteExpr(ctx, expression);
	return ctx.Stream.str();
}

void ExpressionWriter::WriteExpr(WritingContext& ctx, Parsing::Parser::ExprContextPtr expression) const
{
	bool first = true;
	for (auto& operand : expression->getOperands())
	{
		auto& multExpression = get<1>(operand);
		if (first)
		{
			WriteMultExpr(ctx, multExpression);
			first = false;
			continue;
		}

		auto& operation = get<0>(operand);
		ctx.Stream << L' ';
		WriteOperation(ctx, operation);
		ctx.Stream << L' ';
		WriteMultExpr(ctx, multExpression);
	}
}

void ExpressionWriter::WriteMultExpr(WritingContext& ctx, Parsing::Parser::MultExprContextPtr expression) const
{
	bool first = true;
	for (auto& operand : expression->getOperands())
	{
		auto& operandValue = get<1>(operand);
		if (first)
		{
			WriteOperand(ctx, operandValue);
			first = false;
			continue;
		}

		auto& operation = get<0>(operand);
		ctx.Stream << L' ';
		WriteOperation(ctx, operation);
		ctx.Stream << L' ';
		WriteOperand(ctx, operandValue);
	}
}

void ExpressionWriter::WriteOperation(WritingContext& ctx, Parsing::Operation operation) const
{
	switch (operation)
	{
	case Operation::Add:
		ctx.Stream << L'+';
		break;
	case Operation::Substract:
		ctx.Stream << L'-';
		break;
	case Operation::Multiply:
		ctx.Stream << L'*';
		break;
	case Operation::Divide:
		ctx.Stream << L'/';
		break;
	default:
		ThrowError(L"Unhandled operation: " << (int) operation);
	}
}

void ExpressionWriter::WriteOperand(WritingContext& ctx, Parsing::Parser::OperandContextPtr operand) const
{
	auto& negateToken = operand->getNegateToken();
	if (negateToken && negateToken->getFirstChar() == '-')
		ctx.Stream << L"-";

	switch (operand->getType())
	{
	case Parser::OperandContext::OperandType::Number:
		WriteNumber(ctx, operand->getNumber());
		break;
	case Parser::OperandContext::OperandType::Expression:
		ctx.Stream << L'(';
		WriteExpr(ctx, operand->getExpression());
		ctx.Stream << L')';
		break;
	default:
		ThrowError(L"Unhandled operand type:" << (int)operand->getType());
	}
}

void ExpressionWriter::WriteNumber(WritingContext& ctx, Parsing::Token numberToken) const
{
	ctx.Stream << numberToken.getText();
}
