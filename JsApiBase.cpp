#include "JsApiBase.h"
#include "jsException.h"
#include "JsPropertyCache.h"
#include <SFML/System/Vector2.hpp>
#include <thread>
#include <chrono>

JsValueRef CALLBACK jsInBounds(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	sf::Vector2i * size = (sf::Vector2i *) callbackState;

	if (count < 3)
	{
		throwRangeError("Missing required arguments; Requires (int, int)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef number1Value = args[1], number2Value = args[2];

	JsValueType number1Type, number2Type;

	JsGetValueType(number1Value, &number1Type);
	JsGetValueType(number2Value, &number2Type);

	if (number1Type != JsNumber)
	{
		throwRangeError("Argument 1 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number2Type != JsNumber)
	{
		throwRangeError("Argument 2 is not a number");

		return JS_INVALID_REFERENCE;
	}

	int number1, number2;

	JsNumberToInt(number1Value, &number1);
	JsNumberToInt(number2Value, &number2);

	bool inBounds = (number1 >= 0 && number2 >= 0 && number1 < size->x && number2 < size->y);

	JsValueRef returnValue;

	JsBoolToBoolean(inBounds, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsGetDistance(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 5)
	{
		throwRangeError("Missing required arguments; Requires (int, int, int, int)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef number1Value = args[1], number2Value = args[2], number3Value = args[3], number4Value = args[4];

	JsValueType number1Type, number2Type, number3Type, number4Type;

	JsGetValueType(number1Value, &number1Type);
	JsGetValueType(number2Value, &number2Type);
	JsGetValueType(number3Value, &number3Type);
	JsGetValueType(number4Value, &number4Type);

	if (number1Type != JsNumber)
	{
		throwRangeError("Argument 1 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number2Type != JsNumber)
	{
		throwRangeError("Argument 2 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number3Type != JsNumber)
	{
		throwRangeError("Argument 3 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number4Type != JsNumber)
	{
		throwRangeError("Argument 4 is not a number");

		return JS_INVALID_REFERENCE;
	}

	int number1, number2, number3, number4;

	JsNumberToInt(number1Value, &number1);
	JsNumberToInt(number2Value, &number2);
	JsNumberToInt(number3Value, &number3);
	JsNumberToInt(number4Value, &number4);

	double distance = std::sqrt(std::pow(number1 - number3, 2) + std::pow(number2 - number4, 2));

	JsValueRef returnValue;

	JsDoubleToNumber(distance, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsGetManhattanDistance(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 5)
	{
		throwRangeError("Missing required arguments; Requires (int, int, int, int)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef number1Value = args[1], number2Value = args[2], number3Value = args[3], number4Value = args[4];

	JsValueType number1Type, number2Type, number3Type, number4Type;

	JsGetValueType(number1Value, &number1Type);
	JsGetValueType(number2Value, &number2Type);
	JsGetValueType(number3Value, &number3Type);
	JsGetValueType(number4Value, &number4Type);

	if (number1Type != JsNumber)
	{
		throwRangeError("Argument 1 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number2Type != JsNumber)
	{
		throwRangeError("Argument 2 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number3Type != JsNumber)
	{
		throwRangeError("Argument 3 is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (number4Type != JsNumber)
	{
		throwRangeError("Argument 4 is not a number");

		return JS_INVALID_REFERENCE;
	}

	double number1, number2, number3, number4;

	JsNumberToDouble(number1Value, &number1);
	JsNumberToDouble(number2Value, &number2);
	JsNumberToDouble(number3Value, &number3);
	JsNumberToDouble(number4Value, &number4);

	double distance = std::abs(number1 - number3) + std::abs(number2 - number4);

	JsValueRef returnValue;

	JsDoubleToNumber(distance, &returnValue);

	return returnValue;
}

JsValueRef CALLBACK jsExportAPI(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	JsValueRef globalObject;

	JsGetGlobalObject(&globalObject);

	JsValueRef apiObject;

	JsGetProperty(globalObject, jsGetProperty("api"), &apiObject);

	JsValueRef propertiesArray;

	JsGetOwnPropertyNames(apiObject, &propertiesArray);

	JsValueRef lengthValue;

	JsGetProperty(propertiesArray, jsGetProperty("length"), &lengthValue);

	int length;

	JsNumberToInt(lengthValue, &length);

	for (int i = 0; i < length; ++i)
	{
		JsValueRef propertyName;

		JsValueRef indexValue;

		JsIntToNumber(i, &indexValue);

		JsGetIndexedProperty(propertiesArray, indexValue, &propertyName);

		const wchar_t * str;

		size_t len;

		JsStringToPointer(propertyName, &str, &len);

		JsPropertyIdRef property;

		JsGetPropertyIdFromName(str, &property);

		JsValueRef propertyValue;

		JsGetProperty(apiObject, property, &propertyValue);

		JsSetProperty(globalObject, property, propertyValue, true);
	}

	return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK jsSleep(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	double seconds;

	JsNumberToDouble(args[1], &seconds);

	std::this_thread::sleep_for(std::chrono::milliseconds((int) (seconds*1000)));

	return JS_INVALID_REFERENCE;
}