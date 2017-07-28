#include "stringUtils.h"
#include <locale>
#include <codecvt>

std::string wStringToString(const std::wstring & wString)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	return converter.to_bytes(wString);
}

std::wstring stringToWString(const std::string & string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	return converter.from_bytes(string);
}