#ifndef GENERATIONTHREAD_H
#define GENERATIONTHREAD_H

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vector>

enum class GenerationStatus
{
	NOT_STARTED,
	IN_PROGRESS,
	ERROR_OCCURED,
	DONE,
	NONE
};

struct GenerationJob
{
	int id;

	std::wstring script;

	sf::Color ** blocks;
};

void generationThread(int width, int height, const std::vector<std::wstring> & utilityScripts);

int submitGenerationJob(const std::wstring & script);

GenerationStatus getJobStatus(int jobId);

std::string getJobError(int jobId);

sf::Color ** getJobResult(int jobId);

void stopGenerationThread();

#endif