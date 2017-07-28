#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "ChakraCore.h"

namespace sf
{
	class Color;
}

struct Landscape
{
	int blocksWide;
	int blocksHigh;

	sf::Color ** blocks;
};


JsValueRef CALLBACK jsGetColor(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsSetColor(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);


#endif