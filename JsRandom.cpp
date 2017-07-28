#include "JsRandom.h"
#include "JsPrint.h"
#include "jsException.h"
#include "Random.h"

Random random;

JsValueRef CALLBACK jsRandomSetSeed(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	JsValueRef arg = args[1];

	JsValueType type;

	JsGetValueType(arg, &type);

	if (type != JsNumber)
	{
		throwRangeError("Argument must be a number");

		return JS_INVALID_REFERENCE;
	}

	int number;

	JsNumberToInt(arg, &number);

	if (number < 0)
	{
		throwRangeError("Argument must be >= 0");

		return JS_INVALID_REFERENCE;
	}

	random.setSeed(number);

	return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK jsRandomGetBoolean(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	bool randomBool = random.getBool();

	JsValueRef boolValue;

	JsBoolToBoolean(randomBool, &boolValue);

	return boolValue;
}

JsValueRef CALLBACK jsRandomGetInteger(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count == 1)
	{
		int randomInt = random.getInt();

		JsValueRef intValue;

		JsIntToNumber(randomInt, &intValue);

		return intValue;
	}

	if (count == 2)
	{
		JsValueRef maxRef = args[1];

		JsValueType type;

		JsGetValueType(maxRef, &type);

		if (type != JsNumber)
		{
			throwRangeError("max must be a number");

			return JS_INVALID_REFERENCE;
		}

		int max;

		JsNumberToInt(maxRef, &max);

		if (max < 0)
		{
			throwRangeError("max must be >= 0");

			return JS_INVALID_REFERENCE;
		}

		int randomInt = random.getInt(max);

		JsValueRef intValue;

		JsIntToNumber(randomInt, &intValue);

		return intValue;
	}

	if (count >= 3)
	{
		JsValueRef minRef = args[1];
		JsValueRef maxRef = args[2];

		JsValueType minType, maxType;

		JsGetValueType(minRef, &minType);
		JsGetValueType(maxRef, &maxType);

		if (minType != JsNumber)
		{
			throwRangeError("min must be a number");

			return JS_INVALID_REFERENCE;
		}

		if (maxType != JsNumber)
		{
			throwRangeError("max must be a number");

			return JS_INVALID_REFERENCE;
		}

		int min, max;

		JsNumberToInt(minRef, &min);
		JsNumberToInt(maxRef, &max);

		if (min < 0)
		{
			throwRangeError("min must be >= 0");

			return JS_INVALID_REFERENCE;
		}

		if (max < 0)
		{
			throwRangeError("max must be >= 0");

			return JS_INVALID_REFERENCE;
		}

		if (min > max)
		{
			int temp = max;

			max = min;

			min = temp;
		}

		int randomInt = random.getInt((max - min) + 1) + min;

		JsValueRef intValue;

		JsIntToNumber(randomInt, &intValue);

		return intValue;
	}
}

JsValueRef CALLBACK jsRandomGetDecimal(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	double randomDouble = random.getDouble();

	JsValueRef doubleValue;

	JsDoubleToNumber(randomDouble, &doubleValue);

	return doubleValue;
}

JsValueRef CALLBACK jsRandomGetSign(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	int randomSign = random.getBool() ? 1 : -1;

	JsValueRef signValue;

	JsIntToNumber(randomSign, &signValue);

	return signValue;
}
