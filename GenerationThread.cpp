#include "GenerationThread.h"
#include "JsPrint.h"
#include "JsRandom.h"
#include "JsMath.h"
#include "JsApiBase.h"
#include "jsException.h"
#include "JsPropertyCache.h"
#include "stringUtils.h"
#include "ChakraCore.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>
#include <map>

namespace
{
	JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;

	JsContextRef context = JS_INVALID_REFERENCE;

	std::queue<GenerationJob> jobs;

	std::mutex jobMutex;
	std::mutex statusMutex;

	std::atomic<bool> stopThread = false;

	int width;
	int height;

	sf::Vector2i inBoundsFunctionSize;

	const std::vector<std::wstring> * utilityScripts;

	int nextId = 0;

	std::vector<std::pair<int, std::string>> errors;

	std::vector<std::pair<int, sf::Color **>> generations;

	std::vector<GenerationStatus> statuses;



	void runUtilityScripts()
	{
		for (auto string : *utilityScripts)
		{
			JsErrorCode code = JsRunScript(string.c_str(), 0, L"", nullptr);

			if (code == JsErrorScriptException)
			{
				std::cout << getExceptionMessage() << "\n";

				return;
			}
		}
	}

	void populateGeneratorInterface()
	{
		JsValueRef globalObject;

		JsGetGlobalObject(&globalObject);

		JsValueRef apiObject;

		JsCreateObject(&apiObject);

		JsSetProperty(globalObject, jsGetProperty("api"), apiObject, true);

		JsValueRef printFunction;

		JsCreateFunction(jsPrint, nullptr, &printFunction);

		JsSetProperty(apiObject, jsGetProperty("print"), printFunction, true);

		JsValueRef blocksWideValue, blocksHighValue;

		JsIntToNumber(width, &blocksWideValue);
		JsIntToNumber(height, &blocksHighValue);

		JsSetProperty(apiObject, jsGetProperty("blocksWide"), blocksWideValue, true);
		JsSetProperty(apiObject, jsGetProperty("blocksHigh"), blocksHighValue, true);

		JsValueRef randomObject;

		JsCreateObject(&randomObject);

		JsSetProperty(apiObject, jsGetProperty("random"), randomObject, true);

		JsValueRef jsRandomSetSeedFunction, jsRandomGetBooleanFunction, jsRandomGetIntegerFunction, jsRandomGetDecimalFunction, jsRandomGetSignFunction;

		JsCreateFunction(jsRandomSetSeed, nullptr, &jsRandomSetSeedFunction);
		JsCreateFunction(jsRandomGetBoolean, nullptr, &jsRandomGetBooleanFunction);
		JsCreateFunction(jsRandomGetInteger, nullptr, &jsRandomGetIntegerFunction);
		JsCreateFunction(jsRandomGetDecimal, nullptr, &jsRandomGetDecimalFunction);
		JsCreateFunction(jsRandomGetSign, nullptr, &jsRandomGetSignFunction);

		JsSetProperty(randomObject, jsGetProperty("setSeed"), jsRandomSetSeedFunction, true);
		JsSetProperty(randomObject, jsGetProperty("getBoolean"), jsRandomGetBooleanFunction, true);
		JsSetProperty(randomObject, jsGetProperty("getInteger"), jsRandomGetIntegerFunction, true);
		JsSetProperty(randomObject, jsGetProperty("getDecimal"), jsRandomGetDecimalFunction, true);
		JsSetProperty(randomObject, jsGetProperty("getSign"), jsRandomGetSignFunction, true);

		JsValueRef exportAPIFunction, inBoundsFunction, getDistanceFunction, getManhattanDistanceFunction, sleepFunction;

		JsCreateFunction(jsExportAPI, nullptr, &exportAPIFunction);
		//JsCreateFunction(jsInBounds, &inBoundsFunctionSize, &inBoundsFunction);
		JsCreateFunction(jsGetDistance, nullptr, &getDistanceFunction);
		JsCreateFunction(jsGetManhattanDistance, nullptr, &getManhattanDistanceFunction);
		//JsCreateFunction(jsSleep, nullptr, &sleepFunction);

		JsSetProperty(apiObject, jsGetProperty("export"), exportAPIFunction, true);
		//JsSetProperty(apiObject, jsGetProperty("inBounds"), inBoundsFunction, true);
		JsSetProperty(apiObject, jsGetProperty("getDistance"), getDistanceFunction, true);
		JsSetProperty(apiObject, jsGetProperty("getManhattanDistance"), getManhattanDistanceFunction, true);
		//JsSetProperty(apiObject, jsGetProperty("sleep"), sleepFunction, true);

		JsValueRef mathObject;

		JsCreateObject(&mathObject);

		JsSetProperty(apiObject, jsGetProperty("math"), mathObject, true);

		JsValueRef truncateFunction, absFunction, signFunction, ceilFunction, floorFunction, roundFunction, sqrtFunction, sinFunction, cosFunction, tanFunction, atan2Function, powFunction;

		JsCreateFunction(jsTruncate, nullptr, &truncateFunction);
		JsCreateFunction(jsAbs, nullptr, &absFunction);
		JsCreateFunction(jsSign, nullptr, &signFunction);
		JsCreateFunction(jsCeil, nullptr, &ceilFunction);
		JsCreateFunction(jsFloor, nullptr, &floorFunction);
		JsCreateFunction(jsRound, nullptr, &roundFunction);
		JsCreateFunction(jsSqrt, nullptr, &sqrtFunction);
		JsCreateFunction(jsSin, nullptr, &sinFunction);
		JsCreateFunction(jsCos, nullptr, &cosFunction);
		JsCreateFunction(jsTan, nullptr, &tanFunction);
		JsCreateFunction(jsAtan2, nullptr, &atan2Function);
		JsCreateFunction(jsPow, nullptr, &powFunction);

		JsSetProperty(mathObject, jsGetProperty("truncate"), truncateFunction, true);
		JsSetProperty(mathObject, jsGetProperty("abs"), absFunction, true);
		JsSetProperty(mathObject, jsGetProperty("sign"), signFunction, true);
		JsSetProperty(mathObject, jsGetProperty("ceil"), ceilFunction, true);
		JsSetProperty(mathObject, jsGetProperty("floor"), floorFunction, true);
		JsSetProperty(mathObject, jsGetProperty("round"), roundFunction, true);
		JsSetProperty(mathObject, jsGetProperty("sqrt"), sqrtFunction, true);
		JsSetProperty(mathObject, jsGetProperty("sin"), sinFunction, true);
		JsSetProperty(mathObject, jsGetProperty("cos"), cosFunction, true);
		JsSetProperty(mathObject, jsGetProperty("tan"), tanFunction, true);
		JsSetProperty(mathObject, jsGetProperty("atan2"), atan2Function, true);
		JsSetProperty(mathObject, jsGetProperty("pow"), powFunction, true);

		JsValueRef piValue;

		JsDoubleToNumber(3.14159265359, &piValue);

		JsSetProperty(mathObject, jsGetProperty("PI"), piValue, true);
		
		JsValueRef blockArray;

		JsCreateArray(width, &blockArray);

		for (int x = 0; x < width; ++x)
		{
			JsValueRef indexXValue;

			JsIntToNumber(x, &indexXValue);

			JsValueRef subArray;

			JsCreateArray(height, &subArray);

			JsSetIndexedProperty(blockArray, indexXValue, subArray);
		}

		JsSetProperty(apiObject, jsGetProperty("blocks"), blockArray, true);

		runUtilityScripts();
	}

