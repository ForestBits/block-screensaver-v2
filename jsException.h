#ifndef JSEXCEPTION_H
#define JSEXCEPTION_H

#include <string>

std::string getExceptionMessage();

void throwRangeError(const std::string & message);

#endif