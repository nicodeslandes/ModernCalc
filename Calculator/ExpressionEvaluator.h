#pragma once
#include "Parser.h"
#include "ExpressionVisitor.h"
#include <memory>
#include "Parser.h"

class ExpressionEvaluator
{
public:
	CALCULATOR_API ExpressionEvaluator();
	CALCULATOR_API ~ExpressionEvaluator();
	CALCULATOR_API int Evaluate(Parsing::Parser::ExpressionPtr expression, std::vector<int> variableValues = {});
private:
	class ExpressionEvaluatorVisitor;
	std::unique_ptr<ExpressionEvaluatorVisitor> _visitor;
};