	void recordJobError(const GenerationJob & job, const std::string & errorMessage)
	{
		errors.push_back(std::make_pair(job.id, errorMessage));

		statusMutex.lock();

		statuses[job.id] = GenerationStatus::ERROR_OCCURED;

		statusMutex.unlock();

		if (job.blocks)
		{
			for (int x = 0; x < width; ++x)
				delete job.blocks[x];

			delete job.blocks;
		}
	}

	bool checkAndAssignBlocks(const GenerationJob & job)
	{
		JsPropertyIdRef redProperty = jsGetProperty("red");
		JsPropertyIdRef greenProperty = jsGetProperty("green");
		JsPropertyIdRef blueProperty = jsGetProperty("blue");

		JsValueRef globalObject;

		JsGetGlobalObject(&globalObject);

		JsValueRef apiObject;

		JsGetProperty(globalObject, jsGetProperty("api"), &apiObject);

		JsValueType apiType;

		JsGetValueType(apiObject, &apiType);

		if (apiType != JsObject)
		{
			recordJobError(job, "type of api is not object");

			return false;
		}

		JsValueRef blocksValue;

		JsGetProperty(apiObject, jsGetProperty("blocks"), &blocksValue);

		JsValueType blockType;

		JsGetValueType(blocksValue, &blockType);

		if (blockType != JsArray)
		{
			recordJobError(job, "type of blocks array is not array");

			return false;
		}

		for (int x = 0; x < width; ++x)
		{
			JsValueRef subArray;

			JsValueRef indexXValue;

			JsIntToNumber(x, &indexXValue);

			JsGetIndexedProperty(blocksValue, indexXValue, &subArray);

			JsValueType indexType;

			JsGetValueType(subArray, &indexType);

			if (indexType != JsArray)
			{
				recordJobError(job, "item at index " + std::to_string(x) + " is not of type array");

				return false;
			}

			for (int y = 0; y < height; ++y)
			{
				JsValueRef indexYValue;

				JsIntToNumber(y, &indexYValue);

				JsValueRef value;

				JsGetIndexedProperty(subArray, indexYValue, &value);

				JsValueType valueType;

				JsGetValueType(value, &valueType);

				if (valueType != JsObject)
				{
					recordJobError(job, "value at index (" + std::to_string(x) + ", " + std::to_string(y) + ") is not a Color");

					return false;
				}

				JsValueRef redValue, greenValue, blueValue;

				JsGetProperty(value, redProperty, &redValue);
				JsGetProperty(value, greenProperty, &greenValue);
				JsGetProperty(value, blueProperty, &blueValue);

				JsValueType redType, greenType, blueType;

				JsGetValueType(redValue, &redType);
				JsGetValueType(greenValue, &greenType);
				JsGetValueType(blueValue, &blueType);

				if (redType != JsNumber)
				{
					recordJobError(job, "color at index (" + std::to_string(x) + ", " + std::to_string(y) + ") red property is not number\ncolor: " + jsGetObjectAsString(value));

					return false;
				}

				if (greenType != JsNumber)
				{
					recordJobError(job, "color at index (" + std::to_string(x) + ", " + std::to_string(y) + ") green property is not number\ncolor: " + jsGetObjectAsString(value));

					return false;
				}

				if (blueType != JsNumber)
				{
					recordJobError(job, "color at index (" + std::to_string(x) + ", " + std::to_string(y) + ") blue property is not number\ncolor: " + jsGetObjectAsString(value));

					return false;
				}

				int red, green, blue;

				JsNumberToInt(redValue, &red);
				JsNumberToInt(greenValue, &green);
				JsNumberToInt(blueValue, &blue);

				if (red < 0 || red > 255)
				{
					recordJobError(job, "color at index (" + std::to_string(x) + ", " + std::to_string(y) + ") red value is out of range\ncolor: " + jsGetObjectAsString(value));

					return false;
				}

				if (green < 0 || green > 255)
				{
					recordJobError(job, "color at index (" + std::to_string(x) + ", " + std::to_string(y) + ") green value is out of range\ncolor: " + jsGetObjectAsString(value));

					return false;
				}

				if (blue < 0 || blue > 255)
				{
					recordJobError(job, "color at index (" + std::to_string(x) + ", " + std::to_string(y) + ") blue value is out of range\ncolor: " + jsGetObjectAsString(value));

					return false;
				}

				job.blocks[x][y] = sf::Color(red, green, blue);
			}
		}

		return true;
	}

