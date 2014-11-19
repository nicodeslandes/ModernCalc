#pragma once
#include "Parser.h"

class ExpressionWriter
{
public:
	CALCULATOR_API std::wstring Write(Parsing::Parser::ExprContextPtr expression) const;
private:
	class WritingContext;
	void WriteExpr(WritingContext& ctx, Parsing::Parser::ExprContextPtr expression) const;
	void WriteMultExpr(WritingContext& ctx, Parsing::Parser::MultExprContextPtr expression) const;
	void WriteOperand(WritingContext& ctx, Parsing::Parser::OperandContextPtr operand) const;
	void WriteNumber(WritingContext& ctx, Parsing::Token numberToken) const;
	void WriteOperation(WritingContext & ctx, Parsing::Operation operation) const;
};

