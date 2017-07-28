#define NOMINMAX

#include "ChakraCore.h"
#include "JsPrint.h"
#include "JsRandom.h"
#include "jsException.h"
#include "stringUtils.h"
#include "GenerationThread.h"
#include "Random.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <map>



std::wstring loadScriptFile(const std::string & filename)
{
	std::ifstream file(filename);

	std::string line;

	std::vector<std::string> lines;

	while (std::getline(file, line))
	{
		lines.push_back(line.append("\n"));
	}

	std::string script;

	for (auto & line : lines)
		script.append(line);

	std::wstring wscript(script.begin(), script.end());

	return wscript;
}

std::vector<std::string> getFilesInDirectory(const std::string & path)
{
	std::vector<std::string> files;

	WIN32_FIND_DATA data;

	HANDLE handle = FindFirstFile(path.c_str(), &data);

	if (handle == INVALID_HANDLE_VALUE)
		return files;

	files.push_back(data.cFileName);

	while (FindNextFile(handle, &data))
	{
		files.push_back(data.cFileName);
	}

	FindClose(handle);

	return files;
}

void drawLandscapeView(sf::RenderTarget & target, sf::Color ** blocks, int blocksWide, int blocksHigh, int blockSize, int bottomOffset, double worldX, float alpha)
{
	int leftBound = worldX / blockSize - 1;

	if (leftBound < 0)
		leftBound = 0;

	int viewBlocksWide = target.getSize().x / blockSize + 1;

	int rightBound = leftBound + viewBlocksWide + 1;

	if (rightBound >= blocksWide)
		rightBound = blocksWide - 1;

	int leftOffset = worldX - ((leftBound + 1)*blockSize);

	static sf::VertexArray squares(sf::PrimitiveType::Quads);

	int viewXBlocks = rightBound - leftBound + 1;

	int verticesNeeded = viewXBlocks*blocksHigh * 4;

	if (squares.getVertexCount() != verticesNeeded)
		squares.resize(verticesNeeded);

	for (int x = leftBound; x <= rightBound; ++x)
		for (int y = 0; y < blocksHigh; ++y)
		{
			sf::Color & blockColor = blocks[x][y];

			sf::Color color(blockColor.r, blockColor.g, blockColor.b, (int)(alpha * 255));

			sf::Vector2f pos = { (float) (x*blockSize - worldX), (float) (target.getSize().y - (y + 1)*blockSize - bottomOffset) };
			sf::Vector2f size = { (float) blockSize, (float) blockSize };

			sf::Vector2f topLeft = pos;
			sf::Vector2f topRight = { pos.x + size.x, pos.y };
			sf::Vector2f bottomLeft = { pos.x, pos.y + size.y };
			sf::Vector2f bottomRight = { pos.x + size.x, pos.y + size.y };

			sf::Vertex * quad = &squares[((x - leftBound) + y * viewXBlocks) * 4];

			quad[0] = sf::Vertex(topLeft, color);
			quad[1] = sf::Vertex(topRight, color);
			quad[2] = sf::Vertex(bottomRight, color);
			quad[3] = sf::Vertex(bottomLeft, color);
		}

	target.draw(squares);
}

//remove
void drawLandscapeView_old(sf::RenderTarget & target, sf::Color ** blocks, int blocksWide, int blocksHigh, int blockSize, int bottomOffset, float worldX, float alpha)
{
static sf::RectangleShape rectangle;

rectangle.setSize({ (float) blockSize, (float) blockSize });

int leftBound = worldX / blockSize - 1;

if (leftBound < 0)
leftBound = 0;

int viewBlocksWide = target.getSize().x / blockSize + 1;

int rightBound = leftBound + viewBlocksWide + 1;

if (rightBound >= blocksWide)
rightBound = blocksWide - 1;

int leftOffset = worldX - ((leftBound + 1)*blockSize);

sf::VertexArray squares(sf::PrimitiveType::Quads);

for (int x = leftBound; x <= rightBound; ++x)
for (int y = 0; y < blocksHigh; ++y)
{
rectangle.setPosition(x*blockSize - worldX, target.getSize().y - (y + 1)*blockSize - bottomOffset);

//rectangle.move(blockSize*3, 0);

sf::Color blockColor = blocks[x][y];

rectangle.setFillColor(sf::Color(blockColor.r, blockColor.g, blockColor.b, (int) (alpha*255)));

sf::Color color(blockColor.r, blockColor.g, blockColor.b, (int)(alpha * 255));

sf::Vector2f pos = rectangle.getPosition();
sf::Vector2f size = rectangle.getSize();

sf::Vector2f topLeft = pos;
sf::Vector2f topRight = { pos.x + size.x, pos.y };
sf::Vector2f bottomLeft = { pos.x, pos.y + size.y };
sf::Vector2f bottomRight = { pos.x + size.x, pos.y + size.y };

squares.append(sf::Vertex(topLeft, color));
squares.append(sf::Vertex(topRight, color));
squares.append(sf::Vertex(bottomRight, color));
squares.append(sf::Vertex(bottomLeft, color));



//CLEAN UP RENDER CODE

//target.draw(rectangle);
}

target.draw(squares);
}


