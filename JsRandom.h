#ifndef JSRANDOM_H
#define JSRANDOM_H

#include "ChakraCore.h"

JsValueRef CALLBACK jsRandomSetSeed(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);
JsValueRef CALLBACK jsRandomGetBoolean(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);
JsValueRef CALLBACK jsRandomGetInteger(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);
JsValueRef CALLBACK jsRandomGetDecimal(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);
JsValueRef CALLBACK jsRandomGetSign(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

#endif