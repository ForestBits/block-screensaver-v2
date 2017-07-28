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

function generateMountain(centerX, width, height)
{
	var halfSize = math.round(width/2);

	var leftBound = centerX - halfSize >= 0 ? centerX - halfSize : 0;
	var rightBound = centerX + halfSize < blocksWide ? centerX + halfSize : blocksWide - 1;

	for (var x = leftBound; x <= rightBound; ++x)
	{
		var heightPercent;

		if (x != centerX)
			heightPercent = 1 - math.abs(centerX - x)/halfSize;
		else
			heightPercent = 1;

		var columnHeight = math.floor(heightPercent*height);

		if (columnHeight > heightMap[x])
			heightMap[x] = columnHeight;
	}
}

function snowCapMountains()
{
	for (var x = 0; x < blocksWide; ++x)
		for (var y = snowHeight; y < blocksHigh; ++y)
			if (blocks[x][y] == MOUNTAIN)
				blocks[x][y] = SNOW;
}

function generateCircle(x, y, radius, color, blend)
{
	var biggerRadius = math.ceil(radius);

	var leftBound = x - biggerRadius;
	var rightBound = x + biggerRadius;
	var topBound = y + biggerRadius;
	var bottomBound = y - biggerRadius;

	if (leftBound < 0)
		leftBound = 0;

	if (rightBound >= blocksWide)
		rightBound = blocksWide - 1;

	if (topBound >= blocksHigh)
		topBound = blocksHigh - 1;

	if (bottomBound < 0)
		bottomBound = 0;

	var center = new Point(x, y);

	for (var xPos = leftBound; xPos <= rightBound; ++xPos)
		for (var yPos = bottomBound; yPos <= topBound; ++yPos)
		{
			var distance = center.distance(new Point(xPos, yPos));

			if (distance <= radius)
				blocks[xPos][yPos] = blend ? color.mixPercent(blocks[xPos][yPos], 0.05) : color;
		}
}

function generateCloud(x, y, length, layers, layersDown)
{
	var originalLength = length;

	for (var layer = 0; layer < layers; ++layer)
	{
		var yPos = y + layer;

		var layerLength = length - layer*3;

		var xPos = x + random.getInteger(originalLength - layerLength);

		for (var layerStart = xPos; xPos < layerStart + layerLength; ++xPos)
			generateCircle(xPos, yPos, 3, CLOUD, true);

		--length;
	}

	for (var layer = 0; layer < layersDown; ++layer)
	{
		var yPos = y - layer;

		var layerLength = length - layer*3;

		var xPos = x + random.getInteger(originalLength - layerLength);

		for (var layerStart = xPos; xPos < layerStart + layerLength; ++xPos)
			generateCircle(xPos, yPos, 3, CLOUD, true);

		--length;
	}
}

function findValleySpaces(height, spaceNeeded)
{
	var inRow = 0;
	var startSpace = 0;
	var sum = 0;

	var locations = [];

	for (var x = 0; x < blocksWide; ++x)
	{
		if (heightMap[x] <= height)
		{
			if (inRow == 0)
			{
				startSpace = x;

				sum = 0;
			}

			++inRow;

			sum += heightMap[x];
		}
		else
		{
			if (inRow >= spaceNeeded)
				locations.push({x: startSpace, length: inRow, average: math.floor(sum/inRow)});

			inRow = 0;
		}
	}

	if (inRow >= spaceNeeded)
		locations.push({x: startSpace, length: inRow, average: math.floor(sum/inRow)});

	return locations;
}

