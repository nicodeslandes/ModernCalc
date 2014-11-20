#pragma once
#include "Parser.h"
#include "ExpressionVisitor.h"
#include <memory>

class ExpressionEvaluator
{
public:
	CALCULATOR_API ExpressionEvaluator();
	CALCULATOR_API ~ExpressionEvaluator();
	CALCULATOR_API int Evaluate(Parsing::Parser::ExprContextPtr expression);
private:
	class ExpressionEvaluatorVisitor;
	std::unique_ptr<ExpressionEvaluatorVisitor> _visitor;
};

