// Calculator.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "Calculator.h"
#include "Parser.h"

using namespace std;

int calculate(const std::wstring& formula)
{
	Parser parser;
	return parser.parse_formula(formula);
}