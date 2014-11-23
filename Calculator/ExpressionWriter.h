#pragma once

#include "Calculator.h"
#include "Parser.h"
#include "ExpressionVisitor.h"

class ExpressionWriter
{
public:
	CALCULATOR_API ExpressionWriter();
	CALCULATOR_API ~ExpressionWriter();
	CALCULATOR_API std::wstring Write(Parsing::Parser::ExpressionPtr expression);
private:
	class ExpressionWriterVisitor;
	std::unique_ptr<ExpressionWriterVisitor> _visitor;
};

