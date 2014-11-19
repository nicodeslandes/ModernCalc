#pragma once
#include "Parser.h"

class ExpressionEvaluator
{
public:
	CALCULATOR_API int Evaluate(Parsing::Parser::ExprContextPtr expression) const;
private:
	int evaluateExpr(Parsing::Parser::ExprContextPtr expression) const;
	int evaluateMultExpr(Parsing::Parser::MultExprContextPtr expression) const;
	int evaluateOperand(Parsing::Parser::OperandContextPtr operand) const;
	int evaluateNumber(Parsing::Token numberToken) const;

	int processOperation(int a, int b, Parsing::Operation operation) const;
};

