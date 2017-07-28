api.export();

for (var x = 0; x < blocksWide; ++x)
	for (var y = 0; y < blocksHigh; ++y)
		blocks[x][y] = new Color(random.getInteger(256), random.getInteger(256), random.getInteger(256));