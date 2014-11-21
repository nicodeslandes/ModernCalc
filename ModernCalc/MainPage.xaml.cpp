//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Calculator.h"
#include "ParsingException.h"
#include "ExpressionEvaluator.h"
#include "ExpressionWriter.h"

using namespace ModernCalc;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace std;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

void ModernCalc::MainPage::EvaluateFormula()
{
	std::wstring formula(formulaTextBox->Text->Data());
	try
	{
		Parsing::Parser parser;
		ExpressionEvaluator evaluator;
		ExpressionWriter writer;

		auto expression = parser.parse_formula(formula);
		formulaTextBox->Text = ref new String(writer.Write(expression).c_str());
		vector<TextBox^> textBoxes{TextBox_A, TextBox_B, TextBox_C , TextBox_D, TextBox_E , TextBox_F};
		vector<int> values;
		transform(begin(textBoxes), end(textBoxes), back_inserter(values), [](TextBox^ tb) { return _wtoi(tb->Text->Data()); });
		int res = evaluator.Evaluate(expression, values);
		resultTextBox->Text = res.ToString();
	}
	catch (const ParsingException& ex)
	{
		resultTextBox->Text = ref new Platform::String(ex.getMessage().c_str());
	}
}

void ModernCalc::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	EvaluateFormula();
}


void ModernCalc::MainPage::formulaTextBox_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	if (e->Key == Windows::System::VirtualKey::Enter)
	{
		EvaluateFormula();
	}
}


void ModernCalc::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	formulaTextBox->Focus(Windows::UI::Xaml::FocusState::Programmatic);
}
