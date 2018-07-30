#ifndef WIXINCLUDE_H
#define WIXINCLUDE_H
#pragma once
//wix includes:
#include <dutil.h>
//macros to extend the wix system, to exit on false and exit if values are equal or not equal to a given value.
#define ExitOnFalse(v, x, e, s)   if (v ==false) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitOnTrue(v, x, e, s)   if (v ==true) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitOnSpecificValue(p, v, x, e, s)   if (v == p) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitIfValueGreaterThanSuppliedValue(p, v, x, e, s)   if (v > p) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitIfValueLessThanOrEqualToSuppliedValue(p, v, x, e, s)   if (v <= p) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitIfValueLessThanSuppliedValue(p, v, x, e, s)   if (v < p) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitIfValueNotEqualToSuppliedValue(p, v, x, e, s)   if (p != v) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitFunctionWithMessage(x, s)   { ExitTrace(x, s);  goto LExit; }
#define ExitFunctionWithMessageAndStatusCode(x, e, s)  { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#define ExitOnNotNull(p, x, e, s)   if (NULL != p) { x = e; Dutil_RootFailure(__FILE__, __LINE__, x); ExitTrace(x, s);  goto LExit; }
#endif