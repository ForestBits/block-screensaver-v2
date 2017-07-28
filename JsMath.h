#ifndef JSMATH_H
#define JSMATH_H

#include "ChakraCore.h"

JsValueRef CALLBACK jsTruncate(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsAbs(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsSign(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsCeil(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsFloor(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsRound(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsSqrt(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsSin(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsCos(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsTan(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsAtan2(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsPow(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);


#endif