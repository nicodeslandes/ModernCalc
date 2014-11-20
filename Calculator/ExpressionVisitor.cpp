#include "pch.h"
#include "ExpressionVisitor.h"
#include "Error.h"

using namespace std;
using namespace Parsing;

ExpressionVisitorBase::VisitorContextPtr ExpressionVisitorBase::VisitExpression(VisitorContextPtr ctx, Parsing::Parser::ExprContextPtr expression)
{
	bool first = true;

	auto exprCtx = CreateContext(ctx);
	for (auto& operand : expression->getOperands())
	{
		if (first)
		{
			auto operandValue = VisitOperand(exprCtx, get<1>(operand));
			OnVisitExpressionFirstOperand(exprCtx, operandValue);
				
			first = false;
			continue;
		}

		auto operation = get<0>(operand);
		OnBeginVisitExpressionSubsequentOperand(exprCtx, operation);

		auto operandValue = VisitOperand(exprCtx, get<1>(operand));
		OnVisitExpressionSubsequentOperand(exprCtx, operation, operandValue);
	}

	return exprCtx;
}

ExpressionVisitorBase::VisitorContextPtr ExpressionVisitorBase::VisitOperand(VisitorContextPtr ctx, Parsing::Parser::OperandContextPtr operand)
{
	auto operandCtx = CreateContext(ctx);
	auto& negateToken = operand->getNegateToken();

	switch (operand->getType())
	{
	case Parser::OperandContext::OperandType::Number:
	{
		OnBeginVisitNumberOperand(operandCtx, negateToken);
		auto numberCtx = VisitNumber(operandCtx, operand->getNumber());
		OnVisitNumberOperand(operandCtx, negateToken, numberCtx);
		break;
	}
	case Parser::OperandContext::OperandType::Identifier:
	{
		OnBeginVisitIdentifierOperand(operandCtx, negateToken);
		auto numberCtx = VisitIdentifier(operandCtx, operand->getIdentifier());
		OnVisitNumberOperand(operandCtx, negateToken, numberCtx);
		break;
	}
	case Parser::OperandContext::OperandType::Expression:
	{
		OnBeginVisitExpressionOperand(operandCtx, negateToken, operand->getExpression());
		auto exprCtx = VisitExpression(operandCtx, operand->getExpression());
		OnVisitExpressionOperand(operandCtx, negateToken, exprCtx, operand->getExpression());
		break;
	}
	default:
		ThrowError(L"Unhandled operand type:" << (int) operand->getType());
	}

	return operandCtx;
}

ExpressionVisitorBase::VisitorContextPtr ExpressionVisitorBase::VisitNumber(VisitorContextPtr ctx, const wstring& number)
{
	auto numberCtx = CreateContext(ctx);
	OnVisitNumber(numberCtx, number);
	return numberCtx;
}

ExpressionVisitorBase::VisitorContextPtr ExpressionVisitorBase::VisitIdentifier(VisitorContextPtr ctx, const wstring& identifier)
{
	auto numberCtx = CreateContext(ctx);
	OnVisitIdentifier(numberCtx, identifier);
	return numberCtx;
}
