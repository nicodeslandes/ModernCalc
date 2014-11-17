#include "pch.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Calculator_Tests
{
	TEST_CLASS(ParserTests)
	{
	public:
		TEST_METHOD(TestSimpleAdd)
		{
			Parser parser(L"1+2");
			Assert::AreEqual(3, parser.parse_formula());
		}
		TEST_METHOD(TestSimpleMult)
		{
			Parser parser(L"2 * 3");
			Assert::AreEqual(6, parser.parse_formula());
		}
		TEST_METHOD(TestSimpleAddMultPrority)
		{
			Parser parser(L"1+2*3");
			Assert::AreEqual(7, parser.parse_formula());
		}
		TEST_METHOD(TestSimpleBrackets)
		{
			Parser parser(L"(1+2)*3");
			Assert::AreEqual(9, parser.parse_formula());
		}

		TEST_METHOD(TestMultipleCalls)
		{
			Parser parser(L"(1+2)*3");

			//Assert::AreEqual(9, parser.parse_formula());
			//Assert::AreEqual(9, parser.parse_formula());
		}
	};
}