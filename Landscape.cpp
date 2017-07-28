#include "Landscape.h"
#include "jsException.h"
#include <SFML/Graphics/Color.hpp>

namespace
{
	JsPropertyIdRef redProperty;
	JsPropertyIdRef greenProperty;
	JsPropertyIdRef blueProperty;

	JsValueRef colorConstructor;

	bool init = false;

	void initialize()
	{
		init = true;

		JsGetPropertyIdFromName(L"red", &redProperty);
		JsGetPropertyIdFromName(L"green", &greenProperty);
		JsGetPropertyIdFromName(L"blue", &blueProperty);

		JsPropertyIdRef colorProperty;

		JsGetPropertyIdFromName(L"Color", &colorProperty);

		JsValueRef globalObject;

		JsGetGlobalObject(&globalObject);

		JsGetProperty(globalObject, colorProperty, &colorConstructor);
	}
}


JsValueRef CALLBACK jsGetColor(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (!init)
		initialize();

	return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK jsSetColor(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState)
{
	if (!init)
		initialize();

	Landscape * landscape = (Landscape *) callbackState;

	if (count < 4)
	{
		throwRangeError("Missing required argument. Requires (int, int, Color)");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef xArg = args[1], yArg = args[2], colorArg = args[3];

	JsValueType xType, yType;

	JsGetValueType(xArg, &xType);
	JsGetValueType(yArg, &yType);

	if (xType != JsNumber)
	{
		throwRangeError("x is not a number");

		return JS_INVALID_REFERENCE;
	}

	if (yType != JsNumber)
	{
		throwRangeError("y is not a number");

		return JS_INVALID_REFERENCE;
	}

	int xValue, yValue;

	JsNumberToInt(xArg, &xValue);
	JsNumberToInt(yArg, &yValue);

	if (xValue < 0 || xValue >= landscape->blocksWide)
	{
		throwRangeError("x must be in the range 0-blocksWide");

		return JS_INVALID_REFERENCE;
	}

	if (yValue < 0 || yValue >= landscape->blocksHigh)
	{
		throwRangeError("y must be in the range 0-blocksHigh");

		return JS_INVALID_REFERENCE;
	}

	bool isColor = false;

	JsInstanceOf(colorArg, colorConstructor, &isColor);

	if (!isColor)
	{
		throwRangeError("Argument provided is not a color");

		return JS_INVALID_REFERENCE;
	}

	JsValueRef redValue, greenValue, blueValue;

	JsGetProperty(colorArg, redProperty, &redValue);
	JsGetProperty(colorArg, greenProperty, &greenValue);
	JsGetProperty(colorArg, blueProperty, &blueValue);

	JsValueType redType, greenType, blueType;

	JsGetValueType(redValue, &redType);
	JsGetValueType(greenValue, &greenType);
	JsGetValueType(blueValue, &blueType);

	if (redType != JsNumber)
	{
		throwRangeError("Color red property is not number");

		return JS_INVALID_REFERENCE;
	}

	if (greenType != JsNumber)
	{
		throwRangeError("Color green property is not number");

		return JS_INVALID_REFERENCE;
	}

	if (blueType != JsNumber)
	{
		throwRangeError("Color blue property is not number");

		return JS_INVALID_REFERENCE;
	}

	int red, green, blue;

	JsNumberToInt(redValue, &red);
	JsNumberToInt(greenValue, &green);
	JsNumberToInt(blueValue, &blue);

	if (red < 0 || red > 255)
	{
		throwRangeError("red value must be in the range 0-255");

		return JS_INVALID_REFERENCE;
	}

	if (green < 0 || green > 255)
	{
		throwRangeError("green value must be in the range 0-255");

		return JS_INVALID_REFERENCE;
	}

	if (blue < 0 || blue > 255)
	{
		throwRangeError("blue value must be in the range 0-255");

		return JS_INVALID_REFERENCE;
	}

	landscape->blocks[xValue][yValue] = sf::Color(red, green, blue);

	return JS_INVALID_REFERENCE;
}