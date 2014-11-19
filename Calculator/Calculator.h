#pragma once

#ifdef CALCULATOR_EXPORTS
#define CALCULATOR_API __declspec(dllexport)
#else
#define CALCULATOR_API __declspec(dllimport)
#endif