function generateCabin(valley)
{
	var halfLength = math.floor(valley.length/2);

	var cabinWidth = 11;
	var cabinHeight = 6;
	var cabinStartX = (valley.x + valley.length) - (halfLength + math.floor(cabinWidth/2));

	var cabinCenter = new Point(cabinStartX + math.floor(cabinWidth/2), valley.average + math.floor(cabinHeight/2));

	var flattenRadius = 3;
	var leftFlattenBound = cabinStartX - flattenRadius
	var rightFlattenBound = cabinStartX + cabinWidth + flattenRadius;

	if (leftFlattenBound < 0)
		leftFlattenBound = 0;

	if (rightFlattenBound >= blocksWide)
		rightFlattenBound - blocksWide - 1;


	for (var x = cabinStartX; x < cabinStartX + cabinWidth; ++x)
		heightMap[x] = valley.average;

	for (var x = leftFlattenBound; x <= rightFlattenBound; ++x)
		if (heightMap[x] > valley.average)
			heightMap[x] = valley.average;

	for (var x = cabinStartX; x < cabinStartX + cabinWidth; ++x)
		for (var y = 0; y < valley.average; ++y)
			blocks[x][y] = MOUNTAIN;

	for (var x = cabinStartX; x < cabinStartX + cabinWidth; ++x)
		for (var y = valley.average; y < valley.average + cabinHeight; ++y)
			blocks[x][y] = WOOD;

	for (var x = cabinCenter.x - 1; x <= cabinCenter.x + 1; ++x)
		for (y = valley.average; y < valley.average + 3; ++y)
			blocks[x][y] = CABIN_DOOR;

	var chimneyPosition = new Point(cabinCenter.x + 1, valley.average + cabinHeight);
	var chimneyColumns = 3;
	var chimneyHeightStep = 2;

	/*for (var x = chimneyPosition.x; x < chimneyPosition.x + chimneyColumns; ++x)
		for (var y = chimneyPosition.y; y < chimneyPosition.y + chimneyHeightStep*(x - chimneyPosition.x + 1); ++y)
			blocks[x][y] = CHIMNEY;*/

	for (var y = chimneyPosition.y; y < chimneyPosition.y + 1; ++y)
		blocks[chimneyPosition.x - 1][y] = CHIMNEY;

	for (var y = chimneyPosition.y; y < chimneyPosition.y + 1; ++y)
		blocks[chimneyPosition.x + 1][y] = CHIMNEY;

	for (var y = chimneyPosition.y; y < chimneyPosition.y + 4; ++y)
		blocks[chimneyPosition.x][y] = CHIMNEY;

	var leftWindowPosition = new Point(cabinStartX + 1, valley.average + 3);
	var rightWindowPosition = new Point(cabinStartX + cabinWidth - 3, valley.average + 3);

	for (var x = leftWindowPosition.x; x < leftWindowPosition.x + 2; ++x)
		for (var y = leftWindowPosition.y; y < leftWindowPosition.y + 2; ++y)
			blocks[x][y] = WINDOW;

	for (var x = rightWindowPosition.x; x < rightWindowPosition.x + 2; ++x)
		for (var y = rightWindowPosition.y; y < rightWindowPosition.y + 2; ++y)
			blocks[x][y] = WINDOW;
}

var SKY = new Color(144, 195, 212);
var CLOUD = new Color(255, 255, 255);
var SNOW = new Color(255, 255, 255);
var MOUNTAIN = new Color(89, 128, 255);
var WOOD = new Color(130, 90, 0);
var CABIN_DOOR = new Color(61, 42, 0);
var CHIMNEY = new Color(100, 100, 100);
var WINDOW = new Color(145, 228, 255);

setBackgroundBlocks(SKY);

var minHeight = 2;

var maxHeight = 5;

var maxStep = 1;

var heightMap = new Array(blocksWide);

for (var i = 0; i < heightMap.length; ++i)
	heightMap[i] = 0;

var mountainPercent = 0.02;

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < mountainPercent)
	{
		generateMountain(x, random.getInteger(20, 60), random.getInteger(30, 60));
	}

generateGround(minHeight, maxHeight, maxStep, heightMap);

var snowHeight = 35;

var valleys = findValleySpaces(5, 20);

var cabinChance = 0.33;

for (var i = 0; i < valleys.length; ++i)
	if (random.getDecimal() < cabinChance)
		generateCabin(valleys[i]);

for (var x = 0; x < blocksWide; ++x)
	for (var y = 0; y < heightMap[x]; ++y)
		blocks[x][y] = MOUNTAIN;

snowCapMountains();

for (var x = random.getInteger(20); x < blocksWide; ++x)
{
	var length = random.getInteger(20, 40);

	generateCloud(x, 30 + random.getInteger(7), length, random.getInteger(3, 6), random.getInteger(3, 4));

	x += length + random.getInteger(10, 40);
}
