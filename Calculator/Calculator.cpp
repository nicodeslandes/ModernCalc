// Calculator.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "Calculator.h"
#include "Parser.h"
#include "ExpressionEvaluator.h"

using namespace std;
using namespace Parsing;

int calculate(const std::wstring& formula)
{
	Parser parser;
	ExpressionEvaluator ee;
	auto expression = parser.parse_formula(formula);
	return ee.Evaluate(expression);
}