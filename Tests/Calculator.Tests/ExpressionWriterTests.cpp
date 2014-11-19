#include "pch.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "ExpressionWriter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Parsing;
using namespace std;

namespace Calculator_Tests
{
	TEST_CLASS(ExpressionWriterTests)
	{
	public:
		TEST_METHOD(TestSimpleAdd)
		{
			TestFormula(L"1+2", L"1 + 2");
		}
		TEST_METHOD(TestSimpleMult)
		{
			TestFormula(L"2 * 3", L"2 * 3");
		}
		TEST_METHOD(TestSimpleAddMultPrority)
		{
			TestFormula(L"1-2*3", L"1 - 2 * 3");
		}
		TEST_METHOD(TestSimpleBrackets)
		{
			TestFormula(L"(1+2)* 3", L"(1 + 2) * 3");
		}
		TEST_METHOD(TestMultipleAdds)
		{
			TestFormula(L"1 +2  +3", L"1 + 2 + 3");
		}
		TEST_METHOD(TestMultipleAdds2)
		{
			TestFormula(L"+1 ++2 + +3", L"1 + 2 + 3");
		}
		TEST_METHOD(TestMultipleMultiplications)
		{
			TestFormula(L"1 *2  *3",  L"1 * 2 * 3");
		}
		TEST_METHOD(TestMultipleCalls)
		{
			Parser parser;

			auto expression = parser.parse_formula(L"(1+2)*3");
			ExpressionWriter writer;
			Assert::AreEqual(wstring(L"(1 + 2) * 3"), writer.Write(expression));

			// Check we can call parse_formula another time
			expression = parser.parse_formula(L"(1+2)*3");
			Assert::AreEqual(wstring(L"(1 + 2) * 3"), writer.Write(expression));

			// Check we can call Evaluate another time
			Assert::AreEqual(wstring(L"(1 + 2) * 3"), writer.Write(expression));
		}
	private:
		void TestFormula(const wchar_t* formula, const wchar_t* formattedFormula)
		{
			Parser parser;
			auto expression = parser.parse_formula(formula);
			ExpressionWriter writer;
			Assert::AreEqual(wstring(formattedFormula), writer.Write(expression));
		}
	};
}