#pragma once

#include <memory>
#include "Parser.h"


class ExpressionVisitorBase
{
public:
	void Visit(Parsing::Parser::ExprContextPtr expression);
protected:
	class VisitorContext
	{
	public:
		virtual ~VisitorContext() {}
	};
	typedef std::shared_ptr<VisitorContext> VisitorContextPtr;

	virtual VisitorContextPtr CreateContext(VisitorContextPtr parent = nullptr) = 0;

	VisitorContextPtr VisitExpression(VisitorContextPtr ctx, Parsing::Parser::ExprContextPtr expression);
	VisitorContextPtr VisitOperand(VisitorContextPtr ctx, Parsing::Parser::OperandContextPtr operand);
	VisitorContextPtr VisitNumber(VisitorContextPtr ctx, const std::wstring& number);
	VisitorContextPtr VisitIdentifier(VisitorContextPtr ctx, const std::wstring& identifier);

#pragma warning (push)
#pragma warning (disable: 4100)
	virtual void OnVisitExpressionFirstOperand(VisitorContextPtr expressionCtx, VisitorContextPtr operandCtx) {}
	virtual void OnBeginVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Parsing::Operation operation) {}
	virtual void OnVisitExpressionSubsequentOperand(VisitorContextPtr expressionCtx, Parsing::Operation operation, VisitorContextPtr operandCtx) {}

	virtual void OnBeginVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, Parsing::Parser::ExprContextPtr expression) {}
	virtual void OnVisitExpressionOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr expressionCtx, Parsing::Parser::ExprContextPtr expression) {}
	virtual void OnBeginVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken) {}
	virtual void OnBeginVisitIdentifierOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken) {}
	virtual void OnVisitNumberOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr number) {}
	virtual void OnVisitIdentifierOperand(VisitorContextPtr operandCtx, std::optional<Parsing::Token> negateToken, VisitorContextPtr identifier) {}

	virtual void OnVisitNumber(VisitorContextPtr numberCtx, const std::wstring& number) {}
	virtual void OnVisitIdentifier(VisitorContextPtr numberCtx, const std::wstring& identifier) {}
#pragma warning (pop)
};

template <class T>
class ExpressionVisitor : public ExpressionVisitorBase
{
public:
	T Visit(Parsing::Parser::ExprContextPtr expression, VisitorContextPtr ctx = nullptr);

protected:
	virtual T GetResult(VisitorContextPtr ctx) = 0;
};

template<class T>
inline T ExpressionVisitor<T>::Visit(Parsing::Parser::ExprContextPtr expression, VisitorContextPtr ctx /*= nullptr*/)
{
	ctx = VisitExpression(ctx, expression);
	return GetResult(ctx);
}
