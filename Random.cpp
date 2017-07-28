#include "Random.h"
#include <limits>

Random::Random() : max(engine.max())
{
	std::random_device device;

	engine.seed(device());
}

Random::Random(long seed)
{
	engine.seed(seed);
}

void Random::setSeed(long seed)
{
	engine.seed(seed);
}

bool Random::getBool()
{
	int i = getInt(2);

	return i == 0 ? true : false;
}

unsigned char Random::getByte()
{
	std::uniform_int_distribution<int> dist(0, std::numeric_limits<unsigned char>::max());

	return (unsigned char)(dist(engine));
}

unsigned short Random::getShort()
{
	std::uniform_int_distribution<int> dist(0, std::numeric_limits<unsigned short>::max());

	return (unsigned short)(dist(engine));
}

unsigned int Random::getInt()
{
	std::uniform_int_distribution<unsigned int> dist(0, std::numeric_limits<unsigned int>::max());

	return (unsigned int)(dist(engine));
}

unsigned int Random::getInt(int max)
{
	std::uniform_int_distribution<unsigned int> dist(0, (max - 1) >= 0 ? max - 1 : 0);

	return (unsigned int)(dist(engine));
}

unsigned long Random::getLong()
{
	std::uniform_int_distribution<unsigned long> dist(0, std::numeric_limits<unsigned long>::max());

	return (unsigned long)(dist(engine));
}

unsigned long Random::getLong(long max)
{
	std::uniform_int_distribution<unsigned long> dist(0, max - 1);

	return (unsigned long)(dist(engine));
}

float Random::getFloat()
{
	std::uniform_real_distribution<float> dist(0, 1);

	return dist(engine);
}

double Random::getDouble()
{
	std::uniform_real_distribution<double> dist(0, 1);

	return dist(engine);
}

int Random::randomSign()
{
	bool pos = getBool();

	return pos ? 1 : -1;
}
