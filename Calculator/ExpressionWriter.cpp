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
	virtual void OnBeginVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Operation operation);

	virtual void OnBeginVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken, Parser::ExpressionPtr expression);
	virtual void OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken, VisitorContextPtr expressionCtx, Parser::ExpressionPtr expression);
	virtual void OnBeginVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken);
	virtual void OnVisitNumber(VisitorContextPtr numberCtx, const std::wstring& number);
	virtual void OnBeginVisitIdentifierOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken);
	virtual void OnVisitIdentifier(VisitorContextPtr identifierCtx, const wstring& identifier);

private:
	class WritingContext : public ExpressionVisitorBase::VisitorContext
	{
	public:
		wostringstream Stream;
	};

	void WriteOperation(WritingContext& ctx, Operation operation) const;
	bool IsMultiplicationOperation(Operation operation) const;
};

CALCULATOR_API ExpressionWriter::ExpressionWriter() : _visitor(new ExpressionWriterVisitor)
{
}

CALCULATOR_API ExpressionWriter::~ExpressionWriter()
{
}

CALCULATOR_API std::wstring ExpressionWriter::Write(Parser::ExpressionPtr expression)
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

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Operation operation)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*expressionCtx);
	ctx.Stream << L' ';
	WriteOperation(ctx, operation);
	ctx.Stream << L' ';
}

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken, Parser::ExpressionPtr expression)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
	if (negateToken && negateToken->getFirstChar() == '-')
		ctx.Stream << L'-';

	if (expression->getOperands().size() > 1 && !IsMultiplicationOperation(get<0>(expression->getOperands()[1])))
		ctx.Stream << L'(';
}

void ExpressionWriter::ExpressionWriterVisitor::OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken, VisitorContextPtr expressionCtx, Parser::ExpressionPtr expression)
{
	if (expression->getOperands().size() > 1 && !IsMultiplicationOperation(get<0>(expression->getOperands()[1])))
	{
		WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
		ctx.Stream << L')';
	}
}

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
	if (negateToken && negateToken->getFirstChar() == '-')
		ctx.Stream << L'-';
}

void ExpressionWriter::ExpressionWriterVisitor::OnVisitNumber(VisitorContextPtr numberCtx, const std::wstring& number)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*numberCtx);
	ctx.Stream << number;
}

void ExpressionWriter::ExpressionWriterVisitor::OnBeginVisitIdentifierOperand(VisitorContextPtr operandCtx, std::optional<Token> negateToken)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*operandCtx);
	if (negateToken && negateToken->getFirstChar() == '-')
		ctx.Stream << L'-';
}

void ExpressionWriter::ExpressionWriterVisitor::OnVisitIdentifier(VisitorContextPtr identifierCtx, const wstring& identifier)
{
	WritingContext& ctx = dynamic_cast<WritingContext&>(*identifierCtx);
	ctx.Stream << identifier;
}

void ExpressionWriter::ExpressionWriterVisitor::WriteOperation(WritingContext& ctx, Operation operation) const
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

bool ExpressionWriter::ExpressionWriterVisitor::IsMultiplicationOperation(Operation operation) const
{
	return operation == Operation::Multiply || operation == Operation::Divide;
}
