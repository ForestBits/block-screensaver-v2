#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random
{
	std::default_random_engine engine;

	unsigned long max;

public:
	Random();

	Random(long seed);

	void setSeed(long seed);

	bool getBool();

	unsigned char getByte();

	unsigned short getShort();

	unsigned int getInt();

	unsigned int getInt(int max);

	unsigned long getLong();

	unsigned long getLong(long max);

	float getFloat();

	double getDouble();

	int randomSign();
};


#endif