#include "pch.h"
#include "ExpressionWriter.h"
#include "Error.h"

using namespace std;
using namespace Parsing;

class ExpressionWriter::ExpressionWriterVisitor : public ExpressionVisitor<std::wstring>
{
public:
	virtual VisitorContextPtr CreateContext(VisitorContextPtr parent = nullptr);
	virtual std::wstring GetResult(VisitorContextPtr ctx);
	virtual void OnBeginVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Parsing::Operation operation);

	virtual void OnBeginVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, Parsing::Parser::ExprContextPtr expression);
	virtual void OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr expressionCtx, Parsing::Parser::ExprContextPtr expression);
	virtual void OnBeginVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken);
	virtual void OnVisitNumber(VisitorContextPtr numberCtx, Parsing::Token numberToken);

private:
	class WritingContext : public ExpressionVisitorBase::VisitorContext
	{
	public:
		wostringstream Stream;
	};

	void WriteOperation(WritingContext& ctx, Parsing::Operation operation) const;
};

CALCULATOR_API ExpressionWriter::ExpressionWriter() : _visitor(new ExpressionWriterVisitor)
{
}

CALCULATOR_API ExpressionWriter::~ExpressionWriter()
{
}

CALCULATOR_API std::wstring ExpressionWriter::Write(Parsing::Parser::ExprContextPtr expression)
{
	return _visitor->Visit(expression);
}

ExpressionVisitorBase::VisitorContextPtr ExpressionWriter::ExpressionWriterVisitor::CreateContext(VisitorContextPtr parent)
{
	return parent ? parent : make_shared<WritingContext>();
}

std::wstring ExpressionWriter::ExpressionWriterVisitor::GetResult(VisitorContextPtr expressionCtx)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*expressionCtx);
	return ctx.Stream.str();
}

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Parsing::Operation operation)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*expressionCtx);
	ctx.Stream << L' ';
	WriteOperation(ctx, operation);
	ctx.Stream << L' ';
}

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, Parsing::Parser::ExprContextPtr expression)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
	if (negateToken && negateToken->getFirstChar() == '-')
		ctx.Stream << L'-';

	if (expression->getOperands().size() > 1 && get<0>(expression->getOperands()[1]) != Operation::Multiply)
		ctx.Stream << L'(';
}

void ExpressionWriter::ExpressionWriterVisitor::OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr expressionCtx, Parsing::Parser::ExprContextPtr expression)
{
	if (expression->getOperands().size() > 1 && get<0>(expression->getOperands()[1]) != Operation::Multiply)
	{
		WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
		ctx.Stream << L')';
	}
}

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
	if (negateToken && negateToken->getFirstChar() == '-')
		ctx.Stream << L'-';
}

void ExpressionWriter::ExpressionWriterVisitor::OnVisitNumber(VisitorContextPtr numberCtx, Parsing::Token numberToken)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*numberCtx);
	ctx.Stream << numberToken.getText();
}

void ExpressionWriter::ExpressionWriterVisitor::WriteOperation(WritingContext& ctx, Parsing::Operation operation) const
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
