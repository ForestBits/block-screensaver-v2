#ifndef JSAPIBASE_H
#define JSAPIBASE_H

#include "ChakraCore.h"

JsValueRef CALLBACK jsInBounds(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsGetDistance(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsGetManhattanDistance(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsExportAPI(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

JsValueRef CALLBACK jsSleep(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

#endif