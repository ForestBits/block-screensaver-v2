#include "JsPropertyCache.h"
#include "stringUtils.h"
#include "ChakraCore.h"
#include <map>

namespace
{
	std::map<std::string, JsPropertyIdRef> properties;

	JsContextRef cachedContext = nullptr;
}

JsPropertyIdRef jsGetProperty(const std::string & name)
{
	JsContextRef context;

	JsGetCurrentContext(&context);

	if (context != cachedContext)
	{
		cachedContext = context;

		properties.clear();
	}

	auto iter = properties.find(name);

	if (iter == properties.end())
	{
		std::wstring wName = stringToWString(name);

		JsPropertyIdRef property;

		JsGetPropertyIdFromName(wName.c_str(), &property);

		properties[name] = property;

		return property;
	}
	else
		return iter->second;
}