std::vector<int> generateRandomBag(int size)
{
	Random random;

	std::vector<int> remainingIndices;

	for (int i = 0; i < size; ++i)
		remainingIndices.push_back(i);

	std::vector<int> bag;

	while (!remainingIndices.empty())
	{
		int index = random.getInt(remainingIndices.size());

		bag.push_back(remainingIndices[index]);

		remainingIndices.erase(std::find(remainingIndices.begin(), remainingIndices.end(), remainingIndices[index]));
	}

	return std::move(bag);
}


struct CommandOption
{
	std::string flag;
	std::string value;

	bool hasValue;
};

std::vector<CommandOption> findCommands(const std::vector<std::string> & args)
{
	std::vector<CommandOption> commands;

	bool previousArgFlag = false;

	const std::string * lastFlag = nullptr;

	for (auto & arg : args)
	{
		bool isFlag = arg.length() > 1 && (arg[0] == '-' || arg[0] == '/');

		if (!isFlag)
		{
			if (previousArgFlag)
			{
				CommandOption option;

				option.flag = lastFlag->substr(1);
				option.hasValue = true;
				option.value = arg;

				commands.push_back(option);

				previousArgFlag = false;
			}
		}

		if (isFlag)
		{
			if (previousArgFlag)
			{
				CommandOption option;

				option.flag = lastFlag->substr(1);
				option.hasValue = false;

				commands.push_back(option);

				lastFlag = &arg;
			}
			else
			{
				previousArgFlag = true;

				lastFlag = &arg;
			}
		}
	}

	if (previousArgFlag)
	{
		CommandOption option;

		option.flag = lastFlag->substr(1);
		option.hasValue = false;

		commands.push_back(option);
	}

	return std::move(commands);
}


