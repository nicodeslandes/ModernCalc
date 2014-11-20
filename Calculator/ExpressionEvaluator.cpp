#include "pch.h"
#include "ExpressionEvaluator.h"
#include "Error.h"

using namespace std;
using namespace Parsing;

class ExpressionEvaluator::ExpressionEvaluatorVisitor : public ExpressionVisitor<int>
{
public:
	virtual VisitorContextPtr CreateContext(VisitorContextPtr parent = nullptr);
	virtual int OnSuccessfulVisit(VisitorContextPtr ctx);

	virtual void OnVisitExpressionFirstOperand(VisitorContextPtr expressionCtx, VisitorContextPtr operandCtx);
	virtual void OnVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Parsing::Operation operation, VisitorContextPtr operandCtx);
	virtual void OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr expressionCtx, Parsing::Parser::ExprContextPtr expression);
	virtual void OnVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr number);

	virtual void OnVisitNumber(VisitorContextPtr numberCtx, Parsing::Token numberToken);

private:
	class EvaluationContext : public ExpressionVisitorBase::VisitorContext
	{
	public:
		int value;
	};

	int ProcessOperation(int a, int b, Operation operation) const;
};


CALCULATOR_API ExpressionEvaluator::ExpressionEvaluator() : _visitor(new ExpressionEvaluatorVisitor)
{
}

CALCULATOR_API ExpressionEvaluator::~ExpressionEvaluator()
{
}

CALCULATOR_API int ExpressionEvaluator::Evaluate(Parser::ExprContextPtr expression)
{
	return _visitor->Visit(expression);
}


ExpressionVisitorBase::VisitorContextPtr ExpressionEvaluator::ExpressionEvaluatorVisitor::CreateContext(VisitorContextPtr parent)
{
	return make_shared<EvaluationContext>();
}

int ExpressionEvaluator::ExpressionEvaluatorVisitor::OnSuccessfulVisit(VisitorContextPtr expressionCtx)
{
	EvaluationContext& ctx = dynamic_cast<EvaluationContext&>(*expressionCtx);
	return ctx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitExpressionFirstOperand(VisitorContextPtr expressionCtx, VisitorContextPtr operandCtx)
{
	auto& expressionEvalCtx = dynamic_cast<EvaluationContext&>(*expressionCtx);
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	expressionEvalCtx.value = operandEvalCtx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Parsing::Operation operation, VisitorContextPtr operandCtx)
{
	auto& expressionEvalCtx = dynamic_cast<EvaluationContext&>(*expressionCtx);
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	expressionEvalCtx.value = ProcessOperation(expressionEvalCtx.value, operandEvalCtx.value, operation);
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr expressionCtx, Parsing::Parser::ExprContextPtr expression)
{
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	auto& expressionEvalCtx = dynamic_cast<EvaluationContext&>(*expressionCtx);
	auto negateFactor = (negateToken && negateToken->getFirstChar() == L'-') ? -1 : 1;
	operandEvalCtx.value = negateFactor * expressionEvalCtx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr number)
{
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	auto& numberEvalCtx = dynamic_cast<EvaluationContext&>(*number);
	auto negateFactor = (negateToken && negateToken->getFirstChar() == L'-') ? -1 : 1;
	operandEvalCtx.value = negateFactor * numberEvalCtx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitNumber(VisitorContextPtr numberCtx, Parsing::Token numberToken)
{
	auto& numberEvalCtx = dynamic_cast<EvaluationContext&>(*numberCtx);

	auto numberText = numberToken.getText();
	wistringstream stream(numberText);
	int parsed;
	stream >> parsed;
	if (!stream.eof())
	{
		ThrowError(L"Wrong number: " << numberToken);
	}

	numberEvalCtx.value = parsed;
}

int ExpressionEvaluator::ExpressionEvaluatorVisitor::ProcessOperation(int a, int b, Operation operation) const
{
	switch (operation)
	{
	case Operation::Add: return a + b;
	case Operation::Substract: return a - b;
	case Operation::Multiply: return a * b;
	case Operation::Divide: return a / b;
	default:
		ThrowError(L"Unhandled operator: " << (int) operation);
	}
}
