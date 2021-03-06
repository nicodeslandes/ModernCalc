#include "pch.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "ExpressionEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Parsing;

namespace Calculator_Tests
{
	TEST_CLASS(ParserTests)
	{
	public:
		TEST_METHOD(TestSimpleAdd)
		{
			TestFormula(L"1+2", 3);
		}
		TEST_METHOD(TestSimpleMult)
		{
			TestFormula(L"2 * 3", 6);
		}
		TEST_METHOD(TestSimpleAddMultPrority)
		{
			TestFormula(L"1+2*3", 7);
		}
		TEST_METHOD(TestSimpleBrackets)
		{
			TestFormula(L"(1+2)*3", 9);
		}
		TEST_METHOD(TestMultipleAdds)
		{
			TestFormula(L"1 +2  +3", 6);
		}
		TEST_METHOD(TestMultipleAdds2)
		{
			TestFormula(L"+1 ++2 + +3", 6);
		}
		TEST_METHOD(TestMultipleMultiplications)
		{
			TestFormula(L"1 *2  *3", 6);
		}
		TEST_METHOD(TestMultipleCalls)
		{
			Parser parser;

			auto expression = parser.parse_formula(L"(1+2)*3");
			ExpressionEvaluator evaluator;
			Assert::AreEqual(9, evaluator.Evaluate(expression));

			// Check we can call parse_formula another time
			expression = parser.parse_formula(L"(1+2)*3");
			Assert::AreEqual(9, evaluator.Evaluate(expression));

			// Check we can call Evaluate another time
			Assert::AreEqual(9, evaluator.Evaluate(expression));
		}
		TEST_METHOD(TestSimpleIdentifier)
		{
			TestFormula(L"1 + A*2", 5, {2});
		}
		TEST_METHOD(TestComplexIdentifier)
		{
			TestFormula(L"1 *2  *3 - A/2 + (3*C+ A)*D", 107, { 2, 4, 5, 6 });
		}
		TEST_METHOD(TestImplicitMultOperator)
		{
			TestFormula(L"A(2+B)", 6, { 2, 1 });
		}
		TEST_METHOD(TestImplicitMultOperatorInComplexExpression)
		{
			TestFormula(L"1+ 3A/2 + (3C+ A)D", 106, { 2, 4, 5, 6 });
		}
	private:
		void TestFormula(const wchar_t* formula, int value, std::vector<int> variableValues = {})
		{
			Parser parser;
			auto expression = parser.parse_formula(formula);
			ExpressionEvaluator evaluator;
			Assert::AreEqual(value, evaluator.Evaluate(expression, variableValues));
		}
	};
}