	void generate(GenerationJob & job)
	{
		JsCreateContext(runtime, &context);
			
		JsSetCurrentContext(context);

		size_t memUsed;

		JsGetRuntimeMemoryUsage(runtime, &memUsed);

		int megs = (float)memUsed / (1000*1000);

		std::cout << "using " << megs << " MB\n";

		populateGeneratorInterface();

		JsErrorCode code = JsRunScript(job.script.c_str(), 0, L"", nullptr);

		if (code == JsErrorScriptCompile || code == JsErrorScriptException)
		{
			recordJobError(job, getExceptionMessage());

			return;
		}

		job.blocks = new sf::Color *[width];

		for (int i = 0; i < width; ++i)
			job.blocks[i] = new sf::Color[height];

		if (!checkAndAssignBlocks(job))
			return;

		statusMutex.lock();

		generations.push_back(std::make_pair(job.id, job.blocks));

		statuses[job.id] = GenerationStatus::DONE;

		statusMutex.unlock();
	}


}


GenerationStatus getJobStatus(int jobId)
{
	if (jobId < 0 || jobId >= statuses.size())
		return GenerationStatus::NONE;

	return statuses[jobId];
}

std::string getJobError(int jobId)
{
	for (auto & pair : errors)
		if (pair.first == jobId)
			return pair.second;

	return "no error for this job id";
}

sf::Color ** getJobResult(int jobId)
{
	for (auto & pair : generations)
		if (pair.first == jobId)
			return pair.second;

	return nullptr;
}

void generationThread(int width, int height, const std::vector<std::wstring> & utilityScripts)
{
	JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

	::width = width;
	::height = height;

	::utilityScripts = &utilityScripts;

	inBoundsFunctionSize.x = width;
	inBoundsFunctionSize.y = height;

	while (true)
	{
		if (stopThread)
			return;

		jobMutex.lock();

		if (!jobs.empty())
		{
			GenerationJob job = jobs.front();

			jobs.pop();

			jobMutex.unlock();

			statusMutex.lock();

			statuses[job.id] = GenerationStatus::IN_PROGRESS;

			statusMutex.unlock();

			generate(job);
		}
		else
			jobMutex.unlock();


		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int submitGenerationJob(const std::wstring & script)
{
	GenerationJob job;

	job.id = nextId;
	job.script = script;
	job.blocks = nullptr;

	++nextId;

	jobMutex.lock();
	
	jobs.push(job);

	jobMutex.unlock();

	statusMutex.lock();

	statuses.push_back(GenerationStatus::NOT_STARTED);

	statusMutex.unlock();

	return job.id;
}

void stopGenerationThread()
{
	stopThread = true;
}
