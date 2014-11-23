#include "pch.h"
#include "ExpressionEvaluator.h"
#include "Error.h"

using namespace std;
using namespace Parsing;

class ExpressionEvaluator::ExpressionEvaluatorVisitor : public ExpressionVisitor<int>
{
public:
	virtual VisitorContextPtr CreateContext(VisitorContextPtr parent = nullptr);
	virtual int GetResult(VisitorContextPtr ctx);

	virtual void OnVisitExpressionFirstOperand(VisitorContextPtr expressionCtx, VisitorContextPtr operandCtx);
	virtual void OnVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Operation operation, VisitorContextPtr operandCtx);
	virtual void OnVisitExpressionOperand(VisitorContextPtr operandCtx, optional<Token> negateToken, VisitorContextPtr expressionCtx, Parser::ExpressionPtr expression);
	virtual void OnVisitNumberOperand(VisitorContextPtr operandCtx, optional<Token> negateToken, VisitorContextPtr number);
	virtual void OnVisitIdentifierOperand(VisitorContextPtr operandCtx, optional<Token> negateToken, VisitorContextPtr identifier);

	virtual void OnVisitNumber(VisitorContextPtr numberCtx, const wstring& number);
	virtual void OnVisitIdentifier(VisitorContextPtr identifierCtx, const wstring& identifier);

	class EvaluationContext : public ExpressionVisitorBase::VisitorContext
	{
	public:
		EvaluationContext(const vector<int>& variables) : value{}, variables{ variables } {}
		EvaluationContext(const EvaluationContext& parent) : value{}, variables{ parent.variables } {}
		EvaluationContext& operator=(const EvaluationContext&) = delete;
		const vector<int>& variables;
		int value;
	};
	typedef std::shared_ptr<EvaluationContext> EvaluationContextPtr;
private:

	int ProcessOperation(int a, int b, Operation operation) const;
};


CALCULATOR_API ExpressionEvaluator::ExpressionEvaluator() : _visitor(new ExpressionEvaluatorVisitor)
{
}

CALCULATOR_API ExpressionEvaluator::~ExpressionEvaluator()
{
}

CALCULATOR_API int ExpressionEvaluator::Evaluate(Parser::ExpressionPtr expression, std::vector<int> variableValues /* = {}*/)
{
	auto ctx = make_shared<ExpressionEvaluatorVisitor::EvaluationContext>(variableValues);
	return _visitor->Visit(expression, ctx);
}


ExpressionVisitorBase::VisitorContextPtr ExpressionEvaluator::ExpressionEvaluatorVisitor::CreateContext(VisitorContextPtr parent)
{
	return make_shared<EvaluationContext>(*dynamic_pointer_cast<EvaluationContext>(parent));
}

int ExpressionEvaluator::ExpressionEvaluatorVisitor::GetResult(VisitorContextPtr expressionCtx)
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

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Operation operation, VisitorContextPtr operandCtx)
{
	auto& expressionEvalCtx = dynamic_cast<EvaluationContext&>(*expressionCtx);
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	expressionEvalCtx.value = ProcessOperation(expressionEvalCtx.value, operandEvalCtx.value, operation);
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitExpressionOperand(VisitorContextPtr operandCtx, optional<Token> negateToken, VisitorContextPtr expressionCtx, Parser::ExpressionPtr expression)
{
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	auto& expressionEvalCtx = dynamic_cast<EvaluationContext&>(*expressionCtx);
	auto negateFactor = (negateToken && negateToken->getFirstChar() == L'-') ? -1 : 1;
	operandEvalCtx.value = negateFactor * expressionEvalCtx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitNumberOperand(VisitorContextPtr operandCtx, optional<Token> negateToken, VisitorContextPtr number)
{
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	auto& numberEvalCtx = dynamic_cast<EvaluationContext&>(*number);
	auto negateFactor = (negateToken && negateToken->getFirstChar() == L'-') ? -1 : 1;
	operandEvalCtx.value = negateFactor * numberEvalCtx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitIdentifierOperand(VisitorContextPtr operandCtx, optional<Token> negateToken, VisitorContextPtr identifier)
{
	auto& operandEvalCtx = dynamic_cast<EvaluationContext&>(*operandCtx);
	auto& identifierEvalCtx = dynamic_cast<EvaluationContext&>(*identifier);
	auto negateFactor = (negateToken && negateToken->getFirstChar() == L'-') ? -1 : 1;
	operandEvalCtx.value = negateFactor * identifierEvalCtx.value;
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitIdentifier(VisitorContextPtr identifierCtx, const wstring& identifier)
{
	auto& identifierEvalCtx = dynamic_cast<EvaluationContext&>(*identifierCtx);
	if (identifierEvalCtx.variables.size() == 0)
		ThrowError(L"Invalid variable reference: '" << identifier << L"'; no variable is currently available");

	if (identifier.size() != 1)
		ThrowError(L"Wrong variable name: '" << identifier << L"'; variable should be a letter from A to Z");
	int variableIndex = identifier[0] - L'A';
	if (variableIndex < 0 || variableIndex >= (int)identifierEvalCtx.variables.size())
		ThrowError(L"Invalid variable: '" << identifier << L"'; please use variables between A and " << (wchar_t)(L'A' + identifierEvalCtx.variables.size() - 1));

	identifierEvalCtx.value = identifierEvalCtx.variables[variableIndex];
}

void ExpressionEvaluator::ExpressionEvaluatorVisitor::OnVisitNumber(VisitorContextPtr numberCtx, const wstring& number)
{
	auto& numberEvalCtx = dynamic_cast<EvaluationContext&>(*numberCtx);

	wistringstream stream(number);
	int parsed;
	stream >> parsed;
	if (!stream.eof())
	{
		ThrowError(L"Wrong number: " << number);
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
