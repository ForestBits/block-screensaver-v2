api.export();

var SKY = new Color(135, 206, 235);
var SAND = new Color(250, 225, 115);
var CACTUS = new Color(35, 140, 0);

for (var x = 0; x < blocksWide; ++x)
	for (var y = 0; y < blocksHigh; ++y)
		blocks[x][y] = SKY;

var groundBaseHeight = blocksHigh/5;
var maxDifference = 3;

for (var x = 0; x < blocksWide; ++x)
{
	var height = groundBaseHeight + random.getInteger(0, 3)*random.getSign();

	for (var y = 0; y < height - 1; ++y)
		blocks[x][y] = SAND;
}

for (var treePos = random.getInteger(20); treePos < blocksWide; treePos += 5 + random.getInteger(15))
{
	var groundHeight = 0;

	var treeHeight = 10 + random.getInteger(10);

	for (; blocks[treePos][groundHeight] != SKY; ++groundHeight)
		;

	for (var y = groundHeight; y < groundHeight + treeHeight; ++y)
		blocks[treePos][y] = CACTUS;
}

for (var y = 0; y < blocksHigh; ++y)
	blocks[blocksWide - 1][y] = SAND;