api.export();

var SKY = new Color(135, 206, 235);
var DIRT = new Color(101, 67, 33);
var GRASS = new Color(53, 94, 59);
var TRUNK = new Color(88, 67, 18);
var LEAVES = new Color(89, 201, 60);

for (var x = 0; x < blocksWide; ++x)
	for (var y = 0; y < blocksHigh; ++y)
		blocks[x][y] = SKY;

var groundBaseHeight = blocksHigh/5;
var maxDifference = 3;

for (var x = 0; x < blocksWide; ++x)
{
	var height = groundBaseHeight + random.getInteger(0, 3)*random.getSign();

	for (var y = 0; y < height - 1; ++y)
		blocks[x][y] = DIRT;

	blocks[x][height - 1] = GRASS;
}

for (var treePos = random.getInteger(20); treePos < blocksWide; treePos += 5 + random.getInteger(15))
{
	var groundHeight = 0;

	var treeHeight = 10 + random.getInteger(10);

	for (; blocks[treePos][groundHeight] != SKY; ++groundHeight)
		;

	for (var y = groundHeight; y < groundHeight + treeHeight; ++y)
		blocks[treePos][y] = TRUNK;

	var treeTop = {x: treePos, y: groundHeight + treeHeight};

	var maxDistance = 3 + random.getDecimal()*3;
	var fillPercent = 0.8;

	for (var y = treeTop.y; y > treeTop.y - math.ceil(maxDistance); --y)
		blocks[treeTop.x][y] = LEAVES;

	for (var x = treeTop.x - math.ceil(maxDistance); x < treeTop.x + math.ceil(maxDistance); ++x)
		for (var y = treeTop.y - math.ceil(maxDistance); y < treeTop.y + math.ceil(maxDistance); ++y)
		{
			if (!inBounds(x, y))
				continue;

			if (random.getDecimal() > fillPercent)
				continue;

			if (getDistance(x, y, treeTop.x, treeTop.y) < maxDistance)
				blocks[x][y] = LEAVES;
		}
}
