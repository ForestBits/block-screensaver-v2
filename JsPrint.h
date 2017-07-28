#ifndef JSPRINT_H
#define JSPRINT_H

#include "ChakraCore.h"
#include <string>

std::string jsGetTypeAsString(JsValueRef arg, JsValueType type);

std::string jsGetObjectAsString(JsValueRef object);

std::string jsGetArrayAsString(JsValueRef array, bool firstTerm);

std::string JsGetValueAsString(JsValueRef arg);

JsValueRef CALLBACK jsPrint(JsValueRef callee, bool isConstructCall, JsValueRef * args, unsigned short count, void * callbackState);

#endif