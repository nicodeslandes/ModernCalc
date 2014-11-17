//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Calculator.h"
#include <string>
#include <sstream>

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
		int res = calculate(formula);
		resultTextBox->Text = res.ToString();
	}
	catch (const ParsingError& error)
	{
		resultTextBox->Text = ref new Platform::String(error.getMessage().c_str());
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
