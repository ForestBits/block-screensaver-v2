api.export();

function generateGround(minHeight, maxHeight, maxStep, heightMap)
{
	var currentHeight = random.getInteger(minHeight, maxHeight);

	for (var x = 0; x < blocksWide; ++x)
	{
		var step = random.getSign()*random.getInteger(0, maxStep);

		currentHeight += step;

		if (currentHeight < minHeight)
			currentHeight = minHeight;

		if (currentHeight > maxHeight)
			currentHeight = maxHeight;

		if (heightMap[x] < currentHeight)
			heightMap[x] = currentHeight;
	}
}

function fillGround(heightMap)
{
	for (var x = 0; x < blocksWide; ++x)
		for (var y = 0; y < heightMap[x]; ++y)
			blocks[x][y] = SNOW;
}

function generateTreeLeaves(x, y, radius, increment)
{
	var leftBound = x - radius;
	var rightBound = x + radius;

	var height = increment;

	for (var xPos = leftBound; xPos <= rightBound; ++xPos)
	{
		if (xPos >= 0 && xPos < blocksWide)
			for (var yPos = y; yPos < y + height; ++yPos)
				blocks[xPos][yPos] = LEAVES;

		if (xPos < x)
			height += increment;
		else
			height -= increment;
	}
}

/*function generateTreeLeaves(x, y, radius, increment)
{
	var leftBound = x - radius;
	var rightBound = x + radius;

	var height = y;

	for (var xPos = leftBound; xPos <= rightBound; ++xPos)
	{
		if (xPos < 0 || xPos >= blocksWide)
			continue;

		for (var yPos = y; yPos < y + height; ++yPos)
			blocks[xPos][yPos] = LEAVES;

		if (xPos < x)
			height += increment;
		else
			height -= increment;
	}
}*/

function generateTree(x, y, trunkHeight, radius, increment)
{
	for (var yPos = y; yPos < y + trunkHeight; ++yPos)
		if (blocks[x][yPos] == SKY)
			blocks[x][yPos] = WOOD;

	generateTreeLeaves(x, y + trunkHeight, radius, increment);
}

function generateSnow(xChance, yChance, heightMap)
{
	for (var x = 0; x < blocksWide; ++x)
		if (random.getDecimal() < xChance)
		{
			for (var y = blocksHigh - random.getInteger(1, 5); ; --y)
			{
				if (blocks[x][y] != SKY)
					break;

				if (random.getDecimal() < yChance)
				{
					blocks[x][y] = SNOW;

					--y;
				}
			}
		}

	for (var x = 0; x < blocksWide; ++x)
		for (var y = blocksHigh - 1; ; --y)
		 {
			var block = blocks[x][y];

			if (block == LEAVES)
			{
				blocks[x][y + 1] = SNOW;
				blocks[x][y + 2] = SNOW;

				break;
			}

			if ((block != SNOW && block != SKY) || y < heightMap[x])
				break; 
		}
}


var SKY = new Color(144, 195, 212);
var SNOW = new Color(255, 255, 255);
var WOOD = new Color(97, 74, 0);
var LEAVES = new Color(0, 99, 26);

setBackgroundBlocks(SKY);

var heightMap = new Array(blocksWide);

for (var i = 0; i < blocksWide; ++i)
	heightMap[i] = 0;

generateGround(3, 10, 1, heightMap);

fillGround(heightMap);

var treePercent = 0.05;

for (var x = 0; x < blocksWide; ++x)
{
	if (random.getDecimal() < treePercent)
	{
		generateTree(x, heightMap[x], random.getInteger(5, 15), random.getInteger(7, 14), 1.5 + random.getDecimal()*0.5);

		x += 4;
	}
}

generateSnow(1, 0.1, heightMap);

//USE TREE< CITY_SCAPE

//both skyline and urban?

//something with helicopter
//forest fire?
//ANTARTIC