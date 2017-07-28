#include "JsMath.h"
#include "jsException.h"
#include "JsPropertyCache.h"

JsValueRef CALLBACK jsTruncate(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	int value = valueAsDouble;

	JsValueRef returnValue;

	JsIntToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsAbs(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (number)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	double value = std::abs(valueAsDouble);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsSign(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (number)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	int sign;

	double elipson = 0.00000001;

	if (std::abs(valueAsDouble) < elipson)
		sign = 0;
	else if (valueAsDouble < 0)
		sign = -1;
	else
		sign = 1;

	JsValueRef returnValue;

	JsIntToNumber(sign, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsCeil(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	int value = std::ceil(valueAsDouble);

	JsValueRef returnValue;

	JsIntToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsFloor(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	int value = std::floor(valueAsDouble);

	JsValueRef returnValue;

	JsIntToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsRound(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	int value = std::round(valueAsDouble);

	JsValueRef returnValue;

	JsIntToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsSqrt(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	double value = std::sqrt(valueAsDouble);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsSin(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	double value = std::sin(valueAsDouble);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsCos(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	double value = std::cos(valueAsDouble);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsTan(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("Missing required argument; requires (decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	JsValueType argType;

	JsGetValueType(arg, &argType);

	if (argType != JsNumber)
	{
		throwRangeError("argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double valueAsDouble;

	JsNumberToDouble(arg, &valueAsDouble);

	double value = std::tan(valueAsDouble);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsAtan2(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 3)
	{
		throwRangeError("Missing required argument; requires (decimal, decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef yArg = args[1], xArg = args[2];

	JsValueType yType, xType;

	JsGetValueType(yArg, &yType);
	JsGetValueType(xArg, &xType);

	if (yType != JsNumber)
	{
		throwRangeError("first argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (xType != JsNumber)
	{
		throwRangeError("second argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double y, x;

	JsNumberToDouble(yArg, &y);
	JsNumberToDouble(xArg, &x);

	double value = std::atan2(y, x);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsPow(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 3)
	{
		throwRangeError("Missing required argument; requires (decimal, decimal)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef yArg = args[1], xArg = args[2];

	JsValueType yType, xType;

	JsGetValueType(yArg, &yType);
	JsGetValueType(xArg, &xType);

	if (yType != JsNumber)
	{
		throwRangeError("first argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (xType != JsNumber)
	{
		throwRangeError("second argument is not a number");

		return JS_INVALID_REFERENCE;
	}

	double y, x;

	JsNumberToDouble(yArg, &y);
	JsNumberToDouble(xArg, &x);

	double value = std::pow(y, x);

	JsValueRef returnValue;

	JsDoubleToNumber(value, &returnValue);

	return returnValue;
}