int main(int argc, char * argv[])
{
	std::vector<std::string> args;

	for (int i = 1; i < argc; ++i)
		args.push_back(argv[i]);

	std::vector<CommandOption> commands = findCommands(args);

	std::map<std::string, CommandOption *> stringCommandMap;

	for (CommandOption & command : commands)
		stringCommandMap[command.flag] = &command;
	
	sf::Font font;

	font.loadFromFile("dijkstra.ttf");

	std::vector<std::string> files = getFilesInDirectory("landscapes/*.js");

	std::vector<std::wstring> scriptContents;

	//make more efficient structure for this
	std::map<std::string, std::wstring> scriptFilenameTextMap;

	for (auto & file : files)
	{
		std::wstring contents = loadScriptFile("landscapes/" + file);

		scriptFilenameTextMap[file] = contents;

		scriptContents.push_back(contents);
	}

	std::vector<std::string> utilityScriptFiles = getFilesInDirectory("utilityScripts/*.js");

	std::vector<std::wstring> utilityScripts;

	for (auto file : utilityScriptFiles)
		utilityScripts.emplace_back(loadScriptFile("utilityScripts/" + file));

	bool manualScrollMode = false;

	std::string * scriptToTestName = nullptr;

	int scriptsBeforeCut = scriptContents.size();

	//make this more efficient
	if (stringCommandMap.find("test") != stringCommandMap.end())
	{
		manualScrollMode = true;

		CommandOption * command = stringCommandMap.find("test")->second;

		if (command->hasValue)
		{
			scriptToTestName = &command->value;

			for (auto & pair : scriptFilenameTextMap)
				if (pair.first != command->value)
				{
					auto iter = std::find(scriptContents.begin(), scriptContents.end(), pair.second);

					if (iter != scriptContents.end())
						scriptContents.erase(iter);
				}
		}
	}

	bool cutOutAllScripts = false;

	if (scriptContents.size() == 0 && scriptsBeforeCut != 0)
		cutOutAllScripts = true;


	bool reportMousePosition = false;

	if (stringCommandMap.find("mousePosition") != stringCommandMap.end())
		reportMousePosition = true;

	bool setDesktopBackground = false;

	if (stringCommandMap.find("setBackground") != stringCommandMap.end())
	{
		setDesktopBackground = true;
		
		manualScrollMode = false;
	}


	bool needNextLandscape = true;
	bool generatingLandscape = false;
	bool generatedFirstLandscape = false;
	bool transitioning = false;
	bool outOfLandscapes = false;
	bool inTransitionPosition = false;
	bool eventsTriggerStop = false;

	int firstJobId = -1;
	int currentJobId = -1;
	int lastGeneratedIndex = -1;
	int firstGeneratedIndex = -1;

	const int blocksWide = 300;
	const int blocksHigh = 50;
	const int updatesPerSecond = 30;
	const int framesPerSecond = 30;

	double worldBlockPos = 0;

	const double blocksScrolledPerSecond = 6;
	const double mouseBlocksScrolledPerSecondBase = 2;
	const double mouseBlocksScrolledPerSecondRange = 32;
	const double transitionTime = 1;
	const double scrollRegionWindowFraction = 0.2;

	std::thread thread(generationThread, blocksWide, blocksHigh, utilityScripts);

	
	std::cout << "Generation thread started [" << thread.get_id() << "]\n";

	std::vector<int> bag;

	sf::Color ** currentBlocks = nullptr;
	sf::Color ** nextBlocks = nullptr;

	const sf::VideoMode mode = manualScrollMode ? sf::VideoMode(640, 480) : sf::VideoMode::getFullscreenModes()[0];

	sf::RenderWindow window;

	window.setVerticalSyncEnabled(true);

	int blockSize = mode.height / blocksHigh;

	int viewWidth = blockSize*(mode.width / blockSize);
	int viewHeight = blockSize*blocksHigh;

	int bottomOffset = (mode.height - blockSize*blocksHigh) / 2;

	sf::RenderTexture * renderTexture = nullptr;

	if (setDesktopBackground)
	{
		renderTexture = new sf::RenderTexture();

		renderTexture->create(mode.width, mode.height);
	}

	sf::Clock updateClock;
	sf::Clock drawClock;
	sf::Clock deltaClock;
	sf::Clock transitionClock;
	sf::Clock runtimeClock;

	sf::Text noLandscapesText;

	noLandscapesText.setFont(font);
	noLandscapesText.setCharacterSize(40);
	noLandscapesText.setFillColor(sf::Color::White);
	noLandscapesText.setString("No landscapes to display");
	noLandscapesText.setOrigin(noLandscapesText.getLocalBounds().width / 2, noLandscapesText.getLocalBounds().height / 2);
	noLandscapesText.setPosition(mode.width / 2, 80);

	sf::Text noScriptsText;

	noScriptsText.setFont(font);
	noScriptsText.setCharacterSize(40);
	noScriptsText.setFillColor(sf::Color::White);
	noScriptsText.setString("No scripts to run");
	noScriptsText.setOrigin(noScriptsText.getLocalBounds().width / 2, noScriptsText.getLocalBounds().height / 2);
	noScriptsText.setPosition(mode.width / 2, 80);

	sf::Text errorsText;

	errorsText.setFont(font);
	errorsText.setCharacterSize(25);
	errorsText.setFillColor(sf::Color::White);

	std::map<std::string, std::string> errors;

	if (cutOutAllScripts)
	{
		window.create(mode, "Landscapes", manualScrollMode ? sf::Style::Default : sf::Style::Fullscreen);

		std::string errorString;

		errorString.append("No script file with name \"");
		errorString.append(*scriptToTestName);
		errorString.append("\" found");

		errorsText.setString(errorString);
		errorsText.setPosition(20, noLandscapesText.getGlobalBounds().top + noLandscapesText.getGlobalBounds().height + 20);

		while (errorsText.getGlobalBounds().left + errorsText.getGlobalBounds().width >= window.getSize().x)
			errorsText.setCharacterSize(errorsText.getCharacterSize() - 1);
	}

	while (true)
	{
		if (updateClock.getElapsedTime().asSeconds() > 1.f / updatesPerSecond || !generatedFirstLandscape)
		{
			updateClock.restart();

			if (!eventsTriggerStop && runtimeClock.getElapsedTime().asSeconds() > 3)
				eventsTriggerStop = true;

			if (!cutOutAllScripts)
			{
				if (needNextLandscape)
				{
					if (bag.empty())
						bag = generateRandomBag(scriptContents.size());

					if (bag.empty())
					{
						outOfLandscapes = true;

						needNextLandscape = false;

						generatingLandscape = false;

						if (!window.isOpen() && !setDesktopBackground)
							window.create(mode, "Landscapes", manualScrollMode ? sf::Style::Default : sf::Style::Fullscreen);

						std::string errorString;

						for (auto & pair : errors)
							errorString.append(pair.first).append(": ").append(pair.second).append("\n");

						errorsText.setString(errorString);
						errorsText.setPosition(20, noLandscapesText.getGlobalBounds().top + noLandscapesText.getGlobalBounds().height + 20);

						while (errorsText.getGlobalBounds().left + errorsText.getGlobalBounds().width >= window.getSize().x)
							errorsText.setCharacterSize(errorsText.getCharacterSize() - 1);

						continue;
					}

					int index = bag[0];

					if (!generatedFirstLandscape)
						firstGeneratedIndex = index;
					else
						lastGeneratedIndex = index;

					bag.erase(bag.begin());

					int id = submitGenerationJob(scriptContents[index]);

					if (!generatedFirstLandscape)
						firstJobId = id;
					else
						currentJobId = id;

					generatingLandscape = true;

					needNextLandscape = false;
				}

				if (generatingLandscape)
				{
					GenerationStatus status = getJobStatus(!generatedFirstLandscape ? firstJobId : currentJobId);

					if (status == GenerationStatus::DONE)
					{
						if (!generatedFirstLandscape)
						{
							generatedFirstLandscape = true;

							currentBlocks = getJobResult(firstJobId);

							if (!manualScrollMode)
								needNextLandscape = true;

							if (!setDesktopBackground)
								window.create(mode, "Landscapes", manualScrollMode ? sf::Style::Default : sf::Style::Fullscreen);
						}
						else
							nextBlocks = getJobResult(currentJobId);

						generatingLandscape = false;
					}

					if (status == GenerationStatus::ERROR_OCCURED)
					{
						std::string errorMessage = getJobError(!generatedFirstLandscape ? firstJobId : currentJobId);

						int index = !generatedFirstLandscape ? firstGeneratedIndex : lastGeneratedIndex;

						std::wstring scriptText = scriptContents[index];

						std::string scriptName;

						for (auto & pair : scriptFilenameTextMap)
							if (pair.second == scriptText)
							{
								scriptName = pair.first;

								break;
							}

						std::cout << "Error in file " << scriptName << ": " << errorMessage << "\n";

						errors[scriptName] = errorMessage;

						scriptContents.erase(scriptContents.begin() + index);

						bag = generateRandomBag(scriptContents.size());

						needNextLandscape = true;

						generatingLandscape = false;

						continue;
					}

					if (!generatedFirstLandscape)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(50));

						continue;
					}
				}
			}
			
			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					stopGenerationThread();

					thread.join();

					return 0;
				}

				if (event.type == sf::Event::KeyPressed)
				{
					sf::Keyboard::Key key = event.key.code;

					if (key == sf::Keyboard::Escape)
					{
						stopGenerationThread();

						thread.join();

						return 0;
					}

					if (!manualScrollMode)
					{
						stopGenerationThread();

						thread.join();

						return 0;
					}
				}

				if (event.type == sf::Event::MouseMoved)
				{
					if (!manualScrollMode && eventsTriggerStop)
					{
						stopGenerationThread();

						thread.join();

						return 0;
					}
				}

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (!manualScrollMode)
					{
						stopGenerationThread();

						thread.join();

						return 0;
					}

					if (manualScrollMode && reportMousePosition)
					{
						int x = (int)worldBlockPos + event.mouseButton.x / blockSize;
						int y = (window.getSize().y - bottomOffset - event.mouseButton.y) / blockSize;

						std::cout << "Mouse: (" << x << ", " << y << ")\n";
					}
				}

				if (event.type == sf::Event::Resized)
				{
					if (outOfLandscapes)
						continue;

					sf::Vector2u size = window.getSize();

					if (size.y < blocksHigh)
						window.setSize({ (unsigned int)size.x, (unsigned int)blocksHigh });

					size = window.getSize();

					sf::View newView;

					newView.setSize({ (float)size.x, (float)size.y });
					newView.setCenter({ size.x / 2.f, size.y / 2.f });

					window.setView(newView);

					blockSize = window.getSize().y / blocksHigh;

					viewWidth = blockSize*(window.getSize().x / blockSize);
					viewHeight = blockSize*blocksHigh;

					bottomOffset = (window.getSize().y - blockSize*blocksHigh) / 2;
				}
			}
		}

		if (drawClock.getElapsedTime().asSeconds() > 1.f / framesPerSecond)
		{
			drawClock.restart();

			window.clear(sf::Color::Black);

			if (outOfLandscapes)
			{
				window.draw(noLandscapesText);
				window.draw(errorsText);

				window.display();

				continue;
			}

			if (cutOutAllScripts)
			{
				window.draw(noScriptsText);
				window.draw(errorsText);

				window.display();

				continue;
			}

			int delta = deltaClock.restart().asMilliseconds();

			float deltaRatio = delta / 1000.f;

			float furthestRightPosition = blocksWide - (float)(setDesktopBackground ? renderTexture->getSize().x : window.getSize().x) / blockSize;

			if (setDesktopBackground)
			{
				worldBlockPos = Random().getInt((int)furthestRightPosition);

				bottomOffset = renderTexture->getSize().y - blocksHigh*blockSize;

				drawLandscapeView(*renderTexture, currentBlocks, blocksWide, blocksHigh, blockSize, bottomOffset, worldBlockPos*blockSize, 1);

				renderTexture->display();

				renderTexture->getTexture().copyToImage().saveToFile("desktopBackground.png");

				TCHAR buffer[MAX_PATH];

				GetCurrentDirectory(MAX_PATH, buffer);

				std::string picturePath = buffer;

				picturePath.append("\\desktopBackground.png");

				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void *)picturePath.c_str(), SPIF_UPDATEINIFILE);

				stopGenerationThread();

				thread.join();

				return 0;
			}

			if (manualScrollMode)
			{
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

				sf::IntRect windowRect = { 0, 0, (int) window.getSize().x, (int) window.getSize().y };

				if (windowRect.contains(mousePosition))
				{
					int barSize = window.getSize().x * scrollRegionWindowFraction;

					if (mousePosition.x < barSize)
						worldBlockPos -= (mouseBlocksScrolledPerSecondBase + mouseBlocksScrolledPerSecondRange*(1 - (double)mousePosition.x / barSize))*deltaRatio;

					if (mousePosition.x >= window.getSize().x - barSize)
						worldBlockPos += (mouseBlocksScrolledPerSecondBase + mouseBlocksScrolledPerSecondRange*(((double) mousePosition.x - (window.getSize().x - barSize)) / barSize))*deltaRatio;

					if (worldBlockPos < 0)
						worldBlockPos = 0;

					if (worldBlockPos > furthestRightPosition)
						worldBlockPos = furthestRightPosition;
				}
			}
			else
			{
				if (!inTransitionPosition)
					worldBlockPos += blocksScrolledPerSecond*deltaRatio;

				if (worldBlockPos > furthestRightPosition)
				{
					inTransitionPosition = true;

					worldBlockPos = furthestRightPosition;
				}

				if (!transitioning && inTransitionPosition && !generatingLandscape)
				{
					transitioning = true;

					transitionClock.restart();
				}
			}

			if (!transitioning)
				drawLandscapeView(window, currentBlocks, blocksWide, blocksHigh, blockSize, bottomOffset, worldBlockPos*blockSize, 1);
			else
			{
				double transitionPercent = transitionClock.getElapsedTime().asSeconds() / transitionTime;

				if (transitionPercent > 1)
				{
					transitioning = false;

					for (int x = 0; x < blocksWide; ++x)
						delete currentBlocks[x];

					delete currentBlocks;

					currentBlocks = nextBlocks;

					needNextLandscape = true;

					worldBlockPos = 0;

					inTransitionPosition = false;

					continue;
				}

				drawLandscapeView(window, currentBlocks, blocksWide, blocksHigh, blockSize, bottomOffset, worldBlockPos*blockSize, 1 - transitionPercent);
				drawLandscapeView(window, nextBlocks, blocksWide, blocksHigh, blockSize, bottomOffset, 0, transitionPercent);
			}

			window.display();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}