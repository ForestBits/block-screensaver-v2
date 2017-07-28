#include "JsPrint.h"
#include "jsException.h"
#include "JsPropertyCache.h"
#include "stringUtils.h"
#include <iostream>


std::string jsGetTypeAsString(JsValueRef arg, JsValueType type)
{
	if (type == JsUndefined)
		return "undefined";

	if (type == JsNull)
		return "null";

	if (type == JsNumber)
	{
		int numberInt;

		JsNumberToInt(arg, &numberInt);

		double numberDouble;

		JsNumberToDouble(arg, &numberDouble);

		bool isDouble = numberDouble - numberInt > 0.01;

		return (isDouble ? std::to_string(numberDouble) : std::to_string(numberInt));
	}

	if (type == JsString)
	{
		const wchar_t * string;

		size_t length;

		JsStringToPointer(arg, &string, &length);

		std::wstring wString(string);

		return "\"" + std::string(wString.begin(), wString.end()) + "\"";
	}

	if (type == JsBoolean)
	{
		bool boolValue;

		JsBooleanToBool(arg, &boolValue);

		return boolValue ? "true" : "false";
	}

	if (type == JsFunction)
		return "(function)";

	if (type == JsError)
	{
		JsValueRef messageString;

		JsGetProperty(arg, jsGetProperty("message"), &messageString);

		const wchar_t * string;

		size_t length;

		JsStringToPointer(messageString, &string, &length);

		std::wstring wString(string);

		return "Error (\"" + std::string(wString.begin(), wString.end()) + "\")";
	}
}

std::string jsGetArrayAsString(JsValueRef array)
{
	JsValueRef arrayLength;

	JsGetProperty(array, jsGetProperty("length"), &arrayLength);

	int length;

	JsNumberToInt(arrayLength, &length);

	std::string string;

	string.append("[");

	for (int i = 0; i < length; ++i)
	{
		JsValueRef indexNumber;

		JsIntToNumber(i, &indexNumber);

		JsValueRef indexValue;

		JsGetIndexedProperty(array, indexNumber, &indexValue);

		JsValueType indexType;

		JsGetValueType(indexValue, &indexType);

		if (indexType == JsArray)
		{
			if (i != 0)
				string.append(", ");

			string.append(jsGetArrayAsString(indexValue));
		}
		else if (indexType == JsObject)
		{
			if (i != 0)
				string.append(", ");

			string.append(jsGetObjectAsString(indexValue));
		}
		else
		{
			if (i != 0)
				string.append(", ");

			string.append(jsGetTypeAsString(indexValue, indexType));
		}
	}

	string.append("]");

	return string;
}

std::string jsGetObjectAsString(JsValueRef object)
{
	std::string str;

	str.append("{");

	JsValueRef propertiesArray;

	JsGetOwnPropertyNames(object, &propertiesArray);

	for (int i = 0; ; ++i)
	{
		bool hasIndex = false;

		JsValueRef indexNumber;

		JsIntToNumber(i, &indexNumber);

		JsHasIndexedProperty(propertiesArray, indexNumber, &hasIndex);

		if (hasIndex)
		{
			JsValueRef indexRef;

			JsGetIndexedProperty(propertiesArray, indexNumber, &indexRef);

			const wchar_t * wString;

			size_t length;

			JsStringToPointer(indexRef, &wString, &length);

			std::string string = wStringToString(wString);

			JsValueRef value;

			JsGetProperty(object, jsGetProperty(string), &value);

			JsValueType type;

			JsGetValueType(value, &type);

			if (i != 0)
				str.append(", ");

			str.append(string).append(": ");

			if (type == JsUndefined || type == JsNull || type == JsNumber || type == JsString
				|| type == JsBoolean || type == JsFunction || type == JsError)

				str.append(jsGetTypeAsString(value, type));
			else if (type == JsArray)
				str.append(jsGetArrayAsString(value));
			else if (type == JsObject)
				str.append(jsGetObjectAsString(value));
		}
		else
		{
			str.append("}");

			break;
		}
	}

	return str;
}

std::string JsGetValueAsString(JsValueRef arg)
{
	std::string string;

	JsValueType type;

	JsGetValueType(arg, &type);

	if (type == JsUndefined || type == JsNull || type == JsNumber || type == JsString
		|| type == JsBoolean || type == JsFunction || type == JsError)
		return jsGetTypeAsString(arg, type);

	if (type == JsObject)
		return jsGetObjectAsString(arg);

	if (type == JsArray)
		return jsGetArrayAsString(arg);

	if (type == JsSymbol)
		return "(symbol)";

	return "(unknown type)";
}

JsValueRef CALLBACK jsPrint(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (count < 2)
	{
		throwRangeError("No arguments provided.");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef arg = args[1];

	std::cout << JsGetValueAsString(arg) << "\n";

	return JS_INVALID_REFERENCE;
}
