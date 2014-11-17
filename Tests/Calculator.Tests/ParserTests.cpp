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
			Parser parser;
			Assert::AreEqual(3, parser.parse_formula(L"1+2"));
		}
		TEST_METHOD(TestSimpleMult)
		{
			Parser parser;
			Assert::AreEqual(6, parser.parse_formula(L"2 * 3"));
		}
		TEST_METHOD(TestSimpleAddMultPrority)
		{
			Parser parser;
			Assert::AreEqual(7, parser.parse_formula(L"1+2*3"));
		}
		TEST_METHOD(TestSimpleBrackets)
		{
			Parser parser;
			Assert::AreEqual(9, parser.parse_formula(L"(1+2)*3"));
		}

		TEST_METHOD(TestMultipleCalls)
		{
			Parser parser;

			Assert::AreEqual(9, parser.parse_formula(L"(1+2)*3"));
			Assert::AreEqual(9, parser.parse_formula(L"(1+2)*3"));
		}
	};
}