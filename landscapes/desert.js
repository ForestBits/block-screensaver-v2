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

		heightMap[x] = currentHeight;
	}
}

function generateDunes(baseHeight, duneFrequency, duneMinBlocksApart, duneStartIncrement, duneDecrement, duneStartIncrementVariance, duneDecrementVariance, heightMap)
{
	var inDune = false;

	var currentHeight;
	var currentDuneRate;
	var currentDuneDecrement;

	var lastDuneX = 0;

	for (var x = 0; x < blocksWide; ++x)
	{
		if (!inDune && random.getDecimal() < duneFrequency && (lastDuneX == 0 || x - lastDuneX >= duneMinBlocksApart))
		{
			inDune = true;

			currentHeight = heightMap[x];

			currentDuneRate = duneStartIncrement + random.getSign()*duneStartIncrementVariance*random.getDecimal();

			currentDuneDecrement = duneDecrement + random.getSign()*duneDecrementVariance*random.getDecimal();
		}

		if (!inDune)
			continue;

		if (currentHeight > heightMap[x])
			heightMap[x] = currentHeight;

		currentHeight += currentDuneRate;

		currentDuneRate -= currentDuneDecrement;

		if (x != blocksWide - 1 && currentHeight <= heightMap[x + 1])
		{
			inDune = false;

			lastDuneX = x;
		}
	}
}

function fillSand(heightMap)
{
	for (var x = 0; x < blocksWide; ++x)
		for (var y = 0; y < heightMap[x]; ++y)
			blocks[x][y] = SAND;
}

function generateCactus(x, y, height)
{
	for (var yPos = y; yPos < y + height; ++yPos)
		blocks[x][yPos] = CACTUS;

	var leftArm = random.getBoolean();
	var rightArm = random.getBoolean();

	if (leftArm && x > 1)
	{
		var armStart = random.getInteger(2, height - 1);

		var armHeight = random.getInteger(2, math.ceil(height/2));

		for (var yPos = armStart; yPos < armStart + armHeight; ++yPos)
			blocks[x - 2][yPos + y] = CACTUS;

		blocks[x - 1][y + armStart] = CACTUS;
	}

	if (rightArm && x < blocksWide - 2)
	{
		var armStart = random.getInteger(2, height - 1);

		var armHeight = random.getInteger(2, math.ceil(height/2));

		for (var yPos = armStart; yPos < armStart + armHeight; ++yPos)
			blocks[x + 2][yPos + y] = CACTUS;

		blocks[x + 1][y + armStart] = CACTUS;
	}
}

function findFlatAreas(maxHeight, distanceRequired, heightMap)
{
	var areas = [];

	var startX = 0;
	var count = 0;

	for (var x = 0; x < blocksWide; ++x)
	{
		if (heightMap[x] <= maxHeight)
		{
			if (count == 0)
				startX = x;

			++count;
		}
		else
		{
			if (count >= distanceRequired)
			{
				areas.push({x: startX, length: count});

				count = 0;
			}
		}
	}

	if (count >= distanceRequired)
		areas.push({x: startX, length: count});

	return areas;
}

function generateVulture(x, y)
{
	var points = 
	[
		new Point(x - 1, y),
		new Point(x + 1, y),
		new Point(x - 2, y),
		new Point(x + 2, y),
		new Point(x, y - 1)
	];

	for (var i = 0; i < points.length; ++i)
	{
		var point = points[i];

		if (inBounds(point))
			blocks[point.x][point.y] = VULTURE;
	}
}


var SAND = new Color(255, 214, 125);
var SKY = new Color(144, 195, 212);
var CACTUS = new Color(0, 89, 12);
var VULTURE = new Color(25, 25, 25);


setBackgroundBlocks(SKY);

var heightMap = [];

generateGround(5, 10, 1, heightMap);

generateDunes(5, 0.02, 20, 1.5, 0.05, 0.25, 0, heightMap);

fillSand(heightMap);

var areas = findFlatAreas(10, 10, heightMap);

for (var i = 0; i < areas.length; ++i)
{
	var area = areas[i];

	for (var x = area.x + 4; x < area.x + area.length - 4; ++x)
		if (random.getDecimal() < 0.05)
		{
			generateCactus(x, heightMap[x], random.getInteger(5, 9));

			x += 10;
		}
}

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < 0.01)
	{
		generateVulture(x, random.getInteger(40, 49));

		x += 25;
	}