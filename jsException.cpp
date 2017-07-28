#include "jsException.h"
#include "JsPrint.h"
#include "JsPropertyCache.h"
#include "ChakraCore.h"
#include "stringUtils.h"
#include <iostream>

std::string getErrorExceptionMessage(JsValueRef exception)
{
	JsValueRef message, line, column, source;

	JsGetProperty(exception, jsGetProperty("message"), &message);
	JsGetProperty(exception, jsGetProperty("line"), &line);
	JsGetProperty(exception, jsGetProperty("column"), &column);
	JsGetProperty(exception, jsGetProperty("source"), &source);

	int lineValue, columnValue;

	JsNumberToInt(line, &lineValue);
	JsNumberToInt(column, &columnValue);

	++lineValue;

	const wchar_t * sourceStringRaw;

	size_t sourceLength;

	bool validSource = JsStringToPointer(source, &sourceStringRaw, &sourceLength) == JsNoError;

	std::string sourceString;
	
	if (validSource)
		sourceString = wStringToString(sourceStringRaw);

	const wchar_t * exceptionStringRaw;

	size_t stringLength;

	JsStringToPointer(message, &exceptionStringRaw, &stringLength);

	std::string exceptionString = wStringToString(exceptionStringRaw);

	std::string locationString = "Line ";

	locationString.append(std::to_string(lineValue)).append(" Column ").append(std::to_string(columnValue));

	std::string errorCodeString = (validSource ? "\"" + sourceString + "\"" : "");

	std::string errorNameString = "Error: " + exceptionString;

	return errorNameString + "  " + locationString + "  " + errorCodeString;
}

std::string getTypeErrorExceptionMessage(JsValueRef exception)
{
	JsValueRef stack;

	JsGetProperty(exception, jsGetProperty("stack"), &stack);

	const wchar_t * stackRaw;

	size_t length;

	JsStringToPointer(stack, &stackRaw, &length);

	return wStringToString(stackRaw);
}

std::string getSyntaxErrorExceptionMessage(JsValueRef exception)
{
	return getErrorExceptionMessage(exception);
}

std::string getReferenceErrorExceptionMessage(JsValueRef exception)
{
	return getTypeErrorExceptionMessage(exception);
}

std::string getRangeErrorExceptionMessage(JsValueRef exception)
{
	return getTypeErrorExceptionMessage(exception);
}

std::string getExceptionMessage()
{
	JsValueRef exception;

	JsGetAndClearException(&exception);

	JsValueType type;

	JsGetValueType(exception, &type);

	JsValueRef globalObject;

	JsGetGlobalObject(&globalObject);

	JsValueRef errorConstructor, typeErrorConstructor, syntaxErrorConstructor, referenceErrorConstructor, rangeErrorConstructor;

	JsGetProperty(globalObject, jsGetProperty("Error"), &errorConstructor);
	JsGetProperty(globalObject, jsGetProperty("TypeError"), &typeErrorConstructor);
	JsGetProperty(globalObject, jsGetProperty("SyntaxError"), &syntaxErrorConstructor);
	JsGetProperty(globalObject, jsGetProperty("ReferenceError"), &referenceErrorConstructor);
	JsGetProperty(globalObject, jsGetProperty("RangeError"), &rangeErrorConstructor);

	bool isError, isTypeError, isSyntaxError, isReferenceError, isRangeError;

	JsInstanceOf(exception, errorConstructor, &isError);
	JsInstanceOf(exception, typeErrorConstructor, &isTypeError);
	JsInstanceOf(exception, syntaxErrorConstructor, &isSyntaxError);
	JsInstanceOf(exception, referenceErrorConstructor, &isReferenceError);
	JsInstanceOf(exception, rangeErrorConstructor, &isRangeError);

	if (isTypeError)
		return getTypeErrorExceptionMessage(exception);
	else if (isSyntaxError)
		return getSyntaxErrorExceptionMessage(exception);
	else if (isReferenceError)
		return getReferenceErrorExceptionMessage(exception);
	else if (isRangeError)
		return getRangeErrorExceptionMessage(exception);
	else if (isError)
		return getErrorExceptionMessage(exception);
	else
	{
		std::cout << "Error: ";

		std::cout << jsGetObjectAsString(exception) << "\n";

		std::cout << "\n";

		return "";
	}
}

void throwRangeError(const std::string & message)
{
	std::wstring wMessage = stringToWString(message);

	JsValueRef messageObject;

	JsPointerToString(wMessage.c_str(), wMessage.size(), &messageObject);
		
	JsValueRef exception;

	JsCreateRangeError(messageObject, &exception);

	JsSetException(exception);
}
